#include "HelloWorldScene.h"

USING_NS_CC;

bool moveLeft, moveRight; //variable to check if user 
cocos2d::Sprite *spaceship;
cocos2d::Sprite *shield1, *shield2, *shield3, *shield4;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
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
    
    
    auto label = Label::createWithTTF("SCORE: ", "fonts/space_invaders.ttf", 14);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    //Add initial position of spaceship
    spaceship = Sprite::create("res/Player/spaceship.png");
    spaceship->setAnchorPoint(Vec2(0,0));
    spaceship->setPosition(visibleSize.width/2,visibleSize.height * .1);
    this->addChild(spaceship, 0);
    
    //Add initial position of the four shields
    shield1 = Sprite::create("res/Player/player_shield.png");
    shield1->setAnchorPoint(Vec2(0,0));
    shield1->setPosition(visibleSize.width - visibleSize.width/5,visibleSize.height * .20);
    this->addChild(shield1, 0);
    
    shield2 = Sprite::create("res/Player/player_shield.png");
    shield2->setAnchorPoint(Vec2(0,0));
    shield2->setPosition(visibleSize.width - (visibleSize.width/5 * 2),visibleSize.height * .20);
    this->addChild(shield2, 0);

    shield3 = Sprite::create("res/Player/player_shield.png");
    shield3->setAnchorPoint(Vec2(0,0));
    shield3->setPosition(visibleSize.width - (visibleSize.width/5 * 3),visibleSize.height * .20);
    this->addChild(shield3, 0);

    shield4 = Sprite::create("res/Player/player_shield.png");
    shield4->setAnchorPoint(Vec2(0,0));
    shield4->setPosition(visibleSize.width - (visibleSize.width/5 * 4),visibleSize.height * .20);
    this->addChild(shield4, 0);
    
    //Enable and set listener for accelerometer
    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Enable touch input
    auto listener2 = EventListenerTouchOneByOne::create();
    listener2->setSwallowTouches(true);
    listener2->onTouchBegan =CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    //listener2->onTouchMoved =CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    //listener2->onTouchEnded =CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
    
    return true;
}

void HelloWorld::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float x = spaceship->getPosition().x; //current x position of spaceship
    
    //Increase the position of x
    x += acc->x * visibleSize.width * 0.1;
    
    //Collision detection with left and right x borders
    if((x < visibleSize.width-20) && (x >= 0))
        spaceship->setPosition(x,visibleSize.height*.1);
    
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    //CCLOG("ON TOUCH: x=%f, y=%f", touch->getLocatison().x, touch->getLocation().y);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* laser = Sprite::create("res/Player/player_missle.png");
    laser->setPosition(spaceship->getPosition());
    this->addChild(laser);
    laser->runAction(MoveTo::create(0.4, Vec2(spaceship->getPosition().x, visibleSize.height + 50)));

    return true;
}

void HelloWorld::update(float dt){

}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
