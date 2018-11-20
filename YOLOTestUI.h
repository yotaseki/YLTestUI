#ifndef YOLOTESTUI_H
#define YOLOTESTUI_H

#include <QMainWindow>
#include <QDebug>
#include <QTextCodec>
#include "./detect.h"
#include "./MyQclass.h"
#include "./readtxt.h"

#define LABELNUM 2
enum LABELS{
    LABEL_BALL = 0,
    LABEL_GOALPOST
};

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
    bool enableRun();

private:
    Ui::YOLOTestUI *ui;
    void connectSignals();
    DetectYOLOv1 *yolov1;
    MyQclass myq;
    QDir currentdir;
    MyQReadData *data;
};

#endif // YOLOTESTUI_H
