#include "Gravity.h"

b2World* world;
b2Vec2 grav;

Scene* Gravity::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Gravity::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Gravity::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Gravity::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    // x方向0.0f, Y方向9.8meter
    grav.Set(0.0f, -WORLD_TO_SCREEN(9.8));
    bool doSleep = true;
   
    world = new b2World(grav);
    world->SetAllowSleeping(doSleep);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    // CC_CALLBACK_2の2はいくつ引数をとるか
    touchListener->onTouchBegan = CC_CALLBACK_2(Gravity::touchBegan, this);
   
    getEventDispatcher()->addEventListenerWithFixedPriority(touchListener, 100);
    
    schedule(schedule_selector(Gravity::tick));
    
    return true;
}

bool Gravity::touchBegan(Touch* touch, Event* event) {
    
    Gravity::createStar(touch->getLocation());
   
    return true;
}

void Gravity::createStar(Point p) {

    auto starSprite = Sprite::create("star.png");
    starSprite->setPosition(p);

    this->addChild(starSprite,0);
    
    b2BodyDef starBodyDef;
    starBodyDef.type = b2_dynamicBody;
    starBodyDef.position = b2Vec2(starSprite->getPositionX(), starSprite->getPositionY());
    starBodyDef.userData = starSprite;
    
    auto starBody = world->CreateBody(&starBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = WORLD_TO_SCREEN(0.6);
    
    b2FixtureDef starFixtureDef;
    starFixtureDef.shape = &circle;
    starFixtureDef.density = 1.0f;
    starFixtureDef.friction = 0.6f;
    starFixtureDef.restitution = 0.8f;
    
    starBody->CreateFixture(&starFixtureDef);
}

void Gravity::tick(float dt) {
    int velocityIteration = 8;
    int positionIteration = 3;

    world->Step(dt, velocityIteration, positionIteration);
    
    log("====");
    for (auto b = world->GetBodyList(); b; b = b->GetNext()) {
        if (b->GetUserData() != NULL) {
            auto myActor = (Sprite*) b->GetUserData();
            myActor->setPosition(Point(b->GetPosition().x, b->GetPosition().y));
            
            log("Position: %f, %f", b->GetPosition().x, b->GetPosition().y);
            
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
            if (myActor->getPositionY() <  - WORLD_TO_SCREEN(1)
                || myActor->getPositionX() < - WORLD_TO_SCREEN(1)
                || myActor->getPositionX() > Director::getInstance()->getVisibleSize().width + WORLD_TO_SCREEN(1)) {
                    removeChild(myActor);
                    world->DestroyBody(b);
            }
        }
    }
}

void Gravity::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
