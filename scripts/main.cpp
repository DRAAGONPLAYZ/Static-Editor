#include <QApplication>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QPixmap>

#include "logger.h"

int main(int argc, char* argv[])
{
    Logger::initialize();
    Logger::log("[START] Static Editor");

    Logger::log("[INFO] Creating QApplication...");
    QApplication app(argc, argv);
    Logger::log("[OK] QApplication created");

    Logger::log("[INFO] Setting application icon...");
    app.setWindowIcon(QIcon(":/images/icons/Icon.png"));
    Logger::log("[OK] Application icon set");

    Logger::log("[INFO] Creating main window...");
    QMainWindow window;
    Logger::log("[OK] Main window created");

    Logger::log("[INFO] Setting window title...");
    window.setWindowTitle("Static Editor");
    Logger::log("[OK] Window title set");

    Logger::log("[INFO] Setting window size...");
    window.resize(1000, 650);
    Logger::log("[OK] Window size set");

    Logger::log("[INFO] Creating loading wallpaper...");
    auto* wallpaper = new QLabel(&window);
    wallpaper->setPixmap(QPixmap(":/images/wallpapers/LoadWallpaper.png"));
    wallpaper->setScaledContents(true);
    wallpaper->setAlignment(Qt::AlignCenter);
    Logger::log("[OK] Loading wallpaper created");

    Logger::log("[INFO] Setting loading wallpaper as central widget...");
    window.setCentralWidget(wallpaper);
    Logger::log("[OK] Loading wallpaper set");

    Logger::log("[INFO] Showing main window...");
    window.show();
    Logger::log("[OK] Main window shown");

    Logger::log("[INFO] Starting Qt event loop...");
    const int result = app.exec();
    Logger::log(QString("[INFO] Qt event loop ended with code: %1").arg(result));

    Logger::shutdown();
    return result;
}
