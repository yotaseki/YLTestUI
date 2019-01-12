#include "./readtxt.h"


YOLO_ReadText::YOLO_ReadText(QString path)
{
    readTraintxt(path);
}

YOLO_ReadText::~YOLO_ReadText(){
}

void YOLO_ReadText::readTraintxt(QString path){
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
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
    if(images[images.size()-1].length() == 0)
    {
        images.removeLast();
    }
    if(labels[labels.size()-1].length() == 0)
    {
        labels.removeLast();
    }
    file.close();
}

