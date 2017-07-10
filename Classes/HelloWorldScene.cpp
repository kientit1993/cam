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

using namespace std;
using namespace cv::ml;
using namespace cv;
USING_NS_CC;

std::mutex HelloWorld::s_mtx_change_texture;
bool HelloWorld::s_dirty = false;
cv::Mat HelloWorld::img;

// comparison function object

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
Mat equalizeBGRA(const Mat& img)
{
    Mat res(img.size(), img.type());
    Mat imgB(img.size(), CV_8UC1);
    Mat imgG(img.size(), CV_8UC1);
    Mat imgR(img.size(), CV_8UC1);
    Vec4b pixel;
    
    if (img.channels() != 4)
    {
        cout << "ERROR: image input is not a BGRA image!" << endl;
        return Mat();
    }
    
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            pixel = img.at<Vec4b>(r, c);
            imgB.at<uchar>(r, c) = pixel[0];
            imgG.at<uchar>(r, c) = pixel[1];
            imgR.at<uchar>(r, c) = pixel[2];
        }
    }
    
    equalizeHist(imgB, imgB);
    equalizeHist(imgG, imgG);
    equalizeHist(imgR, imgR);
    
    for (int r = 0; r < img.rows; r++)
    {
        for (int c = 0; c < img.cols; c++)
        {
            pixel = Vec4b(imgB.at<uchar>(r, c), imgG.at<uchar>(r, c), imgR.at<uchar>(r, c), img.at<Vec4b>(r, c)[3]);
            res.at<Vec4b>(r, c) = pixel;
        }
    }
    
    return res;
}

vector<cv::Point> contoursConvexHull(vector<vector<cv::Point>> contours)
{
    vector<cv::Point> result;
    vector<cv::Point> pts;
    for (size_t i = 0; i< contours.size(); i++)
        for (size_t j = 0; j< contours[i].size(); j++)
            pts.push_back(contours[i][j]);
    convexHull(pts, result);
    return result;
}

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

    auto label = Label::createWithTTF("demos!", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    //_m_sprite = Sprite::create("HelloWorld.png");
    

    _m_sprite = Sprite::create("HelloWorld.png");
    
    // position the _m_sprite on the center of the screen
    _m_sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the _m_sprite as a child to this layer
    this->addChild(_m_sprite, 0);

    scheduleUpdate();
    
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
    cocos2d::Scene::update(dt);

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
            
            _m_sprite->setTexture(_m_texture2d);
            /*
            if (img.cols>2000 || img.rows>2000){
                resize(img, img, cvSize(img.cols / 2, img.rows / 2), CV_INTER_LINEAR);
                
            }
            if (img.cols>3000 || img.rows>3000){
                resize(img, img, cvSize(img.cols / 4, img.rows / 4), CV_INTER_LINEAR);
                
            }
            
            Mat imgx;
            img.copyTo(imgx);
            cvtColor(imgx, imgx, CV_RGB2GRAY);
            Mat img2;
            
            bilateralFilter(imgx, img2, 11, 13, 13);
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
            cv::Mat crop(1, 784, CV_32FC1);
            
            cv::Ptr<SVM> svm2 = SVM::create();
            
            svm2 = svm2->load("E:\\Doan\\opencv_native_androidstudio-master\\model.xml");// duong dan den file model model.xml
            for (int i = 0; i < contours.size(); i++){
                int peri = cv::arcLength(contours.at(i), true);
                vector<cv::Point> approx;
                approxPolyDP(contours.at(i), approx, 0.02*peri, true);
                Mat imgx2;
                Vec<int, 4> hi = hierarchy.at(i);
                cv::Rect r = boundingRect(contours.at(i));
                
                if (approx.size() == 4 && hi[3] != -1 && r.height>28){
                    int child_idx = hi[2];
                    vector<cv::Point> child_c = contours.at(child_idx);
                    cv::Rect r1 = boundingRect(child_c);
                    
                    Mat croppedImage;
                    Mat ROI(img, r1);
                    ROI.copyTo(cr);
                    
                    
                    Mat croop;
                    cv::resize(cr, croop, CvSize(28, 28), 0, 0, CV_INTER_LINEAR);
                    Mat bina;
                    cvtColor(croop, bina, CV_RGB2GRAY);
                    cv::threshold(bina, bina, 200, 255, THRESH_BINARY_INV);
                    imshow("hh", bina);
                    waitKey();
                    int ii = 0;
                    for (int x = 0; x<bina.rows; x++) {
                        for (int y = 0; y<bina.cols; y++) {
                            crop.at<float>(1, ii++) = (float)bina.at<uchar>(x, y);//- 255.0/2)/255.0;
                            //cout << data.at<double>(i, x * 28 + y) << "---" << i << "---" << (x * 28 + y) << endl;
                            
                            
                        }
                    }
                    
                    cv::Mat res;
                    svm2->predict(crop, res);
                    cout << res << endl;
                    
                }
                
            }*/
            
            
        }
        else
        {
            _m_sprite->getTexture()->updateWithData(img.data, 0, 0, img.cols, img.rows);
            
            if (img.cols>2000 || img.rows>2000)
            {
                resize(img, img, cvSize(img.cols / 2, img.rows / 2), CV_INTER_LINEAR);
                
            }
            if (img.cols>3000 || img.rows>3000)
            {
                resize(img, img, cvSize(img.cols / 4, img.rows / 4), CV_INTER_LINEAR);
                
            }
            
            Mat imgx;
            img.copyTo(imgx);
            cvtColor(imgx, imgx, CV_RGB2GRAY);
            Mat img2;
            
            bilateralFilter(imgx, img2, 11, 13, 13);
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
            cv::Mat crop(1, 784, CV_32FC1);
            
            cv::Ptr<SVM> svm2 = SVM::create();
            
             svm2 = Algorithm::load<SVM>("model.xml");// duong dan den file model model.xml
             for (int i = 0; i < contours.size(); i++)
             {
                 int peri = cv::arcLength(contours.at(i), true);
                 vector<cv::Point> approx;
                 approxPolyDP(contours.at(i), approx, 0.02*peri, true);
                 Mat imgx2;
                 Vec<int, 4> hi = hierarchy.at(i);
                 cv::Rect r = boundingRect(contours.at(i));
             
                 if (approx.size() == 4 && hi[3] != -1 && r.height>28)
                 {
                     int child_idx = hi[2];
                     vector<cv::Point> child_c = contours.at(child_idx);
                     cv::Rect r1 = boundingRect(child_c);
             
                     Mat croppedImage;
                     Mat ROI(img, r1);
                     ROI.copyTo(cr);
             
                     Mat croop;
                     cv::resize(cr, croop, CvSize(28, 28), 0, 0, CV_INTER_LINEAR);
                     Mat bina;
                     cvtColor(croop, bina, CV_RGB2GRAY);
                     cv::threshold(bina, bina, 200, 255, THRESH_BINARY_INV);
                     imshow("hh", bina);
                     waitKey();
                     int ii = 0;
                     for (int x = 0; x<bina.rows; x++)
                     {
                         for (int y = 0; y<bina.cols; y++)
                         {
                             crop.at<float>(1, ii++) = (float)bina.at<uchar>(x, y);//- 255.0/2)/255.0;
                             //cout << data.at<double>(i, x * 28 + y) << "---" << i << "---" << (x * 28 + y) << endl;
             
             
                         }
                     }
                     cv::Mat res;
                     svm2->predict(crop, res);
                     cout << res << endl;
                 }
                 
             }
        }

        cocos2d::Rect rect = cocos2d::Rect::ZERO;
        rect.size = _m_sprite->getTexture()->getContentSize();
        _m_sprite->setTextureRect(rect);
        _m_sprite->setRotation(-90);
    }
}
