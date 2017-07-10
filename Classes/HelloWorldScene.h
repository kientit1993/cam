#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <opencv2/imgproc/imgproc.hpp>

class HelloWorld : public cocos2d::Scene
{
public:
    HelloWorld() = default;
    virtual ~HelloWorld()
    {
        
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
    cocos2d::Texture2D *_m_texture2d;
    static bool s_dirty;
    static cv::Mat img;
};

#endif // __HELLOWORLD_SCENE_H__
