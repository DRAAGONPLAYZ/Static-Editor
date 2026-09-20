#include "settings_window.h"

#include "logger.h"
#include "settings_manager.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

namespace
{
void populateThreadSelector(QComboBox* combo)
{
    combo->addItem(
        SettingsManager::threadLimitName(0),
        0
    );

    const int available = SettingsManager::availableCpuThreads();

    for (int threads = 1; threads <= available; ++threads)
    {
        combo->addItem(
            SettingsManager::threadLimitName(threads),
            threads
        );
    }
}
}

SettingsWindow::SettingsWindow(QWidget* parent)
    : QDialog(parent)
{
    Logger::log("[INFO] Opening Settings window");

    setWindowTitle("Static Editor Settings");
    setMinimumSize(620, 500);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    auto* tabs = new QTabWidget(this);

    auto* performanceTab = new QWidget(tabs);
    auto* performanceLayout = new QVBoxLayout(performanceTab);
    performanceLayout->setContentsMargins(18, 18, 18, 18);
    performanceLayout->setSpacing(18);

    auto* systemGroup = new QGroupBox("CPU", performanceTab);
    auto* systemLayout = new QFormLayout(systemGroup);

    const QString cpuModel = SettingsManager::cpuModelName();
    const int availableThreads = SettingsManager::availableCpuThreads();

    auto* cpuModelLabel = new QLabel(cpuModel, systemGroup);
    cpuModelLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    auto* cpuThreadsLabel = new QLabel(
        QString::number(availableThreads),
        systemGroup
    );

    systemLayout->addRow("CPU model:", cpuModelLabel);
    systemLayout->addRow("Available threads:", cpuThreadsLabel);

    auto* encodingGroup = new QGroupBox("Encoding", performanceTab);
    auto* encodingLayout = new QVBoxLayout(encodingGroup);

    m_hardwareAcceleration = new QCheckBox(
        "Use hardware-accelerated encoding",
        encodingGroup
    );

    auto* hardwareDescription = new QLabel(
        "When enabled, Static Editor will prefer a GPU video encoder during export. "
        "When disabled, exports use CPU encoding (x264).",
        encodingGroup
    );
    hardwareDescription->setWordWrap(true);
    hardwareDescription->setStyleSheet("color: #B8BCE8;");

    encodingLayout->addWidget(m_hardwareAcceleration);
    encodingLayout->addWidget(hardwareDescription);

    auto* previewGroup = new QGroupBox("Timeline Preview", performanceTab);
    auto* previewLayout = new QFormLayout(previewGroup);

    m_previewQuality = new QComboBox(previewGroup);
    m_previewQuality->addItem(
        SettingsManager::previewQualityName(SettingsManager::PreviewQuality::Low),
        static_cast<int>(SettingsManager::PreviewQuality::Low)
    );
    m_previewQuality->addItem(
        SettingsManager::previewQualityName(SettingsManager::PreviewQuality::Medium),
        static_cast<int>(SettingsManager::PreviewQuality::Medium)
    );
    m_previewQuality->addItem(
        SettingsManager::previewQualityName(SettingsManager::PreviewQuality::High),
        static_cast<int>(SettingsManager::PreviewQuality::High)
    );

    m_editingThreadLimit = new QComboBox(previewGroup);
    populateThreadSelector(m_editingThreadLimit);

    previewLayout->addRow("Preview quality:", m_previewQuality);
    previewLayout->addRow("CPU threads:", m_editingThreadLimit);

    auto* previewDescription = new QLabel(
        "Controls preview quality and the CPU thread limit used for editing and playback. "
        "Auto uses 50% of available logical CPU threads.",
        previewGroup
    );
    previewDescription->setWordWrap(true);
    previewDescription->setStyleSheet("color: #B8BCE8;");
    previewLayout->addRow("", previewDescription);

    auto* cacheGroup = new QGroupBox("Export", performanceTab);
    auto* cacheLayout = new QFormLayout(cacheGroup);

    m_exportCacheLocation = new QComboBox(cacheGroup);
    m_exportCacheLocation->addItem(
        SettingsManager::exportCacheLocationName(
            SettingsManager::ExportCacheLocation::RAM
        ),
        static_cast<int>(SettingsManager::ExportCacheLocation::RAM)
    );
    m_exportCacheLocation->addItem(
        SettingsManager::exportCacheLocationName(
            SettingsManager::ExportCacheLocation::Disk
        ),
        static_cast<int>(SettingsManager::ExportCacheLocation::Disk)
    );

    m_exportThreadLimit = new QComboBox(cacheGroup);
    populateThreadSelector(m_exportThreadLimit);

    cacheLayout->addRow("Cache location:", m_exportCacheLocation);
    cacheLayout->addRow("CPU threads:", m_exportThreadLimit);

    auto* cacheDescription = new QLabel(
        "RAM is faster but uses system memory. Disk uses storage space instead "
        "and is better for longer exports or systems with limited RAM. "
        "Auto uses 50% of available logical CPU threads for export.",
        cacheGroup
    );
    cacheDescription->setWordWrap(true);
    cacheDescription->setStyleSheet("color: #B8BCE8;");
    cacheLayout->addRow("", cacheDescription);

    performanceLayout->addWidget(systemGroup);
    performanceLayout->addWidget(encodingGroup);
    performanceLayout->addWidget(previewGroup);
    performanceLayout->addWidget(cacheGroup);
    performanceLayout->addStretch();

    tabs->addTab(performanceTab, "Performance");
    mainLayout->addWidget(tabs);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel,
        this
    );

    connect(buttons, &QDialogButtonBox::accepted, this, [this] {
        saveSettings();
        accept();
    });

    connect(buttons, &QDialogButtonBox::rejected, this, [this] {
        Logger::log("[INFO] Settings changes cancelled");
        reject();
    });

    mainLayout->addWidget(buttons);

    loadSettings();
    Logger::log("[OK] Settings window ready");
}

void SettingsWindow::loadSettings()
{
    m_hardwareAcceleration->setChecked(
        SettingsManager::hardwareAccelerationEnabled()
    );

    const int previewIndex = m_previewQuality->findData(
        static_cast<int>(SettingsManager::previewQuality())
    );

    if (previewIndex >= 0)
    {
        m_previewQuality->setCurrentIndex(previewIndex);
    }

    const int cacheIndex = m_exportCacheLocation->findData(
        static_cast<int>(SettingsManager::exportCacheLocation())
    );

    if (cacheIndex >= 0)
    {
        m_exportCacheLocation->setCurrentIndex(cacheIndex);
    }

    const int editingThreadIndex = m_editingThreadLimit->findData(
        SettingsManager::editingThreadLimit()
    );

    if (editingThreadIndex >= 0)
    {
        m_editingThreadLimit->setCurrentIndex(editingThreadIndex);
    }

    const int exportThreadIndex = m_exportThreadLimit->findData(
        SettingsManager::exportThreadLimit()
    );

    if (exportThreadIndex >= 0)
    {
        m_exportThreadLimit->setCurrentIndex(exportThreadIndex);
    }

    Logger::log("[INFO] Performance settings loaded");
}

void SettingsWindow::saveSettings()
{
    SettingsManager::setHardwareAccelerationEnabled(
        m_hardwareAcceleration->isChecked()
    );

    SettingsManager::setPreviewQuality(
        static_cast<SettingsManager::PreviewQuality>(
            m_previewQuality->currentData().toInt()
        )
    );

    SettingsManager::setExportCacheLocation(
        static_cast<SettingsManager::ExportCacheLocation>(
            m_exportCacheLocation->currentData().toInt()
        )
    );

    SettingsManager::setEditingThreadLimit(
        m_editingThreadLimit->currentData().toInt()
    );

    SettingsManager::setExportThreadLimit(
        m_exportThreadLimit->currentData().toInt()
    );

    Logger::log("[OK] Performance settings saved");
}
