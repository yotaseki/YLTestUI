#include "detect.h"

YOLO_Detect::YOLO_Detect(std::string &cfg, std::string &weight):
    wsize(640),
    hsize(480)
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
    wsize = src.width;
    hsize = src.height;
    yolo_predict(&src);
}

void YOLO_Detect::getPredict(int labelnum, float threshold, std::vector<bbox_T> &bboxes)
{
    int x,y,w,h;
    float score = .0f;
    int i=0;
    while(true){
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
        if(score == 0.0)break;
        i++;
    }
}

void YOLO_Detect::readLabeltxt(int labelnum, std::string &path, std::vector<bbox_T> &bboxes)
{
    std::ifstream ifs((char *)path.c_str());
    std::string line;
    while(std::getline(ifs,line)){
        if(line.empty() || line[0]=='#'){
            continue;
        }
        std::stringstream buf(line);
        buf >> line;
        int cls = std::stoi(line);
        if(cls == labelnum)
        {
            bbox_T b;
            buf >> line;
            b.x = std::stof(line);
            buf >> line;
            b.y = std::stof(line);
            buf >> line;
            b.w = std::stof(line);
            buf >> line;
            b.h = std::stof(line);
            bboxes.push_back(b);
        }
    }
    ifs.close();
}
