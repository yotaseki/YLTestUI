#include "YOLOTestUI.h"
#include "ui_YOLOTestUI.h"

YOLOTestUI::YOLOTestUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YOLOTestUI)
{
    ui->setupUi(this);
}

void YOLOTestUI::connectSignals()
{
    connect(ui->pushSelectWeights, SIGNAL(clicked(bool)), this, SLOT(onPushRunTest()));
    connect(ui->pushSelectConfig, SIGNAL(clicked(bool)), this, SLOT(onPushSelectWeight()));
    connect(ui->pushSelectTestData, SIGNAL(clicked(bool)), this, SLOT(onPushSelectConfig()));
    connect(ui->pushRunTest, SIGNAL(clicked(bool)), this, SLOT(onPushSelectTestData()));
    connect(ui->checkOpenImage, SIGNAL(stateChanged(int)), this, SLOT(stateChangedCheckOpenImage()));
}

void onPushRunTest()
{

    yolo = new YOLO_Darknet(
}

void onPushSelectWeight()
{

}

void onPushSelectConfig()
{

}

void onPushSelectTestData()
{

}

void stateChangedCheckOpenImage()
{

}

YOLOTestUI::~YOLOTestUI()
{
    delete ui;
}
