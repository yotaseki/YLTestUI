#pragma once
#include <darknet/cit_yolo.h>
#include <opencv2/opencv.hpp>

class DetectYOLOv1
{
public:
     typedef struct bbox_T{
        float x;
        float y;
        float w;
        float h;
        float score;
    }bbox;

    DetectYOLOv1(std::string &weight, std::string &cfg);
    ~DetectYOLOv1();
    void detect(IplImage &src);
    void getPredict(int labelnum, int topN, float threshold, std::vector<bbox_T> &bboxes);
    void getGroundTruth(std::string labelpath, std::vector<bbox_T> &bboxes);
private:
    int wsize;
    int hsize;
};
