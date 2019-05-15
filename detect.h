#pragma once
#include "darknet.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

class YOLO_Detect
{
public:
     typedef struct bbox_T{
        float x;
        float y;
        float w;
        float h;
        float score;
    }bbox;

    YOLO_Detect(std::string &weight, std::string &cfg, std::vector<float> &thresholds);
    ~YOLO_Detect();
    void setThreshold(std::vector<float> &thresholds);
    void detect(cv::Mat &src);
    void getPredict(int labelnum, float threshold, std::vector<bbox_T> &bboxes);
    void readLabeltxt(int labelnum, std::string &path, std::vector<bbox_T> &bboxes);
private:
    network *net;
    layer layers;
    std::vector<float> thre;
    std::vector<std::vector <bbox_T> > bboxes;
};
