#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

std::mutex HelloWorld::s_mtx_change_texture;
cocos2d::Texture2D *HelloWorld::_m_texture2D = nullptr;
bool HelloWorld::s_dirty = false;


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

    auto label = Label::createWithTTF("damn!", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    _m_sprite = Sprite::create("HelloWorld.png");
    
    // position the _m_sprite on the center of the screen
    _m_sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the _m_sprite as a child to this layer
    this->addChild(_m_sprite, 0);

    scheduleUpdate();

    /*static bool l_is_running = true;
    static std::thread l_th_process_image([=]()
        {
            _m_image = new cocos2d::Image();
            _m_texture2D = new cocos2d::Texture2D();

            while(l_is_running)
            {
                {
                    std::lock_guard<std::mutex> lock(s_mtx_change_texture);
                    // CCLOG("Changed");
                    // request update texture
                    s_dirty = true;
                }
                // sleep 33 ms. Equivalent with 30 fps
                std::this_thread::sleep_for(std::chrono::milliseconds(33));

            }

        });

    auto l_listener = EventListenerCustom::create("director_reset", [&](EventCustom* event){
        CCLOG("director_reset");
        l_is_running = false;
        l_th_process_image.join();
    });

    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(l_listener, 1);*/

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

void HelloWorld::update(float dt)
{
    cocos2d::Scene::update(dt);

    if(s_dirty)
    {
        s_dirty = false;
        std::lock_guard<std::mutex> lock(s_mtx_change_texture);
        //_m_texture2D->initWithImage(_m_image);
        _m_sprite->setTexture(_m_texture2D);

        Rect rect = Rect::ZERO;
        rect.size = _m_sprite->getTexture()->getContentSize();
        _m_sprite->setTextureRect(rect);
    }
}
