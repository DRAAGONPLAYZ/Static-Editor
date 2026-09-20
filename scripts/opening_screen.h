#pragma once

#include <QPixmap>
#include <QWidget>

class OpeningScreen : public QWidget
{
public:
    explicit OpeningScreen(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_wallpaper;
};
