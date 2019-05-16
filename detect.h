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

    YOLO_Detect(std::string &weight, std::string &cfg);
    ~YOLO_Detect();
    void detect(cv::Mat &src);
    void getPredict(int cls, float threshold, std::vector<bbox_T> &bboxes);
    void readLabeltxt(int cls, std::string &path, std::vector<bbox_T> &bboxes);
    int retClassNum();
private:
    int num_class;
    network *net;
    layer layers;
    std::vector<std::vector <bbox_T> > bboxes;
};
