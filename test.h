#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "./detect.h"

class YOLO_Test{
public:
    YOLO_Test();
    ~YOLO_Test();
    void run_test(std::vector<YOLO_Detect::bbox_T> &predict, std::vector<YOLO_Detect::bbox_T> &truth);
    float calcDistance(float cx1, float cy1, float cx2, float cy2);
    float calcIoU(cv::Rect &p, cv::Rect &g);
    void getMeanIoU(float &mIoU);
    void getPrecision(float &precision);
    void getRecall(float &recall);
private:
    int TP,TN,FP,FN;
    int test_count;
    float sum_iou, mIoU;
};

