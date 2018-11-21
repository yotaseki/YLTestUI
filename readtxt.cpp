#include "./readtxt.h"


MyQReadData::MyQReadData(QString path)
{
    readtxt(path);
}

MyQReadData::~MyQReadData(){
}

void MyQReadData::readtxt(QString path){
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
    file.close();
}
