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
    yolov1 = new DetectYOLOv1(cfg, weight );
    data = new MyQReadData(ui->lineTestData->text());
    ui->progressBar->setValue(0);

    std::vector<float> Precision[LABELNUM];
    std::vector<float> Recall[LABELNUM];
    for(int thre=0; thre<100;thre++)
    {
        YOLOTest *test[LABELNUM];
        for(int cls=0; cls<LABELNUM; cls++){
            test[cls] = new YOLOTest(640,480);
        }
        float threshold =  (float)thre / 100.0;
        for(int i=0; i < data->images.size(); i++){
            // detect
            cv::Mat m = cv::imread(data->images[i].toStdString());
            IplImage ipl = m;
            yolov1->detect(ipl);
            // test

            int labelnum[] = {LABEL_BALL, LABEL_GOALPOST};
            int topN[] = {1, 2};
            std::vector<DetectYOLOv1::bbox_T> predict[LABELNUM];
            std::vector<DetectYOLOv1::bbox_T> g_truth[LABELNUM];
            for(int cls=0; cls<LABELNUM; cls++)
            {
                yolov1->getPredict(labelnum[cls],topN[cls],threshold    ,predict[cls]);
                yolov1->getGroundTruth(labelnum[cls], data->labels[i].toStdString()   ,g_truth[cls]);
                test[cls]->run_test(predict[cls], g_truth[cls]);
            }
        }
        for(int cls=0; cls<LABELNUM; cls++)
        {
            Precision[cls].push_back(test[cls]->precision());
            Recall[cls].push_back(test[cls]->recall());
            delete(test[cls]);
        }
        ui->progressBar->setValue(thre);
    }
    delete(data);
    delete(yolov1);
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
