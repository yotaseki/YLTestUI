#include "YOLOTestUI.h"
#include "ui_YOLOTestUI.h"

YOLOTestUI::YOLOTestUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YOLOTestUI)
{
    ui->setupUi(this);
    connectSignals();
    currentdir = QDir::homePath();
    enableRun();
}

void YOLOTestUI::connectSignals()
{
    connect(ui->pushSelectWeights, SIGNAL(clicked(bool)), this, SLOT(onPushSelectWeight()));
    connect(ui->pushSelectConfig, SIGNAL(clicked(bool)), this, SLOT(onPushSelectConfig()));
    connect(ui->pushSelectTestData, SIGNAL(clicked(bool)), this, SLOT(onPushSelectTestData()));
    connect(ui->pushRunTest, SIGNAL(clicked(bool)), this, SLOT(onPushRunTest()));
    connect(ui->checkOpenImage, SIGNAL(stateChanged(int)), this, SLOT(stateChangedCheckOpenImage()));
    connect(ui->lineWeights, SIGNAL(textChanged(QString)), this, SLOT(enableRun()));
    connect(ui->lineConfig, SIGNAL(textChanged(QString)), this, SLOT(enableRun()));
    connect(ui->lineTestData, SIGNAL(textChanged(QString)), this, SLOT(enableRun()));
}

void YOLOTestUI::onPushRunTest()
{
    std::string cfg = ui->lineConfig->text().toStdString();
    std::string weight = ui->lineWeights->text().toStdString();
    ui->progressBar->setValue(0);
    data = new YOLO_ReadText(ui->lineTestData->text());
    yolo = new YOLO_Detect(cfg, weight );
    YOLO_Test *test;

    int labelnum[] = {LABEL_BALL, LABEL_GOALPOST};
    for(int cls=0; cls<LABELNUM; cls++){
        test = new YOLO_Test();
        for(int thre=0; thre<100;thre++)
        {
            float threshold =  (float)thre / 100.0;
            for(int i=0; i < data->images.size(); i++){
                // detect
                cv::Mat m = cv::imread(data->images[i].toStdString());
                IplImage ipl = m;
                yolo->detect(ipl);
                // test
                std::vector<YOLO_Detect::bbox_T> predict;
                std::vector<YOLO_Detect::bbox_T> g_truth;
                std::string labeltxt = data->labels[i].toStdString();
                yolo->getPredict(labelnum[cls],threshold,predict);
                yolo->readLabeltxt(labelnum[cls], labeltxt, g_truth);
                test->run_test(predict, g_truth);
                // debug
                //qDebug() << predict.size();
                //qDebug() << g_truth.size();
                qDebug() << test.mIoU;
            }
            ui->progressBar->setValue(thre);
        }
        delete(test);
    }
    delete(data);
    delete(yolo);
    ui->checkOpenImage->setEnabled(true);
}

void YOLOTestUI::onPushSelectWeight()
{
    QFileInfo weight = myq.selectFile(currentdir, QString("*.weights"));
    ui->lineWeights->setText(weight.filePath());
}

void YOLOTestUI::onPushSelectConfig()
{
    QFileInfo cfg = myq.selectFile(currentdir, QString("*.cfg"));
    ui->lineConfig->setText(cfg.filePath());
}

void YOLOTestUI::onPushSelectTestData()
{
    QFileInfo testdata = myq.selectFile(currentdir, QString("*.txt"));
    ui->lineTestData->setText(testdata.filePath());
    enableRun();
}

void YOLOTestUI::stateChangedCheckOpenImage()
{

}

YOLOTestUI::~YOLOTestUI()
{
    delete ui;
}

bool YOLOTestUI::enableRun()
{
    QFileInfo checkWeight(ui->lineWeights->text());
    QFileInfo checkConfig(ui->lineConfig->text());
    QFileInfo checkTestData(ui->lineTestData->text());
    bool ret = true;
    if(checkWeight.isFile() && checkWeight.exists()){
        ui->labelWeights->setStyleSheet("background-color:green;");
    }
    else{
        ret = false;
        ui->labelWeights->setStyleSheet("background-color:red;");
    }
    if(checkConfig.isFile() && checkConfig.exists()){
        ui->labelConfig->setStyleSheet("background-color:green;");
    }
    else{
        ret = false;
        ui->labelConfig->setStyleSheet("background-color:red;");
    }
    if(checkTestData.isFile() && checkTestData.exists()){
        ui->labelTestData->setStyleSheet("background-color:green;");
    }
    else{
        ret = false;
        ui->labelTestData->setStyleSheet("background-color:red;");
    }
    if(ret)
    {
        ui->pushRunTest->setEnabled(true);
    }
    else
    {
        ui->pushRunTest->setDisabled(true);
    }
    return ret;
}
