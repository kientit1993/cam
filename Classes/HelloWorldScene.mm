#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CppWrapper.h"

USING_NS_CC;
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    CCLOG("Kientit");
    scene->addChild(layer);
    return scene;
    
}

void HelloWorld::donothing(cv::Mat anh1, cv::Mat anh2)
{
    anh1 = anh2.clone();
    CCLOG("kick thuoc anh: %d", anh1.cols );
    //cocos2d::Texture2D *txture2d = new Texture2D();
    /*txture2d->initWithData(anh1.data,
                            anh1.elemSize() * anh1.cols * anh1.rows,
                            Texture2D::PixelFormat::RGB888,
                            anh1.cols,
                            anh1.rows,
                            cocos2d::Size(anh1.cols, anh1.rows));
    auto sprite = Sprite::create();
    sprite->setTexture(txture2d);
    sprite->setTextureRect(cocos2d::Rect(0, 0, 160, 320));
    //addChild(sprite, 0);*/
}

bool HelloWorld::init()
{
    //Myclass* myclass;
    //myclass->startCapture();
    //RootViewController *rootview;
    //img = rootview->img;
    //CCLOG("Characters: %d", img.cols );
    //txture2d->initWithData(rootview->img.data,
    //rootview->img.elemSize() * rootview->img.cols * rootview->img.rows,
    //Texture2D::PixelFormat::RGB888,
    //rootview->img.cols,
    //rootview->img.rows,
    //cocos2d::Size(rootview->img.cols, rootview->img.rows));
    return true;
}

bool Myclass::init()
{
    //txture2d = new Texture2D();
    //auto scene = Director::getInstance()->getRunningScene();
    //auto coin = Sprite::createWithSpriteFrameName("coin-01.png");
    //scene->addChild(coin);    //sprite = Sprite::create();
    //sprite->initWithTexture(txture2d);
    //scheduleUpdate();
    //return true;
}


void Myclass::startCapture()
{
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    //auto origin = Director::getInstance()->getVisibleOrigin();
    //sprite->create("HelloWorld.png");
    //auto spritex = Sprite::create("HelloWorld.png");
    // Đặt sprite lên vị trí giữa màn hình
    //sprite->setPosition(cocos2d::Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //auto scene = Director::getInstance()->getRunningScene();
    // Thêm đối tượng Sprite vào HelloWorld
    //this->addChild(sprite, 0);
    
    //RootViewController *rootview;
    //img = rootview->img.clone();
    //auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
    //label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    
}

void Myclass::update(float delta)
{
    //Sprite::update(delta);
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    //auto origin = Director::getInstance()->getVisibleOrigin();
    //sprite->create("HelloWorld.png");
    //auto spritex = Sprite::create("HelloWorld.png");
    // Đặt sprite lên vị trí giữa màn hình
    //sprite->setPosition(cocos2d::Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //auto scene = Director::getInstance()->getRunningScene();
    // Thêm đối tượng Sprite vào HelloWorld
    //this->addChild(sprite, 0);
    //updateCameraTexture();
}

void Myclass::updateCameraTexture()
{
    //txture2d->initWithData(img.data,
                         //img.elemSize() * img.cols * img.rows,
                         //Texture2D::PixelFormat::RGB888,
                         //img.cols,
                         //img.rows,
                         //cocos2d::Size(img.cols, img.rows));
    //sprite->setTexture(txture2d);
    //sprite->setTextureRect(cocos2d::Rect(0, 0, 160, 320));
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    //auto origin = Director::getInstance()->getVisibleOrigin();
    //sprite->create("HelloWorld.png");
    //auto spritex = Sprite::create("HelloWorld.png");
    // Đặt sprite lên vị trí giữa màn hình
    //sprite->setPosition(cocos2d::Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //auto scene = Director::getInstance()->getRunningScene();
    // Thêm đối tượng Sprite vào HelloWorld
    //this->addChild(sprite, 0);

}
void Myclass::stopCapture()
{
    //this->unscheduleUpdate();
}
