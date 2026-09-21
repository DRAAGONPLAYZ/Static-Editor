#include "editor_screen.h"

#include "logger.h"
#include "settings_window.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
QPushButton* createTopButton(const QString& text, QWidget* parent)
{
    auto* button = new QPushButton(text, parent);
    button->setCursor(Qt::PointingHandCursor);
    button->setMinimumHeight(38);
    button->setStyleSheet(
        "QPushButton {"
        " background-color: transparent;"
        " color: #BFC8FF;"
        " border: none;"
        " border-radius: 6px;"
        " padding: 6px 14px;"
        " font-size: 14px;"
        " font-weight: 600;"
        "}"
        "QPushButton:hover {"
        " background-color: #18204A;"
        " color: #F2F4FF;"
        "}"
        "QPushButton:pressed {"
        " background-color: #30266B;"
        " color: #FFFFFF;"
        "}"
    );
    return button;
}

QWidget* createEmptyMediaPanel(const QString& title, const QString& description)
{
    auto* panel = new QWidget;
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(8);

    auto* titleLabel = new QLabel(title, panel);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "color: #F2F4FF; font-size: 16px; font-weight: 600;"
    );

    auto* descriptionLabel = new QLabel(description, panel);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("color: #7F89B8; font-size: 13px;");
    descriptionLabel->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Preferred
    );

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addStretch();

    return panel;
}
}

EditorScreen::EditorScreen(QWidget* parent)
    : QWidget(parent)
{
    Logger::log("[INFO] Creating mock editor screen...");
    initializeTemporaryProject();
    buildInterface();
    Logger::log("[OK] Mock editor screen ready");
}

void EditorScreen::initializeTemporaryProject()
{
    // The mock project is represented by serialized .sev data held entirely
    // in memory. Nothing is written to the user's filesystem yet.
    m_temporarySevData = QByteArray(
        "SEV\\0"
        "version=1\\n"
        "project=Temporary Project\\n"
        "state=mock\\n"
    );

    Logger::log(
        QString("[INFO] Temporary .sev project initialized in RAM (%1 bytes)")
            .arg(m_temporarySevData.size())
    );
}

void EditorScreen::buildInterface()
{
    setMinimumSize(1000, 650);

    setStyleSheet(
        "QWidget {"
        " background-color: #070A14;"
        " color: #F2F4FF;"
        "}"
        "QTabWidget::pane {"
        " background-color: #0B1022;"
        " border: 1px solid #26356F;"
        "}"
        "QTabBar::tab {"
        " background-color: #0D132B;"
        " color: #AEB8E8;"
        " border: 1px solid #26356F;"
        " padding: 9px 18px;"
        " min-width: 70px;"
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
    );

    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* topBar = new QWidget(this);
    topBar->setStyleSheet(
        "QWidget {"
        " background-color: #0D1328;"
        " border-bottom: 1px solid #26356F;"
        "}"
    );

    auto* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(12, 6, 12, 6);
    topLayout->setSpacing(4);

    auto* fileButton = createTopButton("File", topBar);
    auto* settingsButton = createTopButton("Settings", topBar);
    auto* helpButton = createTopButton("Help", topBar);

    topLayout->addWidget(fileButton);
    topLayout->addWidget(settingsButton);
    topLayout->addWidget(helpButton);
    topLayout->addStretch();

    auto* projectLabel = new QLabel("Temporary Project", topBar);
    projectLabel->setStyleSheet(
        "color: #7F89B8; font-size: 13px; padding-right: 8px;"
    );
    topLayout->addWidget(projectLabel);

    connect(fileButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] File menu button clicked (mock)");
    });

    connect(helpButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] Help button clicked (mock)");
    });

    connect(settingsButton, &QPushButton::clicked, this, [this] {
        Logger::log("[INFO] Editor Settings button clicked");

        SettingsWindow settingsWindow(this);
        settingsWindow.exec();

        Logger::log("[INFO] Editor Settings window closed");
    });

    rootLayout->addWidget(topBar);

    auto* workspace = new QWidget(this);
    auto* workspaceLayout = new QHBoxLayout(workspace);
    workspaceLayout->setContentsMargins(12, 12, 12, 12);
    workspaceLayout->setSpacing(12);

    auto* mediaPanel = new QWidget(workspace);
    mediaPanel->setMinimumWidth(250);
    mediaPanel->setMaximumWidth(360);
    mediaPanel->setStyleSheet(
        "QWidget {"
        " background-color: #0B1022;"
        " border: 1px solid #26356F;"
        " border-radius: 8px;"
        "}"
    );

    auto* mediaLayout = new QVBoxLayout(mediaPanel);
    mediaLayout->setContentsMargins(10, 10, 10, 10);
    mediaLayout->setSpacing(10);

    auto* mediaTitle = new QLabel("Project Files", mediaPanel);
    mediaTitle->setStyleSheet(
        "color: #BFC8FF; font-size: 16px; font-weight: 600; "
        "padding: 4px 6px;"
    );
    mediaLayout->addWidget(mediaTitle);

    m_mediaTabs = new QTabWidget(mediaPanel);
    m_mediaTabs->setDocumentMode(true);
    m_mediaTabs->addTab(
        createEmptyMediaPanel(
            "No video files",
            "Imported video clips will appear here."
        ),
        "Video"
    );
    m_mediaTabs->addTab(
        createEmptyMediaPanel(
            "No audio files",
            "Imported audio clips will appear here."
        ),
        "Audio"
    );
    m_mediaTabs->addTab(
        createEmptyMediaPanel(
            "No images",
            "Imported images will appear here."
        ),
        "Images"
    );
    mediaLayout->addWidget(m_mediaTabs, 1);

    auto* importButton = new QPushButton("Import", mediaPanel);
    importButton->setMinimumHeight(40);
    importButton->setStyleSheet(
        "QPushButton {"
        " background-color: #4C45B8;"
        " color: #F2F4FF;"
        " border: 1px solid #7A6CFF;"
        " border-radius: 6px;"
        " padding: 8px 18px;"
        " font-weight: 600;"
        "}"
        "QPushButton:hover {"
        " background-color: #5A52CF;"
        "}"
        "QPushButton:pressed {"
        " background-color: #30266B;"
        "}"
    );

    connect(importButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] Import button clicked (mock)");
    });

    mediaLayout->addWidget(importButton);

    workspaceLayout->addWidget(mediaPanel);

    auto* rightWorkspace = new QWidget(workspace);
    auto* rightLayout = new QVBoxLayout(rightWorkspace);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    auto* preview = new QWidget(rightWorkspace);
    preview->setStyleSheet(
        "QWidget {"
        " background-color: #050812;"
        " border: 1px solid #26356F;"
        " border-radius: 8px;"
        "}"
    );

    auto* previewLayout = new QVBoxLayout(preview);
    auto* previewLabel = new QLabel("Preview", preview);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet(
        "color: #7F89B8; font-size: 18px; font-weight: 600;"
    );
    previewLayout->addWidget(previewLabel);

    auto* timeline = new QWidget(rightWorkspace);
    timeline->setMinimumHeight(150);
    timeline->setStyleSheet(
        "QWidget {"
        " background-color: #0B1022;"
        " border: 1px solid #26356F;"
        " border-radius: 8px;"
        "}"
    );

    auto* timelineLayout = new QVBoxLayout(timeline);
    auto* timelineLabel = new QLabel("Timeline", timeline);
    timelineLabel->setStyleSheet(
        "color: #BFC8FF; font-size: 15px; font-weight: 600;"
    );
    timelineLayout->addWidget(timelineLabel);

    auto* timelinePlaceholder = new QLabel(
        "Tracks and clips will appear here.",
        timeline
    );
    timelinePlaceholder->setAlignment(Qt::AlignCenter);
    timelinePlaceholder->setStyleSheet("color: #59658F; font-size: 13px;");
    timelineLayout->addWidget(timelinePlaceholder, 1);

    rightLayout->addWidget(preview, 1);
    rightLayout->addWidget(timeline);

    workspaceLayout->addWidget(rightWorkspace, 1);

    rootLayout->addWidget(workspace, 1);

    auto* statusBar = new QLabel(
        "MOCK EDITOR  •  Project state stored in RAM  •  No files imported",
        this
    );
    statusBar->setMinimumHeight(28);
    statusBar->setAlignment(Qt::AlignCenter);
    statusBar->setStyleSheet(
        "background-color: #0D1328;"
        "color: #59658F;"
        "border-top: 1px solid #26356F;"
        "font-size: 11px;"
    );

    rootLayout->addWidget(statusBar);
}
