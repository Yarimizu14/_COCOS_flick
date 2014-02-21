#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

// Pixcel to Meter : 1メータ=32pixcel
#define PTM_RATIO 32
#define WORLD_TO_SCREEN(n) ((n) * PTM_RATIO)

class Gravity : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
   
    void tick(float dt);
    void createBackground();
    void createStar(Point p);
    bool touchBegan(Touch *touch, Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Gravity);
};

#endif // __HELLOWORLD_SCENE_H__
