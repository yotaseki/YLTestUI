#include "YOLOTestUI.h"

YOLOTestUI::YOLOTestUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YOLOTestUI),
    vis_ui(new Ui::VisBboxWidget)
{
    ui->setupUi(this);
    vis_ui->setupUi(&vis_widget);
    connectSignals();
    currentdir = QDir::homePath();
    enableRun();
    //ui->checkOpenImage->setEnabled(true);
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
    connect(ui->comboClass, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboIndexChanged()));
}

void YOLOTestUI::onComboIndexChanged()
{
    QString fn = QString("graph_cls%1").arg(ui->comboClass->currentIndex());
    fn = fn + ".png";
    ui->progressBar->setValue(100);
    QImage *mImage = new QImage();
    mImage->load( fn );
    QPixmap pMap = QPixmap::fromImage(*mImage);
    pMap = pMap.scaled(ui->labelGraph->size(), Qt::KeepAspectRatio );
    ui->labelGraph->setPixmap( pMap );
}

void YOLOTestUI::onPushRunTest()
{
    std::string cfg = ui->lineConfig->text().toStdString();
    std::string weight = ui->lineWeights->text().toStdString();
    ui->progressBar->setValue(0);
    ui->plainDebugLog->appendPlainText("Forward... (please wait)");
    data = new YOLO_ReadText(ui->lineTestData->text());
    yolo = new YOLO_Detect(cfg, weight );
    YOLO_Test *test;
    
    images.clear();
    predict.clear();
    g_truth.clear();
    predict.resize(LABELNUM);
    g_truth.resize(LABELNUM);
    for(int i=0; i < data->images.size(); i++){
        // detect
        cv::Mat m = cv::imread(data->images[i].toStdString());
        IplImage ipl = m;
        yolo->detect(ipl);
        for(int cls=0; cls<LABELNUM; cls++){
            // test
            std::vector<YOLO_Detect::bbox_T> p;
            std::vector<YOLO_Detect::bbox_T> g;
            std::string labeltxt = data->labels[i].toStdString();
            yolo->getPredict(cls,0.0,p);
            yolo->readLabeltxt(cls, labeltxt, g);
            predict[cls].push_back(p);
            g_truth[cls].push_back(g);
        }
        images.push_back(m);
    }
    ui->plainDebugLog->appendPlainText("Completed");
    for(int cls=0; cls<LABELNUM; cls++){
        float mAP = 0.0f;
        PlotGraph pg;
        std::vector<double> Precision;
        std::vector<double> Recall;
        for(int thre=0; thre<100;thre++){
            float AP = 0.0f;
            test = new YOLO_Test();
            float threshold =  (float)thre / 100.0;
            for(int i=0; i < data->images.size(); i++){
                test->run_test(predict[cls][i], g_truth[cls][i], thre);
                // debug
                //qDebug() << predict.size();
                //qDebug() << g_truth.size();
                float mIoU;
                test->getMeanIoU(mIoU);
                AP += mIoU;
                // qDebug() << "Image:" << i << ",Pred:"<< predict[cls][i].size() << ",Gth:" << g_truth[cls][i].size();
            }
            AP =  AP / data->images.size();
            mAP += AP;
            float precision, recall;
            test->getPrecision(precision);
            test->getRecall(recall);
            Precision.push_back((double)precision);
            Recall.push_back((double)recall);
            ui->plainDebugLog->appendPlainText(QString("Class:%1,Threshold:%2,AP:%3,Precision:%4,Recall:%5").arg(cls).arg(thre).arg(AP).arg(precision).arg(recall));
            //qDebug() << "Class:" << cls << ",Threshold:" << thre <<",AP:" << AP << ",Precision:" << precision << ",Recall:" << recall;
            ui->progressBar->setValue( (cls*(100/LABELNUM)) + (thre/LABELNUM));
            delete(test);
            //qDebug() << "Thre:" << thre;
        }
        if(cls>0)ui->comboClass->addItem(QString("class %1").arg(cls) );
        std::string fn = "graph_cls" + std::to_string(cls) + ".png";
        pg.set_range(0.0,1.0,0);
        pg.set_range(0.0,1.0,1);
        pg.plot(Precision, Recall, fn);
        mAP = mAP / 100;
    }
    ui->progressBar->setValue(100);
    QImage *mImage = new QImage();
    mImage->load( "graph_cls0.png" );

    QPixmap pMap = QPixmap::fromImage(*mImage);
    pMap = pMap.scaled(ui->labelGraph->size(), Qt::KeepAspectRatio );
    ui->labelGraph->setPixmap( pMap );
    ui->checkOpenImage->setEnabled(true);
    delete(data);
    delete(yolo);
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
    QFileInfo testdata = myq.selectFile(currentdir, QString("*"));
    ui->lineTestData->setText(testdata.filePath());
    enableRun();
}

void YOLOTestUI::stateChangedCheckOpenImage()
{
    if(ui->checkOpenImage->checkState() == Qt::Checked){
        vis_widget.show();
    }else
    {
        vis_widget.close();
    }
}

YOLOTestUI::~YOLOTestUI()
{
    delete ui;
    delete vis_ui;
}

bool YOLOTestUI::enableRun()
{
    QFileInfo checkWeight(ui->lineWeights->text());
    QFileInfo checkConfig(ui->lineConfig->text());
    QFileInfo checkTestData(ui->lineTestData->text());
    bool ret = true;
    if(checkWeight.isFile() && (checkWeight.completeSuffix() == "weights") ){
        ui->labelWeights->setStyleSheet("background-color:green;");
    }
    else{
        ret = false;
        ui->labelWeights->setStyleSheet("background-color:red;");
    }
    if(checkConfig.isFile() && (checkConfig.completeSuffix() == "cfg") ){
        ui->labelConfig->setStyleSheet("background-color:green;");
    }
    else{
        ret = false;
        ui->labelConfig->setStyleSheet("background-color:red;");
    }
    if(checkTestData.isFile() && checkTestData.exists() ){
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
