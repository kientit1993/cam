#include "DBGLayer.h"

#include <string>

#include <cocos2d.h>

#include <cocos/base/ccFPSImages.h>


float _s_duration = 3.f;
float _s_scale = 1.f;
static cocos2d::Color3B _s_color = cocos2d::Color3B::WHITE;

bool DBGLayer::init()
{
    cocos2d::Node::init();

    _m_layer = cocos2d::Layer::create();
    _m_layer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);

    this->addChild(_m_layer, 1);

    _m_dbg_strings.reserve(0x1000);
    _m_keys.reserve(0x100);

    cocos2d::Image* _l_img = new (std::nothrow) cocos2d::Image();
    CC_SAFE_RETAIN(_l_img);
    bool isOK = _l_img->initWithImageData(cc_fps_images_png, cc_fps_images_len());
    if (! isOK) {
        CCLOGERROR("%s", "Fails: init fps_images");
        return true;
    }

    cocos2d::Texture2D *_l_text = DIRECTOR->getTextureCache()->addImage(_l_img, "/dbg_font");
    CC_SAFE_RELEASE(_l_img);

    scheduleUpdate();
    schedule(CC_SCHEDULE_SELECTOR(DBGLayer::update_schedule), 1.f / 24); // 24 fps
    return true;
}

void DBGLayer::update_schedule(float _a_dt)
{
    float _l_x;
    float _l_y = _m_layer->getContentSize().height;

    for(cocos2d::LabelAtlas *_l_label : _m_keys)
    {
        switch(std::get<2>(_m_map[_l_label]))
        {
            case DBGLayer::Type::Integer:
            {
                int const *_l_val = reinterpret_cast<int const *>(std::get<1>(_m_map[_l_label]));
                _l_label->setString(std::get<0>(_m_map[_l_label]) + std::to_string(*_l_val));
                break;
            }

            case DBGLayer::Type::Float:
            {
                float const *_l_val = reinterpret_cast<float const *>(std::get<1>(_m_map[_l_label]));
                _l_label->setString(std::get<0>(_m_map[_l_label]) + std::to_string(*_l_val));
                break;
            }

            case DBGLayer::Type::Double:
            {
                double const *_l_val = reinterpret_cast<double const *>(std::get<1>(_m_map[_l_label]));
                _l_label->setString(std::get<0>(_m_map[_l_label]) + std::to_string(*_l_val));
                break;
            }

            case DBGLayer::Type::String:
            {
                _l_label->setString( std::get<0>( _m_map[_l_label] ) + std::string( reinterpret_cast<char const *>( std::get<1>( _m_map[_l_label] ) ) ) );
                break;
            }

            case DBGLayer::Type::ccVec2:
            {
                cocos2d::Vec2 const *_l_val = reinterpret_cast<cocos2d::Vec2 const *>(std::get<1>(_m_map[_l_label]));
                _l_label->setString(std::get<0>(_m_map[_l_label]) + "(" + std::to_string(_l_val->x) + ":" + std::to_string(_l_val->y) + ")");
                break;
            }

            default:
            {
                break;
            }
        }

        _l_x = 5.f;
        _l_y -= _l_label->getContentSize().height * _l_label->getScale();
        _l_label->setPosition(_l_x, _l_y);
    }
}

void DBGLayer::update(float _a_dt)
{
    cocos2d::Node::update(_a_dt);
    static int _s_cnt = 0;
    static float _s_y = _m_layer->getContentSize().height;
    float _l_x = 0;

    for(DBGString &_l_str : _m_dbg_strings)
    {
        auto _l_label = cocos2d::LabelAtlas::create();
        _l_label->initWithString(_l_str._m_content, DIRECTOR->getTextureCache()->getTextureForKey("/dbg_font"), 12, 32 , '.');
        _l_label->setColor(_l_str._m_color);
        _l_label->setScale(_l_str._m_scale);

        _l_x = SCREEN_X(1.f) - (_l_label->getContentSize().width * _l_label->getScale() + 5.f);
        _l_label->setPosition(cocos2d::Vec2(_l_x , _s_y));

        _s_y += _l_label->getContentSize().height * _l_label->getScale();

        _m_layer->setPosition(cocos2d::Vec2(_m_layer->getPosition().x, _m_layer->getPosition().y - _l_label->getContentSize().height * _l_label->getScale()));

        _m_layer->addChild(_l_label);

        this->scheduleOnce([=](float _a_dt)
            {
                _l_label->removeFromParent();
            },
            _l_str._m_duration, "dbg_layer" + std::to_string(_s_cnt++));
    }

    _m_dbg_strings.clear();
}

DBGLayer &DBGLayer::operator << (const std::string &_a_val)
{
    _m_dbg_strings.push_back({_a_val, _s_color, _s_duration, _s_scale});
    return *this;
}

DBGLayer &DBGLayer::operator << (int _a_val)
{
    _m_dbg_strings.push_back({std::to_string(_a_val), _s_color, _s_duration, _s_scale});
    return *this;
}

DBGLayer &DBGLayer::operator << (float _a_val)
{
    _m_dbg_strings.push_back({std::to_string(_a_val), _s_color, _s_duration, _s_scale});
    return *this;
}

DBGLayer &DBGLayer::operator << (double _a_val)
{
    _m_dbg_strings.push_back({std::to_string(_a_val), _s_color, _s_duration, _s_scale});
    return *this;
}

DBGLayer &DBGLayer::operator << (const cocos2d::Vec2 &_a_val)
{
    _m_dbg_strings.push_back({"(" + std::to_string(_a_val.x) + ":" + std::to_string(_a_val.y) + ")", _s_color, _s_duration, _s_scale});
    return *this;
}

void DBGLayer::setColor(const cocos2d::Color3B &_a_color)
{
    _s_color = _a_color;
}

void DBGLayer::setDuration(float _a_duration)
{
    _s_duration = _a_duration;
}

void DBGLayer::setScale(float _a_scale)
{
    _s_scale = _a_scale;
}

void DBGLayer::push_back(const std::string &_a_str, const void *_a_val, DBGLayer::Type &&_a_type)
{
    cocos2d::LabelAtlas *_l_label_atlas = cocos2d::LabelAtlas::create();
    this->addChild(_l_label_atlas, 2);

    _l_label_atlas->initWithString(_a_str, DIRECTOR->getTextureCache()->getTextureForKey("/dbg_font"), 12, 32 , '.');

    _l_label_atlas->setColor(_s_color);
    _l_label_atlas->setScale(_s_scale);

    _m_keys.push_back(_l_label_atlas);
    _m_map[_l_label_atlas] = std::make_tuple(_a_str, _a_val, _a_type);
}
