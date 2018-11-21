#include "test.h"
using namespace std;

YOLOTest::YOLOTest(int img_width, int img_height):
    TP(0),TN(0),FP(0),FN(0)
{
    Width = img_width;
    Height= img_height;
}

YOLOTest::~YOLOTest(){
}

void YOLOTest::run_test(vector<DetectYOLOv1::bbox_T> &pr,vector<DetectYOLOv1::bbox_T> &gt){
    // ground truthから最も近いBoxを評価(評価するBoxが被る可能性有)
    int index;
    if(gt.size() == 0){
        if(pr.size() == 0){
            TN++;
        }
    }
    else{
        for(int i=0;i<gt.size();i++){
            int index;
            float distance=9999999999.9f;
            if(pr.size() > 0){
                for(int j=0;j<pr.size();j++){
                    float d = calcDistance(gt[i].x, gt[i].y, pr[j].x, pr[j].y);
                    if(distance > d){
                        distance = d;
                        index = j;
                    }
                }
                int X,Y,W,H;
                X = (gt[i].x - gt[i].w/2) * Width;
                Y = (gt[i].y - gt[i].h/2) * Height;
                W = gt[i].w * Width;
                H = gt[i].h * Height;
                cv::Rect p(X,Y,W,H);
                X = (pr[index].x - pr[index].w/2) * Width;
                Y = (pr[index].y - pr[index].h/2) * Height;
                W = pr[index].w * Width;
                H = pr[index].h * Height;
                cv::Rect g(X,Y,W,H);
                
                float iou = calcIoU(p,g);
                if(0.5 < iou){
                    TP++;
                }
                else if(iou < 0.5){
                    FP++;
                }
            }
            else{
                FP++;
            }
        }
    }
}

float YOLOTest::calcDistance(float cx1, float cy1, float cx2, float cy2){
    float d = sqrt( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) );
    return d;
}

void YOLOTest::setImageSize(int w, int h){
    Width = w;
    Height = h;
}

float YOLOTest::calcIoU(cv::Rect &p, cv::Rect &r){
    float iou;
    cv::Rect over = p|r;
    int total_area = (p.width*p.height) + (r.width*r.height);
    int overrap_area = over.width*over.height;
    if(total_area + overrap_area == 0){
        return 0;
    }
    iou = overrap_area / (overrap_area + total_area);
    return iou;
}

float YOLOTest::precision(){
    if( (TP+FP) == 0 ){
        return 0.0;
    }
    return (float)TP / (TP + FP);
}

float YOLOTest::recall(){
    if( (TP+FN) == 0 ){
        return 0.0;
    }
    return (float)TP / (TP + FN);
}
