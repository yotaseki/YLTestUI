#ifndef MYQCLASS_H
#define MYQCLASS_H

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTime>

enum COLORS
{
    RED = 0,
    GREEN,
    BLUE,
    ORANGE,
    YELLOW,
    AQUA,
    PURPLE,
    DARK,
    NAVY
};

class MyQclass :public QWidget
{
    Q_OBJECT
public:
    QFileInfo selectFile(QDir &current, QString filter);
    QDir selectDir(QDir &current);
    QFileInfoList scanFiles(QString targetpath ,QString filter);
    QFileInfoList scanDirs(QString targetpath ,QString filter);
    QPixmap MatBGR2pixmap(cv::Mat src);
    QPixmap MatBGRA2pixmap(cv::Mat src);
    QDir makeDirectory(QString targetPath, QString dirname);
    QString filenameGen(QString objectname, int num, QString extension=".png");
    QColor retColor(int color);
    cv::Scalar qcolor2scalar(QColor);
private:
};

#endif // MYQCLASS_H
