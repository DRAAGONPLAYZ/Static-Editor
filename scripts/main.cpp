#include <QApplication>
#include <QLabel>
#include <QMainWindow>

#include "logger.h"

#include <iostream>

namespace
{
void log(const QString& message)
{
    const std::string text = message.toStdString();
    std::cout << text << std::endl;
    Logger::write(message);
}
}

int main(int argc, char* argv[])
{
    Logger::initialize();
    log("[START] Static Editor");

    log("[INFO] Creating QApplication...");
    QApplication app(argc, argv);
    log("[OK] QApplication created");

    log("[INFO] Creating main window...");
    QMainWindow window;
    log("[OK] Main window created");

    log("[INFO] Setting window title...");
    window.setWindowTitle("Static Editor");
    log("[OK] Window title set");

    log("[INFO] Setting window size...");
    window.resize(1000, 650);
    log("[OK] Window size set");

    log("[INFO] Creating central label...");
    auto* label = new QLabel("Static Editor - C++ / Qt / FFmpeg", &window);
    label->setAlignment(Qt::AlignCenter);
    log("[OK] Central label created");

    log("[INFO] Setting central widget...");
    window.setCentralWidget(label);
    log("[OK] Central widget set");

    log("[INFO] Showing main window...");
    window.show();
    log("[OK] Main window shown");

    log("[INFO] Starting Qt event loop...");
    const int result = app.exec();
    log(QString("[INFO] Qt event loop ended with code: %1").arg(result));

    Logger::shutdown();
    return result;
}
