#include "HelloWorldScene.h"

USING_NS_CC;

#define numShields 4
#define numEnemies 10

Label* label;
cocos2d::Sprite *spaceship; //Spaceship of laser
std::vector<Sprite*> shields;
std::vector<Sprite*> enemies;
cocos2d::Sprite *player_laser; //Player projectile laser
int score;

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
    // create and initialize a label
    score = 0;
    
    label = Label::createWithTTF("SCORE: " + std::to_string(score), "fonts/space_invaders.ttf", 14);
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + 40,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
    
    //Add initial position of spaceship
    spaceship = Sprite::create("res/Player/spaceship.png");
    spaceship->setAnchorPoint(Vec2(0,0));
    spaceship->setPosition(visibleSize.width/2,visibleSize.height * .1);
    this->addChild(spaceship, 0);
    
    //Add initial position of the four shields
    for(unsigned i = 1; i < numShields + 1; i++){
        cocos2d::Sprite* shield = Sprite::create("res/Player/player_shield.png");
        shield->setAnchorPoint(Vec2(0,0));
        shield->setPosition(visibleSize.width - visibleSize.width/5 * i+-20,visibleSize.height * .20);
        shields.push_back(shield);
        this->addChild(shield, 0);
    }
    
    //Add enemies
    char const * fp = "res/Enemies/invaderCframe1.png";
    spawnEnemies(visibleSize.width/2, visibleSize.height/2, numEnemies, fp);
    
    //Enable and set listener for accelerometer
    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Enable touch input
    auto listener2 = EventListenerTouchOneByOne::create();
    listener2->setSwallowTouches(true);
    listener2->onTouchBegan =CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
    
    //Update
    this->schedule(schedule_selector(HelloWorld::update));
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
    //if(!laserPass){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Sprite* laser = Sprite::create("res/Player/player_missle.png");
    laser->setPosition(spaceship->getPosition().x + spaceship->getContentSize().width/2,
                       spaceship->getPosition().y + spaceship->getContentSize().height);
    this->addChild(laser);
    laser->runAction(MoveTo::create(0.4, Vec2(spaceship->getPosition().x, visibleSize.height + 50)));
    score++;
    //}

    return true;
}

void HelloWorld::update(float dt){
    //Laser collision
    label->setString("SCORE: " + std::to_string(score));
    //if(laser->getPosition)
}

void HelloWorld::spawnEnemies(float h, float w, float numEnemy, const char * filepath){
    /*
    cocos2d::Sprite* enemy = Sprite::create(filepath);
    cocos2d::Sprite* enemy2 = Sprite::create(filepath);
    //enemy->setAnchorPoint(Vec2(0,0));
    enemy->setPosition(0,h);
    enemy->setAnchorPoint(Vec2(0,0));
    
    
    enemy2->setPosition(enemy2->getBoundingBox().size.width + 10,h);
    enemy2->setAnchorPoint(Vec2(0,0));
    //enemy->runAction(MoveTo::create(8, Vec2(enemy->getPositionX(), -100)));
    this->addChild(enemy, 0);
    this->addChild(enemy2,0);
    */
    
    for(int i = 0; i < numEnemy; i++) {
        cocos2d::Sprite* enemy = Sprite::create(filepath);
        //enemy->setAnchorPoint(Vec2(0,0));
        enemy->setPosition(i*enemy->getBoundingBox().size.width + w,h);
        enemy->setAnchorPoint(Vec2(0,0));
        //enemy->runAction(MoveTo::create(8, Vec2(enemy->getPositionX(), -100)));
        this->addChild(enemy, 0);
    }
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
