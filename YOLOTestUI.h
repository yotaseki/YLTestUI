#ifndef YOLOTESTUI_H
#define YOLOTESTUI_H

#include <QMainWindow>

namespace Ui {
class YOLOTestUI;
}

class YOLOTestUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit YOLOTestUI(QWidget *parent = 0);
    ~YOLOTestUI();

private:
    Ui::YOLOTestUI *ui;
};

#endif // YOLOTESTUI_H
