#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <opencv2/shape.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <string>
#include "DBGLayer.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
USING_NS_CC;
DBGLayer * _g_dbg_layer = nullptr;
std::mutex HelloWorld::s_mtx_change_texture;
bool HelloWorld::s_dirty = false;
cv::Mat HelloWorld::img;
cv::Ptr<SVM> svm2;
bool _l_running = true;

double median(cv::Mat channel)
{
    double m = (channel.rows*channel.cols) / 2;
    int bin = 0;
    double med = -1.0;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist;
    cv::calcHist(&channel, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    for (int i = 0; i < histSize && med < 0.0; ++i)
    {
        bin += cvRound(hist.at< float >(i));
        if (bin > m && med < 0.0)
            med = i;
    }
    return med;
}

Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols / 2., src.rows / 2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
    return dst;
}

Mat HelloWorld::detectNumber(Mat img1,char *path, string result) {
    Mat img;
    img1.copyTo(img);
    if (img1.cols>img1.rows) {
        resize(img1, img, cvSize(960, 720), CV_INTER_LINEAR);
    }
    else {
        resize(img1, img, cvSize(720, 960), CV_INTER_LINEAR);
    }
    Mat imgx;
    img.copyTo(imgx);
    cvtColor(imgx, imgx, CV_RGB2GRAY);
    Mat img2, img11;
    medianBlur(imgx, img11, 7 );
    cv::threshold(img11, img2, 200, 255, THRESH_BINARY_INV);
    
    double sigma = 0.33;
    double v = median(img2);
    double lower = int(MAX(0, (1.0 - sigma)*v));
    double upper = int(MIN(255, (1.0 + sigma)*v));
    Mat edged;
    Canny(img2, edged, lower, upper);
    Mat copy;
    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;
    vector<int> myvector;
    findContours(edged, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat cr;
    //std::sort(contours1.begin(), contours1.end(),compareContourAreas);
    cv::Mat crop(100, 784, CV_32FC1);
    int dem = 0;
    vector<cv::Point> listPoint;
    for (int i = 0; i < contours.size(); i++) {
        int peri = cv::arcLength(contours.at(i), true);
        vector<cv::Point> approx;
        approxPolyDP(contours.at(i), approx, 0.02*peri, true);
        Mat imgx2;
        Vec<int, 4> hi = hierarchy.at(i);
        cv::Rect r = boundingRect(contours.at(i));
        
        cv::Rect ra,big;
        
        if (approx.size() >= 4 && hi[3] == -1 && hi[2] != -1 && (r.height + r.width)>100) {
            Mat abcd(img, r);
            //imshow("abc", abcd);
            //waitKey();
            int child_idx = hi[2];
            vector<cv::Point> child_c = contours.at(child_idx);
            Vec<int, 4> hi2 = hierarchy.at(child_idx);
            cv::Rect rx = boundingRect(child_c);
            if (r == rx && hi2[2]!=-1) {
                int childC_index = hi2[2];
                Vec<int, 4> hi2C = hierarchy.at(childC_index);
                vector<cv::Point> child_cC = contours.at(childC_index);
                cv::Rect rxC = boundingRect(child_cC);
                big = rxC;
                listPoint.push_back({ (rxC.x + rxC.width / 2), (rxC.y + rxC.height / 2) });
                while (hi2C[0] != -1) {
                    vector<cv::Point> child_cN = contours.at(hi2C[0]);
                    int indxxx = hi2C[0];
                    cv::Rect rxC1 = boundingRect(child_cN);
                    if ((float)(rxC1.height * 2 + rxC1.width * 2) / (float)(rx.height * 2 + rx.width * 2) > 0.1) {
                        ra=rxC1;
                        big = ra | rxC;
                    }
                    hi2C = hierarchy.at(indxxx);
                }
                
                Mat child_img(img, big);
                //imshow("child", child_img);
                //waitKey();
                //cout << "2 countour";
            }
            else {
                if ((float)(rx.height*2+ rx.width * 2) / (r.height*2+r.width*2) > 0.1) {
                    big = rx;
                    listPoint.push_back({ (rx.x + rx.width / 2), (rx.y + rx.height / 2) });
                    while (hi2[0] != -1) {
                        vector<cv::Point> child_cN = contours.at(hi2[0]);
                        int indxxx = hi2[0];
                        cv::Rect rxC = boundingRect(child_cN);
                        if ((float)(rxC.height * 2 + rxC.width * 2) / (float)(r.height * 2 + r.width * 2) > 0.1) {
                            ra=rxC;
                            big = ra | rx;
                        }
                        hi2 = hierarchy.at(indxxx);
                    }
                    Mat child_img(img, big);
                    //imshow("abc", child_img);
                    //waitKey();
                    //cout << "1 contour";
                }
            }
            Mat cr;
            Mat croppedImage;
            Mat ROI(imgx,big);
            
            ROI.copyTo(cr);
            Mat croop;
            cv::resize(cr, croop, CvSize(28, 28), 0, 0, CV_INTER_LINEAR);
            Mat bina;
            croop.copyTo(bina);
            cv::threshold(croop, bina, 200, 255, THRESH_BINARY_INV);
            
            /*
             //data 6 9
             for (int i = 0; i < 540; i++) {
             std::ostringstream buff;
             buff << "o9\\09"<<i << ".png";
             Mat dst = rotate(bina, (double)360 / 540 * i);
             imwrite(buff.str() , dst);
             waitKey();
             }
             
             for (int i = 540; i < 1080; i++) {
             std::ostringstream buff;
             buff << "o9\\09" << i << ".png";
             Mat dst = rotate(bina, (double)360 / 540 * i);
             cv::flip(dst, dst, 1);
             imwrite(buff.str(), dst);
             waitKey();
             }
             */
            
            int ii = 0;
            for (int x = 0; x<bina.rows; x++) {
                for (int y = 0; y<bina.cols; y++) {
                    crop.at<float>(dem, ii++) = (float)bina.at<uchar>(x, y);
                }
            }
            dem++;
        }
    }
    
    Mat distances = cv::Mat::zeros(dem, dem, CV_32FC1);
    for (int i = 0; i < dem; i++) {
        distances.at<float>(i, i) = 0;
        for (int j = i + 1; j < dem; j++) {
            distances.at<float>(i, j) = sqrt((listPoint.at(i).x - listPoint.at(j).x)*(listPoint.at(i).x - listPoint.at(j).x) + (listPoint.at(i).y - listPoint.at(j).y)*(listPoint.at(i).y - listPoint.at(j).y));
            distances.at<float>(j, i) = sqrt((listPoint.at(i).x - listPoint.at(j).x)*(listPoint.at(i).x - listPoint.at(j).x) + (listPoint.at(i).y - listPoint.at(j).y)*(listPoint.at(i).y - listPoint.at(j).y));
        }
    }
    //cv::Ptr<SVM> svm2 = SVM::create();
    //svm2 = svm2->load(path);
    cv::Mat res;
    svm2->predict(crop, res);
    int i = 0;
    std::ostringstream ss;
    
    std::vector<int8_t> result1;
    while (i< dem) {
        this->removeChildByTag(1);
        //int result = res.at<float>(i);
        result1.push_back(res.at<float>(i));
        ss << result1[i] << " ";
        printf("%d", result1[i]);
        auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
        //std::string s(ss.str());
        
        label->setString(std::to_string(result1[i]));
        label->setTag(1);
        label->setPosition(300 , 50);
        this->addChild(label);
        i++;
    }
    std::string s(ss.str());
    //cout << s << endl;
    //printf("%s", s.c_str());
    result = s.c_str();
    return distances;
}
/*
int main() {
    Mat img = imread("color\\Photo 2-14-17, 10 45 54 AM.jpg");
    Mat distances;
    string result;
    distances= detectNumber(img, "model.xml",result) ;
    cout << result;
    cout << distances.size().width << " height	=" << distances.size().height << endl;
    for (int i = 0; i < distances.size().width; i++) {
        for (int j = 0; j < distances.size().height; j++) {
            cout << distances.at<float>(i, j) <<"---";;
        }
        cout << endl;
    }
}
 */

/*string HelloWorld::detectNumber(const Mat &img, const std::string &path2) {
    
    Mat imgx;
    img.copyTo(imgx);
    cvtColor(imgx, imgx, CV_RGB2GRAY);
    Mat img2;
    
    bilateralFilter(imgx,img2,11, 13, 13);
    double sigma =0.33;
    double v =median(img2);
    double lower= int(MAX(0,(1.0-sigma)*v));
    double upper= int(MIN(255,(1.0+sigma)*v));
    Mat edged;
    Canny(img2,edged,lower,upper);
    Mat copy;
    vector<vector<cv::Point> > contours;
    vector<Vec4i> hierarchy;
    vector<int> myvector;
    findContours( edged, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    Mat cr;
    //std::sort(contours1.begin(), contours1.end(),compareContourAreas);
    cv::Mat crop(100, 784, CV_32FC1);
    int dem=0;
    for (int i = 0; i < contours.size(); i++){
        int peri=cv::arcLength(contours.at(i), true);
        vector<cv::Point> approx ;
        approxPolyDP(contours.at(i),approx,0.02*peri,true);
        Mat imgx2;
        Vec<int,4> hi = hierarchy.at(i);
        cv::Rect r= boundingRect(contours.at(i));
        
        if(approx.size()==4&& hi[3]!=-1&& r.height>28){
            int child_idx= hi[2];
            vector<cv::Point> child_c= contours.at(child_idx);
            cv::Rect r1= boundingRect(child_c);
            
            Mat croppedImage;
            Mat ROI(img, r1);
            ROI.copyTo(cr);
            
            
            Mat croop;
            cv::resize(cr, croop, CvSize(28,28), 0, 0, CV_INTER_LINEAR);
            Mat bina;
            cvtColor(croop,bina,CV_RGB2GRAY);
            cv::threshold(bina, bina,200,255,THRESH_BINARY_INV );
            
            int ii = 0;
            for (int x = 0; x<bina.rows; x++) {
                for (int y = 0; y<bina.cols; y++) {
                    crop.at<float>(dem, ii++) = (float)bina.at<uchar>(x, y) ;//- 255.0/2)/255.0;
                    //cout << data.at<double>(i, x * 28 + y) << "---" << i << "---" << (x * 28 + y) << endl;
                    
                    
                }
            }
            dem++;
        }
        
    }
    
    //cv::Ptr<SVM> svm2 = SVM::create();
    // vecData[i]=im;
    cv::Mat res;
    
        //svm2 = svm2->load(path2);
    //CCASSERT(svm2, "cant load xml");
    svm2->predict(crop, res);
        
        int i=0;
    std::vector<int8_t> result;
    //int result;
    std::ostringstream ss;
    
    while(i< dem){
        
        result.push_back(res.at<float>(i));
        
        ss << result[i] << " ";
        //CCLabelTTF *kien;
        this->removeChildByTag(1);
        auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
        std::string s(ss.str());
    
        label->setString(std::to_string(result[i]));
        label->setTag(1);
        label->setPosition(300 , 50);
        this->addChild(label);
        
        printf("%d", result[i]);
        printf("string:%s", s.c_str());
        i++;
    }
    
    //return s.c_str();
}
 */

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    /*auto label = Label::createWithTTF("demos!", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);*/
    
    //detect();
    
    // add "HelloWorld" splash screen"
    //_m_sprite = Sprite::create("HelloWorld.png");
    

    _m_sprite = Sprite::create("HelloWorld.png");
    
    
    svm2 = SVM::create();
    svm2 = svm2->load(FileUtils::getInstance()->fullPathForFilename("model_new.xml"));
    CCASSERT(svm2, "cant load xml");
    // position the _m_sprite on the center of the screen
    _m_sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    
    //_g_dbg_layer = DBGLayer::create();
    //_g_dbg_layer->setColor(cocos2d::Color3B::RED);
    //_g_dbg_layer->setDuration(10.0f);
    //_g_dbg_layer->setScale(0.75f);
    //addChild(_g_dbg_layer,0x1000,0x1000);
    
    
    
    //CCLOG("%s", FileUtils::getInstance()->fullPathForFilename("HelloWorld.png").data());
    
    // add the _m_sprite as a child to this layer
    //this->addChild(_m_sprite, 0);
    
    //detectNumber( img, "model.xml");
    
    //scheduleUpdate();
    schedule(schedule_selector(HelloWorld::update), 1.0f);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt)
{
    //scheduleOnce(schedule_selector(cocos2d::Scene::update), 1.0f);
    //cocos2d::Scene::update(dt);
    //printf("Kien");
    if(s_dirty)
    {
        s_dirty = false;
        std::lock_guard<std::mutex> lock(s_mtx_change_texture);
        if (_m_texture2d == nullptr)
        {
            _m_texture2d = new cocos2d::Texture2D();
            _m_texture2d->initWithData(img.data,
                                       img.elemSize() * img.cols * img.rows,
                                       cocos2d::Texture2D::PixelFormat::RGBA8888,
                                       img.cols,
                                       img.rows,
                                       cocos2d::Size(img.cols, img.rows));
            
            //_m_sprite->setTexture(_m_texture2d);
            
        }
        else
        {
            //_m_sprite->getTexture()->updateWithData(img.data, 0, 0, img.cols, img.rows);

            try
            {
                //if (detectNumber(img,FileUtils::getInstance()->fullPathForFilename("model.xml")) != NULL)
                
                //std::string s(ss.str());
                /*
                float delay = 1.0f;
                auto delayaction = DelayTime::create(delay);
                auto func = CallFunc::create([]()
                {
                    detectNumber(img,FileUtils::getInstance()->fullPathForFilename("model.xml"));
                });
                 */
                //detectNumber(img,FileUtils::getInstance()->fullPathForFilename("model.xml"));
                string result1;
                detectNumber(img, "model_new.xml", result1);
                
                //this -> runAction(Sequence::create(delayaction, func, NULL));
                    //*_g_dbg_layer << "Kien";
                //std::stringstream kien;
                //
                
                
            
            }
            catch (const std::exception &e)
            {
                //*_g_dbg_layer << std::string("ERROR") + std::string(e.what());
            }

        }
        
                //cocos2d::Rect rect = cocos2d::Rect::ZERO;
        //rect.size = _m_sprite->getTexture()->getContentSize();
        //_m_sprite->setTextureRect(rect);
        //_m_sprite->setRotation(-90);
    }
}
