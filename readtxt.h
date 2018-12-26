#pragma once

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

class YOLO_ReadText{
public:
    YOLO_ReadText(QString filePath);
    ~YOLO_ReadText();
    QList<QString> images;
    QList<QString> labels;
private:
    void readtxt(QString path);
};
