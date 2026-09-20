#pragma once

#include <QDialog>

class QCheckBox;
class QComboBox;

class SettingsWindow : public QDialog
{
public:
    explicit SettingsWindow(QWidget* parent = nullptr);

private:
    void loadSettings();
    void saveSettings();

    QCheckBox* m_hardwareAcceleration = nullptr;
    QComboBox* m_previewQuality = nullptr;
    QComboBox* m_exportCacheLocation = nullptr;
};
