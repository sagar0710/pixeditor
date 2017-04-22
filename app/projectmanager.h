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

class Project;
class Settings;

class ProjectManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project NOTIFY projectChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)

    Q_PROPERTY(Project *temporaryProject READ temporaryProject NOTIFY temporaryProjectChanged)

    Q_PROPERTY(Settings *applicationSettings READ applicationSettings
        WRITE setApplicationSettings NOTIFY applicationSettingsChanged)

public:
    explicit ProjectManager(QObject *parent = 0);
    ~ProjectManager();

    Project *project() const;

    Project *temporaryProject() const;

    QString type() const;

    Settings *applicationSettings() const;
    void setApplicationSettings(Settings *applicationSettings);

    Q_INVOKABLE void beginCreation(const QString &type);
    Q_INVOKABLE bool completeCreation();

signals:
    void projectChanged();
    void typeChanged();
    void temporaryProjectChanged();
    void applicationSettingsChanged();

private slots:
    void creationFailed(const QString &errorMessage);
    void projectUrlChanged();

private:
    Q_DISABLE_COPY(ProjectManager)

    QScopedPointer<Project> mProject;
    QString mType;

    QScopedPointer<Project> mTemporaryProject;
    QString mTemporaryProjectType;
    bool mProjectCreationFailed;

    Settings *mSettings;
};

#endif // PROJECTMANAGER_H
