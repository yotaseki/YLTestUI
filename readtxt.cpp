#include "./readtxt.h"


YOLO_ReadText::YOLO_ReadText(QString path)
{
    YOLOReadtxt(path);
}

YOLO_ReadText::~YOLO_ReadText(){
}

void YOLO_ReadText::readtxt(QString path){
    QFile file(path);
    if(!file.open(QIODevice::YOLOReadOnly)){
        QString errStr = "Open failed. :" + path;
        qDebug() << errStr;
    }
    QTextStream in(&file);
    QString inStr = in.readAll();
    images = inStr.split('\n');
    inStr = inStr.replace("images","labels");
    inStr = inStr.replace(".jpg", ".txt");
    inStr = inStr.replace(".png", ".txt");
    labels = inStr.split('\n');
    file.close();
}
