#pragma once

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

class MyQReadData{
public:
    MyQReadData(QString filePath);
    ~MyQReadData();
    QList<QString> images;
    QList<QString> labels;
private:
    void readtxt(QString path);
};
