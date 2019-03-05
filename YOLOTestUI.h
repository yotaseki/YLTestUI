#ifndef YOLOTESTUI_H
#define YOLOTESTUI_H

#include <QMainWindow>
#include <QDebug>
#include <QTextCodec>
#include "./detect.h"
#include "./MyQclass.h"
#include "./readtxt.h"
#include "./test.h"
#include "./plotGraph.h"
#include "ui_YOLOTestUI.h"
#include "ui_VisBboxUI.h"

namespace Ui {
class YOLOTestUI;
class VisBboxWidget;
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
    void onComboIndexChanged();
    void stateChangedCheckOpenImage();
    bool enableRun();

private:
    Ui::YOLOTestUI *ui;
    Ui::VisBboxWidget *vis_ui;
    QWidget vis_widget;
    void connectSignals();
    MyQclass myq;
    QDir currentdir;
    YOLO_Detect *yolo;
    YOLO_ReadText *data;
    std::vector<cv::Mat> images;
    std::vector<std::vector<std::vector<YOLO_Detect::bbox_T> > > predict;
    std::vector<std::vector<std::vector<YOLO_Detect::bbox_T> > > g_truth;
};

#endif // YOLOTESTUI_H
