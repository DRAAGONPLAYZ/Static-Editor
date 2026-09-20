#include "settings_manager.h"

#include <QSettings>

namespace
{
constexpr const char* HardwareAccelerationKey = "performance/hardwareAcceleration";
constexpr const char* PreviewQualityKey = "performance/previewQuality";
constexpr const char* ExportCacheLocationKey = "performance/exportCacheLocation";
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
