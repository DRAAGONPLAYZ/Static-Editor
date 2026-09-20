#include "opening_screen.h"

#include "settings_window.h"

#include "logger.h"

#include <QApplication>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

OpeningScreen::OpeningScreen(QWidget* parent)
    : QWidget(parent),
      m_wallpaper(":/images/wallpapers/LoadWallpaper.png")
{
    Logger::log("[INFO] Building opening screen layout...");

    setMinimumSize(800, 500);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 36, 40, 36);
    mainLayout->setSpacing(14);

    auto* title = new QLabel("STATIC EDITOR", this);
    QFont titleFont = font();
    titleFont.setPointSize(28);
    titleFont.setWeight(QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        " color: #F2F4FF;"
        " letter-spacing: 2px;"
        "}"
    );

    auto* subtitle = new QLabel("Simple. Fast. Yours.", this);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet(
        "QLabel {"
        " color: #B8BCE8;"
        " font-size: 14px;"
        "}"
    );

    auto* buttonContainer = new QWidget(this);
    auto* buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 20, 0, 20);
    buttonLayout->setSpacing(12);

    const QString buttonStyle =
        "QPushButton {"
        " background-color: rgba(8, 12, 30, 235);"
        " color: #F2F4FF;"
        " border: 1px solid #3546B8;"
        " border-radius: 8px;"
        " padding: 12px 24px;"
        " font-size: 15px;"
        " font-weight: 600;"
        "}"
        "QPushButton:hover {"
        " background-color: rgba(24, 34, 82, 245);"
        " border: 1px solid #5870FF;"
        "}"
        "QPushButton:pressed {"
        " background-color: rgba(70, 40, 120, 250);"
        " border: 1px solid #9B6CFF;"
        "}"
        "QPushButton:focus {"
        " outline: none;"
        " border: 1px solid #7A6CFF;"
        "}";

    auto* newProjectButton = new QPushButton("New Project", buttonContainer);
    auto* openProjectButton = new QPushButton("Open Project", buttonContainer);
    auto* settingsButton = new QPushButton("Settings", buttonContainer);
    auto* exitButton = new QPushButton("Exit", buttonContainer);

    for (QPushButton* button : {
             newProjectButton,
             openProjectButton,
             settingsButton,
             exitButton
         })
    {
        button->setMinimumHeight(48);
        button->setMaximumWidth(360);
        button->setStyleSheet(buttonStyle);
        buttonLayout->addWidget(button);
    }

    connect(newProjectButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] New Project button clicked");
    });

    connect(openProjectButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] Open Project button clicked");
    });

    connect(settingsButton, &QPushButton::clicked, this, [this] {
        Logger::log("[INFO] Settings button clicked");

        SettingsWindow settingsWindow(this);
        settingsWindow.exec();

        Logger::log("[INFO] Settings window closed");
    });

    connect(exitButton, &QPushButton::clicked, this, [] {
        Logger::log("[INFO] Exit button clicked");
        QApplication::quit();
    });

    auto* centeredButtonLayout = new QHBoxLayout;
    centeredButtonLayout->setContentsMargins(0, 0, 0, 0);
    centeredButtonLayout->addStretch();
    centeredButtonLayout->addWidget(buttonContainer);
    centeredButtonLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(centeredButtonLayout);
    mainLayout->addStretch();

    Logger::log("[OK] Opening screen layout built");
}

void OpeningScreen::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_wallpaper.isNull())
    {
        painter.drawPixmap(
            rect(),
            m_wallpaper,
            m_wallpaper.rect()
        );
    }
    else
    {
        painter.fillRect(rect(), QColor("#070A14"));
    }

    // Keep the wallpaper visible while preventing it from overpowering the UI.
    painter.fillRect(rect(), QColor(4, 6, 18, 125));
}
