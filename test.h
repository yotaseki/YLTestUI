#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "./detect.h"

class YOLOTest{
public:
    YOLOTest(int img_width, int img_height);
    ~YOLOTest();
    void run_test(std::vector<DetectYOLOv1::bbox_T> &predict, std::vector<DetectYOLOv1::bbox_T> &truth);
    void setImageSize(int w,int h);
    float calcDistance(float cx1, float cy1, float cx2, float cy2);
    float calcIoU(cv::Rect &p, cv::Rect &g);
    float precision();
    float recall();
    int nearestIndex();

private:
    int Width;
    int Height;
    int TP;
    int TN;
    int FP;
    int FN;
};

