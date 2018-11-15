#include "YOLOTestUI.h"
#include "ui_YOLOTestUI.h"

YOLOTestUI::YOLOTestUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::YOLOTestUI)
{
    ui->setupUi(this);
}

YOLOTestUI::~YOLOTestUI()
{
    delete ui;
}
