#include "test.h"
using namespace std;

YOLO_Test::YOLO_Test():TP(0),TN(0),FP(0),FN(0),test_count(0),sum_iou(0.0),mIoU(0.0)
{
}

YOLO_Test::~YOLO_Test(){
}

void YOLO_Test::run_test(vector<YOLO_Detect::bbox_T> &pr,vector<YOLO_Detect::bbox_T> &gt){
    float iou = 0.0;
    if( (gt.size()>0) && (pr.size()>0) ){
        for(int i=0;i<pr.size();i++){
            // 最も近いbboxの判定
            int index = -1;
            float distance=9999999999.9f;
            for(int j=0;j<gt.size();j++){
                float d = calcDistance(gt[j].x, gt[j].y, pr[i].x, pr[i].y);
                if(distance > d){
                    distance = d;
                    index = j;
                }
            }
            // IoU計算
            int X,Y,W,H;
            X = 448* (pr[i].x - pr[i].w/2);
            Y = 448* (pr[i].y - pr[i].h/2);
            W = 448* pr[i].w;
            H = 448* pr[i].h;
            cv::Rect p(X,Y,W,H);
            //cout << "[P]" << X << ","<< Y << "," << W << "," << H << endl ;
            X = 448* (gt[index].x - gt[index].w/2);
            Y = 448* (gt[index].y - gt[index].h/2);
            W = 448* gt[index].w;
            H = 448* gt[index].h;
            cv::Rect g(X,Y,W,H);
            //cout << "[G]" << X << "," << Y << "," << W << "," << H << endl;
            iou += calcIoU(p,g);
        }
        // TP判定
        iou = iou / pr.size();
        if(iou > 0.5){
            TP++;
        }
        else{
            FP++;
        }
        // mIoU更新
        test_count++;
        sum_iou += iou;
        mIoU = sum_iou/test_count;
    }
    else if( (gt.size()==0) && (pr.size()==0) ){
        TN++;
    }
    else if( (gt.size()>0) && (pr.size()==0) ){
        FN++;
    }
    else if( (gt.size()==0) && (pr.size()>0) ){
        FP++;
    }
}

float YOLO_Test::calcDistance(float cx1, float cy1, float cx2, float cy2){
    float d = sqrt( (cx2-cx1)*(cx2-cx1) + (cy2-cy1)*(cy2-cy1) );
    return d;
}

float YOLO_Test::calcIoU(cv::Rect &p, cv::Rect &r){
    float iou;
    cv::Rect over = p & r;
    int total_area = (p.width*p.height) + (r.width*r.height);
    int overrap_area = over.width*over.height;
    if(total_area + overrap_area == 0){
        return 0.0;
    }
    iou = (float)overrap_area / (total_area - overrap_area);
    return iou;
}

void YOLO_Test::getMeanIoU(float &mIoU)
{
    mIoU = this->mIoU;
}

void YOLO_Test::getPrecision(float &precision)
{
    if( (TP+FP)==0 )
    {
        precision = 0.0f;
    }
    else{
        precision = (float)TP / (TP+FP);
        //std::cout << "precision = TP / (TP + FP)" << std::endl;
        //std::cout << precision << "=" << TP << "/ (" << TP <<" + " << FP << ")" << std::endl;
    }
}

void YOLO_Test::getRecall(float &recall)
{
    if( (TP+FN)==0 )
    {
        recall = 0.0f;
    }
    else{
        recall = (float)TP / (TP+FN);
        //std::cout << "recall = TP / (TP + FN)" << std::endl;
        //std::cout << recall << "=" << TP << "/ (" << TP <<" + " << FN << ")" << std::endl;
    }
}
