#include "settings_window.h"

#include "logger.h"
#include "settings_manager.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSizePolicy>
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
    setMinimumSize(760, 620);
    resize(820, 680);

    setStyleSheet(
        "QDialog {"
        " background-color: #070A14;"
        " color: #F2F4FF;"
        "}"
        "QWidget {"
        " color: #F2F4FF;"
        "}"
        "QTabWidget::pane {"
        " background-color: #0B1022;"
        " border: 1px solid #26356F;"
        " border-radius: 8px;"
        " top: -1px;"
        "}"
        "QTabBar::tab {"
        " background-color: #0D132B;"
        " color: #AEB8E8;"
        " border: 1px solid #26356F;"
        " border-bottom: none;"
        " padding: 10px 22px;"
        " min-width: 110px;"
        "}"
        "QTabBar::tab:selected {"
        " background-color: #161B3A;"
        " color: #F2F4FF;"
        " border-top: 2px solid #7A6CFF;"
        "}"
        "QTabBar::tab:hover {"
        " background-color: #18204A;"
        " color: #FFFFFF;"
        "}"
        "QGroupBox {"
        " background-color: #0D1328;"
        " border: 1px solid #28386F;"
        " border-radius: 8px;"
        " margin-top: 14px;"
        " padding: 18px 12px 12px 12px;"
        " font-weight: 600;"
        " color: #F2F4FF;"
        "}"
        "QGroupBox::title {"
        " subcontrol-origin: margin;"
        " left: 14px;"
        " padding: 0 7px;"
        " color: #BFC8FF;"
        " background-color: #0D1328;"
        "}"
        "QLabel {"
        " color: #F2F4FF;"
        "}"
        "QCheckBox {"
        " color: #F2F4FF;"
        " spacing: 9px;"
        "}"
        "QCheckBox::indicator {"
        " width: 18px;"
        " height: 18px;"
        " border: 1px solid #5265B8;"
        " border-radius: 4px;"
        " background-color: #080D1D;"
        "}"
        "QCheckBox::indicator:hover {"
        " border: 1px solid #7A6CFF;"
        "}"
        "QCheckBox::indicator:checked {"
        " background-color: #4C45B8;"
        " border: 1px solid #8A7CFF;"
        "}"
        "QComboBox {"
        " background-color: #080D1D;"
        " color: #F2F4FF;"
        " border: 1px solid #344789;"
        " border-radius: 6px;"
        " padding: 7px 10px;"
        " min-height: 28px;"
        "}"
        "QComboBox:hover {"
        " border: 1px solid #586DCC;"
        "}"
        "QComboBox:focus {"
        " border: 1px solid #8A7CFF;"
        "}"
        "QComboBox::drop-down {"
        " width: 30px;"
        " border-left: 1px solid #344789;"
        " background-color: #111938;"
        "}"
        "QComboBox QAbstractItemView {"
        " background-color: #0B1022;"
        " color: #F2F4FF;"
        " selection-background-color: #403A91;"
        " selection-color: #FFFFFF;"
        " border: 1px solid #344789;"
        "}"
        "QPushButton {"
        " background-color: #101735;"
        " color: #F2F4FF;"
        " border: 1px solid #3546B8;"
        " border-radius: 6px;"
        " padding: 9px 20px;"
        " min-width: 90px;"
        " font-weight: 600;"
        "}"
        "QPushButton:hover {"
        " background-color: #18245A;"
        " border: 1px solid #6378E8;"
        "}"
        "QPushButton:pressed {"
        " background-color: #30266B;"
        " border: 1px solid #9B6CFF;"
        "}"
        "QPushButton:focus {"
        " border: 1px solid #8A7CFF;"
        "}"
        "QDialogButtonBox {"
        " background-color: transparent;"
        "}"
    );

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
    systemLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    systemLayout->setHorizontalSpacing(18);
    systemLayout->setVerticalSpacing(10);

    const QString cpuModel = SettingsManager::cpuModelName();
    const int availableThreads = SettingsManager::availableCpuThreads();

    auto* cpuModelLabel = new QLabel(cpuModel, systemGroup);
    cpuModelLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    cpuModelLabel->setWordWrap(true);
    cpuModelLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    auto* cpuThreadsLabel = new QLabel(
        QString::number(availableThreads),
        systemGroup
    );

    systemLayout->addRow("CPU model:", cpuModelLabel);
    systemLayout->addRow("Available threads:", cpuThreadsLabel);

    auto* encodingGroup = new QGroupBox("Encoding", performanceTab);
    auto* encodingLayout = new QVBoxLayout(encodingGroup);
    encodingLayout->setSpacing(8);

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
    hardwareDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hardwareDescription->setStyleSheet("color: #AEB8E8;");

    encodingLayout->addWidget(m_hardwareAcceleration);
    encodingLayout->addWidget(hardwareDescription);

    auto* previewGroup = new QGroupBox("Timeline Preview", performanceTab);
    auto* previewLayout = new QFormLayout(previewGroup);
    previewLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    previewLayout->setHorizontalSpacing(18);
    previewLayout->setVerticalSpacing(10);

    m_previewQuality = new QComboBox(previewGroup);
    m_previewQuality->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
    m_editingThreadLimit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    populateThreadSelector(m_editingThreadLimit);

    previewLayout->addRow("Preview quality:", m_previewQuality);
    previewLayout->addRow("CPU threads:", m_editingThreadLimit);

    auto* previewDescription = new QLabel(
        "Controls preview quality and the CPU thread limit used for editing and playback. "
        "Auto uses 50% of available logical CPU threads.",
        previewGroup
    );
    previewDescription->setWordWrap(true);
    previewDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    previewDescription->setStyleSheet("color: #AEB8E8;");
    previewLayout->addRow("", previewDescription);

    auto* cacheGroup = new QGroupBox("Export", performanceTab);
    auto* cacheLayout = new QFormLayout(cacheGroup);
    cacheLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    cacheLayout->setHorizontalSpacing(18);
    cacheLayout->setVerticalSpacing(10);

    m_exportCacheLocation = new QComboBox(cacheGroup);
    m_exportCacheLocation->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
    m_exportThreadLimit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
    cacheDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    cacheDescription->setStyleSheet("color: #AEB8E8;");
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
