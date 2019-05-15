#include "detect.h"

YOLO_Detect::YOLO_Detect(std::string &cfg, std::string &weight)
{
    net = load_network( (const char*)cfg.c_str(), (const char*)weight.c_str(), 0);
    layers = net->layers[net->n - 1];
    set_batch_network(net, 1);
    set_threshold(thresholds);
    bboxes.resize(net->cls);
    
}

YOLO_Detect::~YOLO_Detect()
{

}

void YOLO_Detect::detect(cv::Mat &src)
{
    cv::Mat resized;
    cv::resize(src,resized,cv::Size(net->w,net->h));
    IplImage ipl = resized;
    image im = ipl_to_image(&ipl_i);
    float *X = im.data;
    network_predict(net, X);
    int num;
    float threshold = 0.0;
    detection *dets = get_network_boxes(net, im.w, im.h, threshold, .5, 0, 1, &num);
    float nms = .45;
    if(nms){
        do_nms_sort(dets, nboxes, l.classes, nms);
    }
    for(int i=0;i<num;i++){
        for(int cls=0;cls<l.classes;cls++){
            if(dets[i].prob[cls] > thre[cls]){
                bbox_T b;
                b.x = dets[i].bbox.x;
                b.y = dets[i].bbox.y;
                b.w = dets[i].bbox.w;
                b.h = dets[i].bbox.h;
                b.score = dets[i].prob[cls];
                bboxes[cls].push_back(b);
            }
        }
    }
}

void YOLO_Detect::getPredict(int labelnum, float threshold, std::vector<bbox_T> &bbox_v)
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
            //std::cout << "[G]" << cls << "," << b.x << "," << b.y << "," << b.w << "," << b.h << std::endl;
        }
    }
    ifs.close();
}
