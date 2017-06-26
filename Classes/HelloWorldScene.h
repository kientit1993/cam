#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    HelloWorld() = default;
    virtual ~HelloWorld()
    {
        CC_SAFE_DELETE(_m_texture2D);
        CC_SAFE_DELETE(_m_image);
    }

    static cocos2d::Scene* createScene();

    virtual bool init() override;

    virtual void update(float dt) override;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    static std::mutex s_mtx_change_texture;
    cocos2d::Sprite *_m_sprite;
    static cocos2d::Texture2D *_m_texture2D;
    cocos2d::Image *_m_image;
    static bool s_dirty;
};

#endif // __HELLOWORLD_SCENE_H__
