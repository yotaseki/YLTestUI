#ifndef YOLOTESTUI_H
#define YOLOTESTUI_H

#include <QMainWindow>
#include <QDebug>
#include <QTextCodec>
#include <darknet/darknet_yolo.h>

namespace Ui {
class YOLOTestUI;
}

class YOLOTestUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit YOLOTestUI(QWidget *parent = 0);
    ~YOLOTestUI();
private slots:
    void onPushRunTest();
    void onPushSelectWeight();
    void onPushSelectConfig();
    void onPushSelectTestData();
    void stateChangedCheckOpenImage();
    
private:
    Ui::YOLOTestUI *ui;
    YOLO_Darknet *yolo;
    void connectSignals();

};

#endif // YOLOTESTUI_H
