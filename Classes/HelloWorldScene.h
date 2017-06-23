#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h" 

#include "RootViewController.h"

#include <cstring>
#include <stdlib.h>

class MyClassImpl;
#ifdef __cplusplus
extern "C"
{
#endif
class HelloWorld : public cocos2d::Scene
{
public:
    static void donothing(cv::Mat anh1, cv::Mat anh2);
    double timesTwo(double number);
    static cocos2d::Scene* createScene();
    enum { cANSWER_TO_LIFE_THE_UNIVERSE_AND_EVERYTHING = 42 };
    virtual bool init();
    void doSomethingWithMyClass( void );
    cv::Mat img;
    cocos2d::Texture2D *txture2d;
    cocos2d::Sprite *sprite;    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    int someMethod (void *objectiveCObject, void *aParameter);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

};
#ifdef __cplusplus
}
#endif

class Myclass: public cocos2d::Sprite
{
public:
    
    virtual bool init();
    void startCapture();
    void stopCapture();
    void updateCameraTexture();
    void update (float delta);
    cocos2d::Texture2D *txture2d;
    cocos2d::Sprite *sprite;
    cv::Mat img;
    CREATE_FUNC(Myclass);
};

#endif // __HELLOWORLD_SCENE_H__
