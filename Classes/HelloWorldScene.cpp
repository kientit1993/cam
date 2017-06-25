#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

cocos2d::Sprite* HelloWorld::sprite = nullptr;
cv::Mat MyClass::img;
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

Sprite* MyClass::createSprite()
{
    return MyClass::create();
}
void MyClass::startCap(cv::Mat image)
{
    img = image.clone();
}

void MyClass::UpdateCamtxture()
{
    
    txture->initWithData(img.data,
                          img.elemSize() * img.cols * img.rows,
                          cocos2d::Texture2D::PixelFormat::RGB888,
                          img.cols,
                          img.rows,
                          cocos2d::Size(img.cols, img.rows));
    HelloWorld::sprite->setTexture(MyClass::txture);
    if (HelloWorld::sprite == nullptr)
    {
        CCLOG("sprite null");
    }
    CCLOG("c++ image: %d", img.cols);
    HelloWorld::sprite->setTextureRect(cocos2d::Rect(160,160,256,256));
}

void MyClass::stopCap()
{
    unscheduleUpdate();
}

void MyClass::update(float delta)
{
    Sprite::update(delta);
    UpdateCamtxture();
}

bool MyClass::init()
{
    HelloWorld::sprite->createWithTexture(MyClass::txture);
    CCLOG("Kien");
    this->scheduleUpdate();
    return true;
}

void HelloWorld::setTexture(cocos2d::Texture2D *texture)
{
    sprite->setTexture(texture);
    Rect rect = Rect::ZERO;
    rect.size = texture->getContentSize();
    sprite->setTextureRect(rect);
    CCLOG("rect size = %f %f", rect.size.height, rect.size.width);
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
    /*
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
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    //position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            //origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);*/
    MyClass* myclass = new MyClass();
    
    //sprite = cocos2d::Sprite::create();
    // add "HelloWorld" splash screen"   
    // position the sprite on the center of the screen
    //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(myclass, 0);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
