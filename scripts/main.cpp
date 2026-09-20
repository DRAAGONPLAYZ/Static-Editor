#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QIcon>
#include <QMainWindow>
#include <QString>

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

    Logger::log("[INFO] Loading bundled Inter fonts...");

    const int regularFontId =
        QFontDatabase::addApplicationFont(":/fonts/Inter-Regular.ttf");
    const int mediumFontId =
        QFontDatabase::addApplicationFont(":/fonts/Inter-Medium.ttf");
    const int semiBoldFontId =
        QFontDatabase::addApplicationFont(":/fonts/Inter-SemiBold.ttf");
    const int boldFontId =
        QFontDatabase::addApplicationFont(":/fonts/Inter-Bold.ttf");

    if (regularFontId < 0 ||
        mediumFontId < 0 ||
        semiBoldFontId < 0 ||
        boldFontId < 0)
    {
        Logger::log("[ERROR] One or more bundled Inter fonts failed to load");
    }
    else
    {
        QFont applicationFont = app.font();
        applicationFont.setFamily("Inter");
        app.setFont(applicationFont);

        Logger::log("[OK] Inter loaded and set as application-wide font");
    }

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
