#ifndef __DBGLAYER_H__
#define __DBGLAYER_H__

#include <string>
#include <vector>
#include <unordered_map>
#include "Utils.h"


#include <cocos2d.h>


class DBGLayer : public cocos2d::Node
{
public:
    struct DBGString
    {
        std::string _m_content;
        cocos2d::Color3B _m_color;
        float _m_duration;
        float _m_scale;
    };

    enum class Type
    {
        Integer,
        Float,
        Double,
        String,
        ccVec2,
    };

    CREATE_FUNC(DBGLayer);

    virtual void update(float _a_dt) override;

    virtual DBGLayer &operator << (const std::string &_a_val);
    virtual DBGLayer &operator << (int _a_val);
    virtual DBGLayer &operator << (float _a_val);
    virtual DBGLayer &operator << (double _a_val);
    virtual DBGLayer &operator << (const cocos2d::Vec2 &_a_val);

    virtual void setColor(const cocos2d::Color3B &_a_color);
    virtual void setDuration(float _a_duration);
    virtual void setScale(float _a_scale);

    virtual void push_back(const std::string &_a_str, const void *_a_val, DBGLayer::Type &&_a_type);

    virtual void update_schedule(float _a_dt);

protected:

    virtual bool init() override;

public:

    cocos2d::Layer *_m_layer;
    std::vector<DBGString> _m_dbg_strings;
    std::vector<cocos2d::LabelAtlas*> _m_keys;
    std::unordered_map<cocos2d::LabelAtlas*, std::tuple<std::string, const void *, DBGLayer::Type> > _m_map;
};

#endif // __DBGLAYER_H__
