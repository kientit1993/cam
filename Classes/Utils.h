#ifndef __UTILS_H__
#define __UTILS_H__

#include <sstream>
#include <locale>
#include <cerrno>
#include <string>
#include <vector>

#include <cstring>
#include <cstdarg>

#include <sys/types.h>

#include <cocos/platform/CCPlatformConfig.h>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <process.h>
#define gettid                  GetCurrentThreadId
#else
#include <unistd.h>
#endif

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

static inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


static inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

#define BIT_GET(n)        (1 << (n))
#define BIT_SET(t, n)   (t | BIT_GET(n))
#define BIT_CLR(t, n)   (t & ~(BIT_GET(n)))

namespace custom
{
    template < typename T >
    inline std::string to_string( const T& t )
    {
        std::ostringstream stm ;
        stm << t ;
        return stm.str() ;
    }

    inline std::string &operator<<(std::string &astr, std::wstring const &awstr)
    {
        return astr = std::string(awstr.begin(), awstr.end());
    }

    inline std::wstring operator "" _ws(char const *astr, size_t)
    {
        std::string lstr(astr);
        return std::wstring(lstr.begin(), lstr.end());
    }
}





#define C3_TO_V3(c3)            (cocos2d::Vec3((c3).r, (c3).g, (c3).b))
#define C3_TO_V4(c3, a)            (cocos2d::Vec4((c3).r, (c3).g, (c3).b, a))

#define V3_TO_C3(v3)            (cocos2d::Color3B((v3).x, (v3).y, (v3).z))
#define V3_TO_C4(v3, w)            (cocos2d::Color4B((v3).x, (v3).y, (v3).z, w))

#define COLOR_CANDLE            (cocos2d::Color3B(255, 147, 41))
#define COLOR_40W_TUNGSTEN      (cocos2d::Color3B(255, 197, 143))
#define COLOR_100W_TUNGSTEN     (cocos2d::Color3B(255, 214, 170))
#define COLOR_HALOGEN           (cocos2d::Color3B(255, 214, 224))
#define COLOR_CARBON_ARC        (cocos2d::Color3B(255, 250, 244))

#define COLOR_HIGH_NOON_SUN     (cocos2d::Color3B(255, 255, 251))
#define COLOR_DIRECT_SUNLIGHT   (cocos2d::Color3B(255, 255, 255))

#define COLOR_OVERCAST_SKY      (cocos2d::Color3B(201, 216, 255))
#define COLOR_CLEAR_SKY         (cocos2d::Color3B(64, 156, 255))

#define HEX_TO_R(hex) (((hex) & 0xFF0000U) >> 16)
#define R_TO_HEX(r) (((r)&0xFF)<<16)
#define HEX_TO_G(hex) (((hex) & 0xFF00U) >> 8)
#define G_TO_HEX(g) (((g)&0xFF)<<8)
#define HEX_TO_B(hex) (((hex) & 0xFFU))
#define B_TO_HEX(b) ((b)&0xFF)
#define RGB_TO_HEX(r,g,b) ( R_TO_HEX(r) | G_TO_HEX(g) | B_TO_HEX(b) )
#define HEX_TO_C3B(hex) (cocos2d::Color3B(HEX_TO_R(hex), HEX_TO_G(hex), HEX_TO_B(hex)))

#define DIRECTOR (cocos2d::Director::getInstance())
#define RENDERER (DIRECTOR->getRenderer())

#define VISIBLE_SIZE (DIRECTOR->getVisibleSize())
#define ORIGIN (DIRECTOR->getVisibleOrigin())
#define WIN_SIZE (DIRECTOR->getWinSize())
#define WIN_SIZEPX (DIRECTOR->getWinSizeInPixels())

#define SCREEN_X(prop) ((VISIBLE_SIZE.width * (prop)) + (ORIGIN.x))
#define SCREEN_Y(prop) ((VISIBLE_SIZE.height * (prop)) + (ORIGIN.y))
#define SCREEN_SIZE(propX, propY)   cocos2d::Size(SCREEN_X(propX), SCREEN_Y(propY))
#define V2_SCREEN(propX, propY)     cocos2d::Vec2(SCREEN_X(propX), SCREEN_Y(propY))
#define V2_SCREEN_CENTER            V2_SCREEN(0.5, 0.5)

#define CONTENT_X(nodePtr, prop) ((nodePtr)->getContentSize().width * (prop))
#define CONTENT_Y(nodePtr, prop) ((nodePtr)->getContentSize().height * (prop))


#define V2_CONTENT(nodePtr, prop) cocos2d::Vec2(CONTENT_X(nodePtr, prop), CONTENT_Y(nodePtr, prop))
#define V2_CONTENT_CENTER(nodePtr) V2_CONTENT(nodePtr, 0.5)
#define V2_BOUNDING_CENTER(nodePtr) cocos2d::Vec2(BOUNDING_X(nodePtr, 0.5), BOUNDING_Y(nodePtr, 0.5))

#define SCREEN_SCALE_X(nodePtr) (SCREEN_X(1) / CONTENT_X(nodePtr, 1))
#define SCREEN_SCALE_Y(nodePtr) (SCREEN_Y(1) / CONTENT_Y(nodePtr, 1))

#define BOUNDING_X(nodePtr, prop) ((nodePtr)->getBoundingBox().size.width * (prop))
#define BOUNDING_Y(nodePtr, prop) ((nodePtr)->getBoundingBox().size.height * (prop))

#define VEC2_CHILD_POS_BY_TAG(parent, tag, func) ((parent)->func((parent)->getChildByTag(tag)->getPosition()))

#ifndef __LOGI
    #define __LOGI(format,...)            CCLOG("[I] %s[%s][%d]: " format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#ifndef __LOGD
    #define __LOGD(format,...)            CCLOG("[D] %s[%s][%d]: " format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#ifndef __LOGE
    #define __LOGE(format,...)            CCLOG("[E] %s[%s][%d]: " format " %s\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__, strerror(errno))
#endif
#ifndef __LOGT
    #define __LOGT(format,...)            CCLOG("[T'%d] %s[%s][%d]: " format "\n", gettid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#ifndef __LOGP
    #define __LOGP(format,...)            CCLOG("[P'%d] %s[%s][%d]: " format "\n", getpid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define _FILE_DBG                   PRODUCT_NAME##".txt"

#define STR_LOGI(format,...)            __toStr("[I] %s[%s][%d]: " format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define STR_LOGD(format,...)            __toStr("[D] %s[%s][%d]: " format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define STR_LOGE(format,...)            __toStr("[E] %s[%s][%d]: " format "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define STR_LOGT(format,...)            __toStr("[T'%d] %s[%s][%d]: " format "\n", gettid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define STR_LOGP(format,...)            __toStr("[P'%d] %s[%s][%d]: " format "\n", getpid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define SLIDE_LENGTH        (SCREEN_X(.25f))

#define RUNNING_SCENE       (DIRECTOR->getRunningScene())

#define RUNNING_LAYER       (RUNNING_SCENE->getChildByTag(TAG_LAYER))

#define RUNNING_NODE        (RUNNING_LAYER ? RUNNING_LAYER : RUNNING_SCENE)

#define F_IS_EQUAL(aA, aB, aC)          ((aA) > ((aB) - (aC)) && (aA) < ((aB) + (aC)))

enum
{
    TAG_LAYER = 1000,
    TAG_LAYER_AP,
    TAG_LAYER_MAIN,
    TAG_LAYER_RECORD,
    TAG_LAYER_POPUP,
    TAG_LAYER_OVERLAY,
};

enum
{
    Z_01 = 100,
    Z_02 = 200,
    Z_03 = 300,
    Z_04 = 400,
    Z_MAX = 0x7FFFFFFF
};

#define REINTERPRET_CAST(val, type) (*reinterpret_cast<type const *>(&(val)))

#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

#define PHYSBODYBYTAG(this, tag) (this)->getChildByTag(static_cast<int>(tag))->getPhysicsBody()

static inline std::string __toStr(const char *format, ...)
{
    int bufferSize = 0x100;
    char* buf = nullptr;
    va_list args;
    va_start(args, format);

    do
    {
        buf = new (std::nothrow) char[bufferSize];
        if (buf == nullptr)
            return std::string(); // not enough memory

        int ret = vsnprintf(buf, bufferSize - 3, format, args);
        if (ret < 0)
        {
            bufferSize *= 2;

            delete [] buf;
        }
        else
            break;

    } while (true);

    va_end(args);
    strcat(buf, "\0");
    return std::string(buf);
}

#define CREATE_FUNC_ARG01(__MAIN_TYPE__, __ARG_TYPE__1) \
static __MAIN_TYPE__* create(__ARG_TYPE__1 arg1) \
{ \
    __MAIN_TYPE__ *pRet = new(std::nothrow) __MAIN_TYPE__(arg1); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

#define CREATE_FUNC_ARG02(__MAIN_TYPE__, __ARG_TYPE__1, __ARG_TYPE__2) \
static __MAIN_TYPE__* create(__ARG_TYPE__1 arg1, __ARG_TYPE__2 arg2) \
{ \
    __MAIN_TYPE__ *pRet = new(std::nothrow) __MAIN_TYPE__(arg1, arg2); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

#endif // __UTILS_H__
