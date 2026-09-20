#pragma once

#include <QString>

class SettingsManager
{
public:
    enum class PreviewQuality
    {
        Low,
        Medium,
        High
    };

    enum class ExportCacheLocation
    {
        RAM,
        Disk
    };

    static bool hardwareAccelerationEnabled();
    static void setHardwareAccelerationEnabled(bool enabled);

    static PreviewQuality previewQuality();
    static void setPreviewQuality(PreviewQuality quality);

    static ExportCacheLocation exportCacheLocation();
    static void setExportCacheLocation(ExportCacheLocation location);

    static int exportThreadLimit();
    static void setExportThreadLimit(int threads);

    static int editingThreadLimit();
    static void setEditingThreadLimit(int threads);

    static int availableCpuThreads();
    static QString cpuModelName();

    static QString threadLimitName(int threads);

    static QString previewQualityName(PreviewQuality quality);
    static QString exportCacheLocationName(ExportCacheLocation location);
};