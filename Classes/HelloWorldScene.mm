#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
    
}

bool HelloWorld::init()
{
    Myclass* myclass;
    myclass->startCapture();
    //addChild(myclass, 0);
    return true;
}

bool Myclass::init()
{
    txture2d = new Texture2D();
    auto sprite = Sprite::create();
    sprite->initWithTexture(txture2d);
    return true;
}


void Myclass::startCapture()
{   RootViewController *rootview;
    img = rootview->img.clone();
    this->scheduleUpdate();
}

void Myclass::update(float delta)
{
    Sprite::update(delta);
    updateCameraTexture();
}

void Myclass::updateCameraTexture()
{
    txture2d->initWithData(img.data,
                         img.elemSize() * img.cols * img.rows,
                         Texture2D::PixelFormat::RGB888,
                         img.cols,
                         img.rows,
                         cocos2d::Size(img.cols, img.rows));
    auto sprite = Sprite::create();
    sprite->setTexture(txture2d);
    sprite->setTextureRect(cocos2d::Rect(0, 0, 160, 320));    
}
void Myclass::stopCapture()
{
    this->unscheduleUpdate();
}
