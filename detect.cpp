#include "detect.h"

static IplImage *image_to_ipl(image im)
{
    int x,y,c;
    IplImage *disp = cvCreateImage(cvSize(im.w,im.h), IPL_DEPTH_8U, im.c);
    int step = disp->widthStep;
    for(y = 0; y < im.h; ++y){
        for(x = 0; x < im.w; ++x){
            for(c= 0; c < im.c; ++c){
                float val = im.data[c*im.h*im.w + y*im.w + x];
                disp->imageData[y*step + x*im.c + c] = (unsigned char)(val*255);
            }
        }
    }
    return disp;
}

static image ipl_to_image(IplImage* src)
{
    int h = src->height;
    int w = src->width;
    int c = src->nChannels;
    image im = make_image(w, h, c);
    unsigned char *data = (unsigned char *)src->imageData;
    int step = src->widthStep;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    return im;
}

YOLO_Detect::YOLO_Detect(std::string &cfg, std::string &weight)
{
    net = load_network( (char*)cfg.c_str(), (char*)weight.c_str(), 0);
    layers = net->layers[net->n - 1];
    num_class = layers.classes;
    set_batch_network(net, 1);
    bboxes.resize(num_class);
}

YOLO_Detect::~YOLO_Detect()
{

}

void YOLO_Detect::detect(cv::Mat &src)
{
    cv::Mat resized = src.clone();
    cv::resize(resized,resized,cv::Size(net->w,net->h));
    IplImage ipl = resized;
    image im = ipl_to_image(&ipl);
    float *X = im.data;
    network_predict(net, X);
    int num_bbox;
    float threshold = 0.0;
    detection *dets = get_network_boxes(net, im.w, im.h, threshold, .5, 0, 1, &num_bbox);
    float nms = .45;
    if(nms){
        do_nms_sort(dets, num_bbox, num_class, nms);
    }
    for(int i=0;i<num_bbox;i++){
        for(int cls=0;cls<num_class;cls++){
            bbox_T b = {
                dets[i].bbox.x
                ,dets[i].bbox.y
                ,dets[i].bbox.w
                ,dets[i].bbox.h
                ,dets[i].prob[cls]
            };
            bboxes[cls].push_back(b);
        }
    }
    free_detections(dets, num_bbox);
    free_image(im);
}

void YOLO_Detect::getPredict(int cls, float threshold, std::vector<bbox_T> &bbox_v)
{
    bbox_v.clear();
    for(int i=0;i<bboxes[cls].size();i++){
        if(bboxes[cls][i].score > threshold){
            bbox b = {
                bboxes[cls][i].x
                ,bboxes[cls][i].y
                ,bboxes[cls][i].w
                ,bboxes[cls][i].h
                ,bboxes[cls][i].score
            };
            bbox_v.push_back(b);
        }
    }
}

void YOLO_Detect::readLabeltxt(int cls, std::string &path, std::vector<bbox_T> &bboxes)
{
    std::ifstream ifs((char *)path.c_str());
    std::string line;
    while(std::getline(ifs,line)){
        if(line.empty() || line[0]=='#'){
            continue;
        }
        std::stringstream buf(line);
        buf >> line;
        int labelnum = std::stoi(line);
        if(labelnum == cls)
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

int YOLO_Detect::retClassNum(){
    return num_class;
}
