/*
    Copyright 2023, Mitch Curtis

    This file is part of Slate.

    Slate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Slate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Slate. If not, see <http://www.gnu.org/licenses/>.
*/

#include "projectmanager.h"

#include <QLoggingCategory>

#include "applicationsettings.h"
#include "imageproject.h"
#include "layeredimageproject.h"
#include "tilesetproject.h"

Q_LOGGING_CATEGORY(lcProjectManager, "app.projectManager")

ProjectManager::ProjectManager(QObject *parent) :
    QObject(parent),
    mProject(nullptr),
    mTemporaryProject(nullptr),
    mProjectCreationFailed(false),
    mSettings(nullptr),
    mReady(false)
{
}

ProjectManager::~ProjectManager()
{
    qCDebug(lcProjectManager) << "destroying ProjectManager";
}

Project *ProjectManager::project() const
{
    return mProject.get();
}

Project *ProjectManager::temporaryProject() const
{
    return mTemporaryProject.get();
}

ApplicationSettings *ProjectManager::applicationSettings() const
{
    return mSettings;
}

void ProjectManager::setApplicationSettings(ApplicationSettings *settings)
{
    if (settings == mSettings)
        return;

    mSettings = settings;
    emit applicationSettingsChanged();
}

bool ProjectManager::isReady() const
{
    return mReady;
}

void ProjectManager::setReady(bool ready)
{
    qCDebug(lcProjectManager) << "setting ready to" << ready;
    if (ready == mReady)
        return;

    mReady = ready;
    emit readyChanged();
}

// Provides a temporary project for client code to use
// while it tries to create/load a new project, so that if
// Project::createNew()/load() fails, the current project
// is not affected.
void ProjectManager::beginCreation(Project::Type projectType)
{
    if (mTemporaryProject) {
        qWarning() << "Already creating new project";
        return;
    }

    mProjectCreationFailed = false;

    if (projectType == Project::ImageType) {
        mTemporaryProject.reset(new ImageProject);
    } else if (projectType == Project::TilesetType) {
        mTemporaryProject.reset(new TilesetProject);
    } else if (projectType == Project::LayeredImageType) {
        mTemporaryProject.reset(new LayeredImageProject);
    }

    qCDebug(lcProjectManager) << "beginning creation of" << mTemporaryProject->typeString() << "project (" << mTemporaryProject.get() << ")";

    connect(mTemporaryProject.get(), &Project::errorOccurred, this, &ProjectManager::onCreationFailed);

    emit temporaryProjectChanged();
}

// If there were no errors from temporaryProject since the call to
// beginCreation(), swaps project with temporaryProject and destroys the temporary project.
bool ProjectManager::completeCreation()
{
    if (mProjectCreationFailed)
        return false;

    if (!mTemporaryProject) {
        qWarning() << "No project was being created";
        return false;
    }

    if (mProject) {
        disconnect(mProject.get(), &Project::urlChanged, this, &ProjectManager::projectUrlChanged);
    }

    QUrl oldProjectUrl;

    if (mProject) {
        // There was a project before the one we're creating.

        oldProjectUrl = mProject->url();

        std::unique_ptr<Project> connectionGuard;
        mProject.swap(connectionGuard);

        // By emitting readyChanged here, we prevent some QML
        // types whose destruction order is undefined from
        // trying to set a project that's not meant for them.
        // For example, the old project (that's currently still open)
        // could be a tileset project, and a TileCanvas could
        // try to set an ImageProject (the new project).

        // By storing the old project in connectionGuard, we're
        // giving classes that keep a pointer to the project
        // a chance to disconnect any connections to it.
        // When readyChanged() is emitted, it will cause
        // e.g. the canvas to call disconnectSignals(),
        // which will succeed because connectionGuard will
        // be alive until the end of this block.
        setReady(false);

        qCDebug(lcProjectManager).nospace() << "nullified old project " << connectionGuard.get()
            << "; about to emit projectChanged()";
        emit projectChanged();
    }

    Q_ASSERT(!mProject);
    Q_ASSERT(mTemporaryProject);

    mProject.swap(mTemporaryProject);

    Q_ASSERT(mProject);
    Q_ASSERT(!mTemporaryProject);

    emit temporaryProjectChanged();

    if (mProject) {
        mProject->setParent(this);
        // Currently projects don't require settings during loading; we only store it there because it's a more
        // logical place than ImageCanvas, which does need it (for events). For that reason, it's OK to set it so late.
        mProject->setSettings(mSettings);

        connect(mProject.get(), &Project::urlChanged, this, &ProjectManager::projectUrlChanged);

        if (mProject->url() != oldProjectUrl)
            projectUrlChanged();
    }

    qCDebug(lcProjectManager) << "creation of" << mProject->typeString() << "project succeeded; about to emit projectChanged()";

    emit projectChanged();

    qCDebug(lcProjectManager) << "emitted projectChanged()";

    setReady(true);

    return true;
}

Project::Type ProjectManager::projectTypeForFileName(const QString &fileName) const
{
    return fileName.endsWith(".stp") ? Project::TilesetType
        : fileName.endsWith(".slp") ? Project::LayeredImageType : Project::ImageType;
}

Project::Type ProjectManager::projectTypeForUrl(const QUrl &url) const
{
    return projectTypeForFileName(url.toString());
}

QString ProjectManager::projectExtensionForType(Project::Type projectType) const
{
    switch (projectType) {
    case Project::ImageType: return QLatin1String("png");
    case Project::LayeredImageType: return QLatin1String("slp");
    case Project::TilesetType: return QLatin1String("stp");
    default: return QString();
    }
}

void ProjectManager::onCreationFailed(const QString &errorMessage)
{
    qCDebug(lcProjectManager) << "creation of" << mTemporaryProject->typeString() << "project failed;" << errorMessage;

    mProjectCreationFailed = true;
    mTemporaryProject.reset();
    emit temporaryProjectChanged();

    // In case this was a recent file that we were loading, remove it from our list.
    // The function only checks that the file exists (not that e.g. its contents couldn't be loaded),
    // but it will remove any other invalid files at the same time. It's good enough.
    mSettings->removeInvalidRecentFiles();

    emit creationFailed(errorMessage);
}

void ProjectManager::projectUrlChanged()
{
    if (mProject->hasLoaded() && !mProject->url().isEmpty()) {
        qCDebug(lcProjectManager) << "adding" << mProject->url().toString() << "to recent files list";
        mSettings->addRecentFile(mProject->url().toString());
    }
}
