#include <QApplication>
#include <QMainWindow>

#include "logger.h"
#include "opening_screen.h"

int main(int argc, char* argv[])
{
    Logger::initialize();
    Logger::log("[START] Static Editor");

    Logger::log("[INFO] Creating QApplication...");
    QApplication app(argc, argv);
    app.setOrganizationName("Static Dev Team");
    app.setApplicationName("Static Editor");
    Logger::log("[OK] QApplication created");

    Logger::log("[INFO] Setting application icon...");
    app.setWindowIcon(QIcon(":/images/icons/Icon.png"));
    Logger::log("[OK] Application icon set");

    Logger::log("[INFO] Creating main window...");
    QMainWindow window;
    Logger::log("[OK] Main window created");

    window.setWindowTitle("Static Editor");
    window.resize(1000, 650);

    Logger::log("[INFO] Creating opening screen...");
    auto* openingScreen = new OpeningScreen;
    Logger::log("[OK] Opening screen created");

    Logger::log("[INFO] Setting opening screen as central widget...");
    window.setCentralWidget(openingScreen);
    Logger::log("[OK] Opening screen set");

    Logger::log("[INFO] Showing main window...");
    window.show();
    Logger::log("[OK] Main window shown");

    Logger::log("[INFO] Starting Qt event loop...");
    const int result = app.exec();
    Logger::log(QString("[INFO] Qt event loop ended with code: %1").arg(result));

    Logger::shutdown();
    return result;
}
