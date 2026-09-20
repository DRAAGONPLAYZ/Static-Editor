#include <QApplication>
#include <QLabel>
#include <QMainWindow>

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "[START] Static Editor" << std::endl;

    std::cout << "[INFO] Creating QApplication..." << std::endl;
    QApplication app(argc, argv);
    std::cout << "[OK] QApplication created" << std::endl;

    std::cout << "[INFO] Creating main window..." << std::endl;
    QMainWindow window;
    std::cout << "[OK] Main window created" << std::endl;

    std::cout << "[INFO] Setting window title..." << std::endl;
    window.setWindowTitle("Static Editor");
    std::cout << "[OK] Window title set" << std::endl;

    std::cout << "[INFO] Setting window size..." << std::endl;
    window.resize(1000, 650);
    std::cout << "[OK] Window size set" << std::endl;

    std::cout << "[INFO] Creating central label..." << std::endl;
    auto* label = new QLabel("Static Editor - C++ / Qt / FFmpeg", &window);
    label->setAlignment(Qt::AlignCenter);
    std::cout << "[OK] Central label created" << std::endl;

    std::cout << "[INFO] Setting central widget..." << std::endl;
    window.setCentralWidget(label);
    std::cout << "[OK] Central widget set" << std::endl;

    std::cout << "[INFO] Showing main window..." << std::endl;
    window.show();
    std::cout << "[OK] Main window shown" << std::endl;

    std::cout << "[INFO] Starting Qt event loop..." << std::endl;
    const int result = app.exec();
    std::cout << "[INFO] Qt event loop ended with code: " << result << std::endl;

    std::cout << "[END] Static Editor" << std::endl;
    return result;
}
