#include "logger.h"

#include "user_directories.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

namespace
{
constexpr int MaxLogFiles = 5;
}

bool Logger::initialize()
{
    const QString directory = UserDirectories::logs();

    if (directory.isEmpty())
    {
        return false;
    }

    QDir logDirectory(directory);

    const QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");

    const QString filePath =
        logDirectory.filePath(QString("StaticEditor-%1.log").arg(timestamp));

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream stream(&file);
    stream << "[START] Static Editor" << Qt::endl;

    const QFileInfoList files = logDirectory.entryInfoList(
        {"StaticEditor-*.log"},
        QDir::Files,
        QDir::Time | QDir::Reversed
    );

    for (int index = MaxLogFiles; index < files.size(); ++index)
    {
        QFile::remove(files.at(index).absoluteFilePath());
    }

    return true;
}

void Logger::write(const QString& message)
{
    const QString directory = UserDirectories::logs();
    QDir logDirectory(directory);

    const QFileInfoList files = logDirectory.entryInfoList(
        {"StaticEditor-*.log"},
        QDir::Files,
        QDir::Time
    );

    if (files.isEmpty())
    {
        return;
    }

    QFile file(files.first().absoluteFilePath());

    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);
    stream << message << Qt::endl;
}

void Logger::shutdown()
{
    write("[END] Static Editor");
}
