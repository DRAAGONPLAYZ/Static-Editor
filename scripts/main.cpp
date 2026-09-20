#include <QApplication>
#include <QLabel>
#include <QMainWindow>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Static Editor");
    window.resize(1000, 650);

    auto* label = new QLabel("Static Editor - C++ / Qt / FFmpeg", &window);
    label->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(label);

    return app.exec();
}
