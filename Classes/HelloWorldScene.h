#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <opencv2/imgproc/imgproc.hpp>

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    static cocos2d::Sprite* sprite;
    
    static void setTexture(cocos2d::Texture2D *texture);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

class MyClass : public cocos2d::Sprite
{
public:
    static cocos2d::Sprite* createSprite();
    virtual bool init() override;
    cocos2d::Texture2D *txture;
    static cv::Mat img;
    static void startCap(cv::Mat image);
    void stopCap();
    void UpdateCamtxture();
    
    void update(float) override;
};

#endif // __HELLOWORLD_SCENE_H__
