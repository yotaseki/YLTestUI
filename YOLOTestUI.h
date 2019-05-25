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
    void valueChangedVisPage(int);
    void valueChangedVisThreshold(int);

private:
    Ui::YOLOTestUI *ui;
    Ui::VisBboxWidget *vis_ui;
    QWidget vis_widget;
    MyQclass myq;
    int vis_num;
    int num_class;
    QDir currentdir;
    QWidget *scrollVisContent;
    QLabel *vis_img;
    QLabel *vis_fname;
    QVBoxLayout *vis_vlay;
    YOLO_Detect *yolo;
    YOLO_ReadText *data;
    std::vector<cv::Mat> images;
    std::vector<QString> images_path;
    std::vector<std::vector<std::vector<YOLO_Detect::bbox_T> > > predict;
    std::vector<std::vector<std::vector<YOLO_Detect::bbox_T> > > g_truth;
    void connectSignals();
    void drawBbox(cv::Mat &img, cv::Mat &dst, std::vector<YOLO_Detect::bbox_T>  p_bbox, std::vector<YOLO_Detect::bbox_T>  t_bbox, float threshold, int color_idx);
    void displayResultImageOnScrollVisResults(int page);
    void drawBbox(cv::Mat &src, cv::Mat &dst, YOLO_Detect::bbox_T &bbox);
};

#endif // YOLOTESTUI_H
