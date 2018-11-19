#include "YOLOTestUI.h"
#include "ui_YOLOTestUI.h"

YOLOTestUI::YOLOTestUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YOLOTestUI)
{
    ui->setupUi(this);
    connectSignals();

}

void YOLOTestUI::connectSignals()
{
    connect(ui->pushSelectWeights, SIGNAL(clicked(bool)), this, SLOT(onPushSelectWeight()));
    connect(ui->pushSelectConfig, SIGNAL(clicked(bool)), this, SLOT(onPushSelectConfig()));
    connect(ui->pushSelectTestData, SIGNAL(clicked(bool)), this, SLOT(onPushSelectTestData()));
    connect(ui->pushRunTest, SIGNAL(clicked(bool)), this, SLOT(onPushRunTest()));
    connect(ui->checkOpenImage, SIGNAL(stateChanged(int)), this, SLOT(stateChangedCheckOpenImage()));
}

void YOLOTestUI::onPushRunTest()
{
    QByteArray ba1 = ui->lineWeights->text().toLocal8Bit();
    char *weight = ba1.data();
    QByteArray ba2 = ui->lineConfig->text().toLocal8Bit();
    char *cfg = ba2.data();
    QByteArray ba3 = ui->lineTestData->text().toLocal8Bit();
    char *testdata = ba3.data();
    int threshold[]  ={0,0};
    yolo = new YOLO_Darknet(weight,cfg,threshold);
}

void YOLOTestUI::onPushSelectWeight()
{

}

void YOLOTestUI::onPushSelectConfig()
{

}

void YOLOTestUI::onPushSelectTestData()
{

}

void YOLOTestUI::stateChangedCheckOpenImage()
{

}

YOLOTestUI::~YOLOTestUI()
{

    delete(yolo);
    delete ui;
}
