#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h" 
#include "RootViewController.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

class Myclass: public cocos2d::Sprite
{
public:
    virtual bool init();
    void startCapture();
    void stopCapture();
    void updateCameraTexture();
    void update (float delta);
    cocos2d::Texture2D *txture2d;
    cv::Mat img;
    CREATE_FUNC(Myclass);
};

#endif // __HELLOWORLD_SCENE_H__
