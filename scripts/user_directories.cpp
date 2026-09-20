#include "user_directories.h"

#include <QDir>
#include <QStandardPaths>

QString UserDirectories::root()
{
    const QString userDataDirectory =
        QStandardPaths::writableLocation(
            QStandardPaths::GenericDataLocation
        );

    return userDataDirectory + "/StaticDevTeam/StaticEditor";
}

QString UserDirectories::logs()
{
    const QString logsDirectory = root() + "/Logs";

    QDir().mkpath(logsDirectory);

    return logsDirectory;
}
