/*
    Copyright 2016, Mitch Curtis

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

#include "project.h"

#include <QDateTime>
#include <QImage>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QMetaEnum>

#include "applicationsettings.h"

Q_LOGGING_CATEGORY(lcProject, "app.project")
Q_LOGGING_CATEGORY(lcProjectLifecycle, "app.project.lifecycle")

Project::Project() :
    mSettings(nullptr),
    mFromNew(false),
    mUsingTempImage(false),
    mComposingMacro(false),
    mHadUnsavedChangesBeforeMacroBegan(false)
{
    connect(&mUndoStack, SIGNAL(cleanChanged(bool)), this, SIGNAL(unsavedChangesChanged()));
}

Project::Type Project::type() const
{
    return UnknownType;
}

QString Project::typeString() const
{
    return typeToString(type());
}

QString Project::typeToString(Project::Type type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Project::Type>();
    return metaEnum.valueToKey(type);
}

QUrl Project::url() const
{
    return mUrl;
}

void Project::setUrl(const QUrl &url)
{
    if (url == mUrl)
        return;

    const bool wasLoaded = hasLoaded();

    mUrl = url;

    if (wasLoaded != hasLoaded()) {
        emit loadedChanged();
    }

    emit urlChanged();
}

QString Project::displayUrl() const
{
    return mUrl.toDisplayString(QUrl::PreferLocalFile);
}

QUrl Project::dirUrl() const
{
    QDir dir(mUrl.toLocalFile());
    if (!dir.cdUp())
        return QUrl();

    return QUrl::fromLocalFile(dir.path());
}

QString Project::fileBaseName() const
{
    return QFileInfo(mUrl.toLocalFile()).baseName();
}

bool Project::hasLoaded() const
{
    return mFromNew || mUrl.isValid();
}

bool Project::isNewProject() const
{
    return mFromNew;
}

void Project::setNewProject(bool newProject)
{
    if (newProject == mFromNew)
        return;

    const bool wasLoaded = hasLoaded();
    const bool couldSave = canSave();

    mFromNew = newProject;

    if (wasLoaded != hasLoaded()) {
        emit loadedChanged();
    }

    if (couldSave != canSave()) {
        emit canSaveChanged();
    }

    emit newProjectChanged();
}

bool Project::hasUnsavedChanges() const
{
    return !mUndoStack.isClean();
}

bool Project::canSave() const
{
    return !mComposingMacro && (isNewProject() || hasUnsavedChanges());
}

void Project::load(const QUrl &)
{
}

void Project::close()
{
}

void Project::save()
{
    if (mFromNew) {
        Q_ASSERT_X(mUrl.isEmpty(), Q_FUNC_INFO, "New projects must have a valid URL to save to");
    }

    saveAs(mUrl);
}

void Project::saveAs(const QUrl &url)
{
    emit preProjectSaved();

    doSaveAs(url);
}

void Project::revert()
{
    qCDebug(lcProject) << "reverting changes...";

    if (!hasLoaded())
        return;

    clearChanges();

    qCDebug(lcProject) << "... reverted changes";
}

void Project::error(const QString &message)
{
    emit errorOccurred(message);
}

void Project::doSaveAs(const QUrl &)
{
}

void Project::setComposingMacro(bool composingMacro)
{
    if (composingMacro == mComposingMacro)
        return;

    const bool couldSave = canSave();
    mComposingMacro = composingMacro;
    if (couldSave != canSave()) {
        emit canSaveChanged();
    }
}

QUrl Project::createTemporaryImage(int width, int height, const QColor &colour)
{
    if (!mTempDir.isValid()) {
        error(QString::fromLatin1("Failed to create temporary image directory: %1").arg(mTempDir.errorString()));
        return QUrl();
    }

    QImage tempImage(width, height, QImage::Format_ARGB32_Premultiplied);
    tempImage.fill(colour);

    const QString dateString = QDateTime::currentDateTime().toString(QLatin1String("hh-mm-ss-zzz"));
    const QString fileName = QString::fromLatin1("%1/tileset-%2.png").arg(mTempDir.path()).arg(dateString);
    if (!tempImage.save(fileName)) {
        error(QString::fromLatin1("Failed to save temporary image to %1").arg(fileName));
        return QUrl();
    }

    qCDebug(lcProject) << "Successfully created temporary image:" << fileName;
    mUsingTempImage = true;
    return QUrl::fromLocalFile(fileName);
}

void Project::readGuides(const QJsonObject &projectJson)
{
    mGuides.clear();

    QJsonArray guidesArray = projectJson.value(QLatin1String("guides")).toArray();
    for (int i = 0; i < guidesArray.size(); ++i) {
        QJsonObject guideObject = guidesArray.at(i).toObject();
        const int position = guideObject.value(QLatin1String("position")).toInt();
        const Qt::Orientation orientation = static_cast<Qt::Orientation>(
            guideObject.value(QLatin1String("orientation")).toInt());
        mGuides.append(Guide(position, orientation));
    }
}

void Project::writeGuides(QJsonObject &projectJson) const
{
    QJsonArray guidesArray;
    for (int i = 0; i < mGuides.size(); ++i) {
        QJsonObject guideObject;
        const Guide guide = mGuides.at(i);
        guideObject[QLatin1String("position")] = guide.position();
        guideObject[QLatin1String("orientation")] = guide.orientation();
        guidesArray.append(guideObject);
    }
    projectJson[QLatin1String("guides")] = guidesArray;
}

ApplicationSettings *Project::settings() const
{
    return mSettings;
}

void Project::setSettings(ApplicationSettings *settings)
{
    if (settings == mSettings)
        return;

    mSettings = settings;
    emit settingsChanged();
}

QJsonObject *Project::cachedProjectJson()
{
    return &mCachedProjectJson;
}

QSize Project::size() const
{
    return QSize(0, 0);
}

void Project::setSize(const QSize &)
{
}

QRect Project::bounds() const
{
    return QRect();
}

int Project::widthInPixels() const
{
    return 0;
}

int Project::heightInPixels() const
{
    return 0;
}

QVector<Guide> Project::guides() const
{
    return mGuides;
}

void Project::addGuide(const Guide &guide)
{
    if (mGuides.contains(guide))
        return;

    mGuides.append(guide);

    emit guidesChanged();
}

void Project::moveGuide(const Guide &guide, int to)
{
    auto it = std::find(mGuides.begin(), mGuides.end(), guide);
    if (it == mGuides.end())
        return;

    it->setPosition(to);

    emit guidesChanged();
}

void Project::removeGuide(const Guide &guide)
{
    if (mGuides.removeOne(guide)) {
        emit guidesChanged();
    }
}

int Project::currentLayerIndex() const
{
    return -1;
}

QImage Project::exportedImage() const
{
    return QImage();
}

QUndoStack *Project::undoStack()
{
    return &mUndoStack;
}

bool Project::isComposingMacro() const
{
    return mComposingMacro;
}

void Project::beginMacro(const QString &text)
{
    if (isComposingMacro()) {
        qCDebug(lcProject).nospace() << "tried to begin macro" << text
            << ", but we're busy composing another right now";
        return;
    }

    qCDebug(lcProject) << "beginning macro" << text;

    mHadUnsavedChangesBeforeMacroBegan = hasUnsavedChanges();
    mUndoStack.beginMacro(text);
    setComposingMacro(true);
}

void Project::endMacro()
{
    Q_ASSERT(mComposingMacro);

    qCDebug(lcProject) << "ending macro";

    mUndoStack.endMacro();
    // This handles the emission of the canSaveChanged signal.
    setComposingMacro(false);

    // It's not enough to rely on the cleanChanged signal to cause
    // our unchangedChangesSignal to be called, because cleanChanged
    // apparently does not get emitted when a macro ends. So, we do it ourselves here.
    if (hasUnsavedChanges() != mHadUnsavedChangesBeforeMacroBegan) {
        emit unsavedChangesChanged();
    }
}

QDebug operator<<(QDebug debug, const QUndoCommand &command)
{
    debug << &command;
    return debug;
}

void Project::addChange(QUndoCommand *undoCommand)
{
    qCDebug(lcProject) << "adding change" << *undoCommand;
    mUndoStack.push(undoCommand);
}

void Project::clearChanges()
{
    const bool hadUnsavedChanges = hasUnsavedChanges();

    mUndoStack.setIndex(mUndoStack.cleanIndex());
    mHadUnsavedChangesBeforeMacroBegan = false;

    if (hasUnsavedChanges() != hadUnsavedChanges) {
        emit unsavedChangesChanged();
    }
}
