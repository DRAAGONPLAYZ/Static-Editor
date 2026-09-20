#include <QApplication>
#include <QLabel>
#include <QMainWindow>

#include "logger.h"

int main(int argc, char* argv[])
{
    Logger::initialize();
    Logger::log("[START] Static Editor");

    Logger::log("[INFO] Creating QApplication...");
    QApplication app(argc, argv);
    Logger::log("[OK] QApplication created");

    Logger::log("[INFO] Creating main window...");
    QMainWindow window;
    Logger::log("[OK] Main window created");

    Logger::log("[INFO] Setting window title...");
    window.setWindowTitle("Static Editor");
    Logger::log("[OK] Window title set");

    Logger::log("[INFO] Setting window size...");
    window.resize(1000, 650);
    Logger::log("[OK] Window size set");

    Logger::log("[INFO] Creating central label...");
    auto* label = new QLabel("Static Editor - C++ / Qt / FFmpeg", &window);
    label->setAlignment(Qt::AlignCenter);
    Logger::log("[OK] Central label created");

    Logger::log("[INFO] Setting central widget...");
    window.setCentralWidget(label);
    Logger::log("[OK] Central widget set");

    Logger::log("[INFO] Showing main window...");
    window.show();
    Logger::log("[OK] Main window shown");

    Logger::log("[INFO] Starting Qt event loop...");
    const int result = app.exec();
    Logger::log(QString("[INFO] Qt event loop ended with code: %1").arg(result));

    Logger::shutdown();
    return result;
}
