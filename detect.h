#pragma once
#include <darknet/cit_yolo.h>
#include <opencv2/opencv.hpp>

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
    void detect(IplImage &src);
    void getPredict(int labelnum, int topN, float threshold, std::vector<bbox_T> &bboxes);
    void getGroundTruth(int labelnum, std::string labelpath, std::vector<bbox_T> &bboxes);
private:
    int wsize;
    int hsize;
};
