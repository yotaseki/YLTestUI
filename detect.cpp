#include "detect.h"

YOLO_Detect::YOLO_Detect(std::string &cfg, std::string &weight):
    wsize(320),
    hsize(240)
{
    int threshold[2] = {0,0};
    yolo_init((char *)cfg.c_str(), (char *)weight.c_str(), threshold);
}

YOLO_Detect::~YOLO_Detect()
{
    yolo_fina();
}

void YOLO_Detect::detect(IplImage &src)
{
    yolo_predict(&src);
    wsize = src.width;
    hsize = src.height;
}

void YOLO_Detect::getPredict(int labelnum, int topN, float threshold, std::vector<bbox_T> &bboxes)
{
    int x,y,w,h;
    float score = .0f;
    for(int i=0;i<topN;i++){
        yolo_get_object(labelnum, i, x, y, w, h, score);
        if(score > threshold){
            bbox pred = {
                (float)x / wsize,
                (float)y / hsize,
                (float)w / wsize,
                (float)h / hsize,
                score
            };
            bboxes.push_back(pred);
        }
    }
}

void YOLO_Detect::getGroundTruth(int labelnum, std::string labelpath, std::vector<bbox_T> &bboxes)
{

}
