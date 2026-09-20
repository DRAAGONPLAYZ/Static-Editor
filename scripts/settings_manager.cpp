#include "settings_manager.h"

#include <QSettings>

#include <thread>

#ifdef Q_OS_LINUX
#include <QFile>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace
{
constexpr const char* HardwareAccelerationKey = "performance/hardwareAcceleration";
constexpr const char* PreviewQualityKey = "performance/previewQuality";
constexpr const char* ExportCacheLocationKey = "performance/exportCacheLocation";
constexpr const char* ExportThreadLimitKey = "performance/exportThreadLimit";
constexpr const char* EditingThreadLimitKey = "performance/editingThreadLimit";
constexpr int AutoThreadLimit = 0;
}

bool SettingsManager::hardwareAccelerationEnabled()
{
    QSettings settings;
    return settings.value(HardwareAccelerationKey, false).toBool();
}

void SettingsManager::setHardwareAccelerationEnabled(bool enabled)
{
    QSettings settings;
    settings.setValue(HardwareAccelerationKey, enabled);
}

SettingsManager::PreviewQuality SettingsManager::previewQuality()
{
    QSettings settings;
    const int value = settings.value(
        PreviewQualityKey,
        static_cast<int>(PreviewQuality::Medium)
    ).toInt();

    switch (value)
    {
    case static_cast<int>(PreviewQuality::Low):
        return PreviewQuality::Low;

    case static_cast<int>(PreviewQuality::High):
        return PreviewQuality::High;

    default:
        return PreviewQuality::Medium;
    }
}

void SettingsManager::setPreviewQuality(PreviewQuality quality)
{
    QSettings settings;
    settings.setValue(
        PreviewQualityKey,
        static_cast<int>(quality)
    );
}

SettingsManager::ExportCacheLocation SettingsManager::exportCacheLocation()
{
    QSettings settings;
    const int value = settings.value(
        ExportCacheLocationKey,
        static_cast<int>(ExportCacheLocation::Disk)
    ).toInt();

    if (value == static_cast<int>(ExportCacheLocation::RAM))
    {
        return ExportCacheLocation::RAM;
    }

    return ExportCacheLocation::Disk;
}

void SettingsManager::setExportCacheLocation(ExportCacheLocation location)
{
    QSettings settings;
    settings.setValue(
        ExportCacheLocationKey,
        static_cast<int>(location)
    );
}

int SettingsManager::exportThreadLimit()
{
    QSettings settings;
    return settings.value(ExportThreadLimitKey, AutoThreadLimit).toInt();
}

void SettingsManager::setExportThreadLimit(int threads)
{
    QSettings settings;
    settings.setValue(
        ExportThreadLimitKey,
        qBound(0, threads, availableCpuThreads())
    );
}

int SettingsManager::editingThreadLimit()
{
    QSettings settings;
    return settings.value(EditingThreadLimitKey, AutoThreadLimit).toInt();
}

void SettingsManager::setEditingThreadLimit(int threads)
{
    QSettings settings;
    settings.setValue(
        EditingThreadLimitKey,
        qBound(0, threads, availableCpuThreads())
    );
}

int SettingsManager::availableCpuThreads()
{
    const unsigned int threads = std::thread::hardware_concurrency();
    return threads == 0 ? 1 : static_cast<int>(threads);
}

QString SettingsManager::cpuModelName()
{
#ifdef Q_OS_LINUX
    QFile file("/proc/cpuinfo");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            const QString line = QString::fromUtf8(file.readLine()).trimmed();

            if (line.startsWith("model name"))
            {
                const int separator = line.indexOf(':');

                if (separator >= 0)
                {
                    return line.mid(separator + 1).trimmed();
                }
            }
        }
    }
#elif defined(Q_OS_WIN)
    HKEY key = nullptr;

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0,
            KEY_READ,
            &key
        ) == ERROR_SUCCESS)
    {
        char buffer[256]{};
        DWORD bufferSize = sizeof(buffer);
        DWORD type = 0;

        if (RegQueryValueExA(
                key,
                "ProcessorNameString",
                nullptr,
                &type,
                reinterpret_cast<LPBYTE>(buffer),
                &bufferSize
            ) == ERROR_SUCCESS)
        {
            RegCloseKey(key);
            return QString::fromLocal8Bit(buffer).trimmed();
        }

        RegCloseKey(key);
    }
#endif

    return "Unknown CPU";
}

QString SettingsManager::threadLimitName(int threads)
{
    if (threads == AutoThreadLimit)
    {
        const int available = availableCpuThreads();
        const int automatic = qMax(1, (available + 1) / 2);
        return QString("Auto (50%%) — %1 / %2 threads")
            .arg(automatic)
            .arg(available);
    }

    return QString("%1 / %2 thread%3")
        .arg(threads)
        .arg(availableCpuThreads())
        .arg(availableCpuThreads() == 1 ? "" : "s");
}

QString SettingsManager::previewQualityName(PreviewQuality quality)
{
    switch (quality)
    {
    case PreviewQuality::Low:
        return "Low";

    case PreviewQuality::High:
        return "High";

    default:
        return "Medium";
    }
}

QString SettingsManager::exportCacheLocationName(ExportCacheLocation location)
{
    switch (location)
    {
    case ExportCacheLocation::RAM:
        return "RAM";

    default:
        return "Disk";
    }
}
