/*
    Copyright 2017, Mitch Curtis

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

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "project.h"

class ApplicationSettings;

class ProjectManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project NOTIFY projectChanged)

    Q_PROPERTY(Project *temporaryProject READ temporaryProject NOTIFY temporaryProjectChanged)

    Q_PROPERTY(ApplicationSettings *applicationSettings READ applicationSettings
        WRITE setApplicationSettings NOTIFY applicationSettingsChanged)

public:
    explicit ProjectManager(QObject *parent = 0);
    ~ProjectManager();

    Project *project() const;

    Project *temporaryProject() const;

    ApplicationSettings *applicationSettings() const;
    void setApplicationSettings(ApplicationSettings *applicationSettings);

    Q_INVOKABLE void beginCreation(Project::Type projectType);
    Q_INVOKABLE bool completeCreation();

    Q_INVOKABLE Project::Type projectTypeForUrl(const QUrl &url) const;

signals:
    void projectChanged();
    void temporaryProjectChanged();
    void applicationSettingsChanged();
    void creationFailed(const QString &errorMessage);

private slots:
    void onCreationFailed(const QString &errorMessage);
    void projectUrlChanged();

private:
    Q_DISABLE_COPY(ProjectManager)

    QScopedPointer<Project> mProject;

    QScopedPointer<Project> mTemporaryProject;
    bool mProjectCreationFailed;

    ApplicationSettings *mSettings;
};

#endif // PROJECTMANAGER_H
