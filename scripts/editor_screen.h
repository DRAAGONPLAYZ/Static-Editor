#pragma once

#include <QByteArray>
#include <QWidget>

class QTabWidget;

class EditorScreen : public QWidget
{
public:
    explicit EditorScreen(QWidget* parent = nullptr);

private:
    void initializeTemporaryProject();
    void buildInterface();

    QByteArray m_temporarySevData;
    QTabWidget* m_mediaTabs = nullptr;
};
