#include "HelloWorldScene.h"

USING_NS_CC;

#define numEnemies 10

Label* label;
cocos2d::Sprite *player_laser; //Player projectile laser
bool laserOut = false;
int score, numShields = 4, enemyMoveInd = 0;
std::vector<int> movInd, indxcnt (numEnemies*5, 0);
std::vector<cocos2d::Sprite*> enemies_moving;


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

// initializes game variables and events
bool HelloWorld::init()
{
    //Indexes used for enemy movement
    movInd.push_back(1);
    movInd.push_back(3);
    movInd.push_back(2);
    movInd.push_back(3);
    
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
    label->setPosition(Vec2(origin.x + 50,
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
        shield->setPosition(visibleSize.width - visibleSize.width/5 * i - 20,visibleSize.height * 0.20);
        shields.push_back(shield);
        this->addChild(shield, 0);
    }
    
    //Add 5 rows of enemies
    char const * fp1 = "res/Enemies/invaderCframe1.png";
    char const * fp2 = "res/Enemies/invaderBframe1.png";
    char const * fp3 = "res/Enemies/invaderAframe1.png";
    spawnEnemies(visibleSize.width/2, visibleSize.height/2, numEnemies, fp1);
    spawnEnemies(visibleSize.width/2 - 20, visibleSize.height/2, numEnemies, fp2);
    spawnEnemies(visibleSize.width/2 - 40, visibleSize.height/2, numEnemies, fp2);
    spawnEnemies(visibleSize.width/2 - 60, visibleSize.height/2, numEnemies, fp3);
    spawnEnemies(visibleSize.width/2 - 80, visibleSize.height/2, numEnemies, fp3);
    
    enemies_moving = enemies;
    
    int c = 0;
    for(unsigned i = 0; i < numEnemies; i++){
        if(c == numEnemies-1)
            c = 0;
        indxcnt.at(i) = c;
        c++;
    }
    //Move enemy
    //moveEnemy(2);

    //Enable and set listener for accelerometer
    Device::setAccelerometerEnabled(true);
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::OnAcceleration, this));
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Enable touch input and set listener
    auto listener2 = EventListenerTouchOneByOne::create();
    listener2->setSwallowTouches(true);
    listener2->onTouchBegan =CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
    
    //Add update function to scheduler
    this->schedule(schedule_selector(HelloWorld::update));
    
    return true;
}

void HelloWorld::OnAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float x = spaceship->getPosition().x; //current x position of spaceship
    
    //Increase the position of x
    x += acc->x * visibleSize.width * 0.05;
    
    //Collision detection with left and right x borders
    if((x < visibleSize.width-20) && (x >= 0))
        spaceship->setPosition(x,visibleSize.height*.1);
    
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    
    //Checks if there is a laser already out, 1 laser at a time
    if(!laserOut){
        laserOut = true;
        Size visibleSize = Director::getInstance()->getVisibleSize();
        player_laser = Sprite::create("res/Player/player_missle.png");
        player_laser->setPosition(spaceship->getPosition().x + spaceship->getContentSize().width/2,
                                  spaceship->getPosition().y + spaceship->getContentSize().height);
        this->addChild(player_laser);
        auto action = MoveTo::create(0.7, Vec2(spaceship->getPosition().x, visibleSize.height + 50));
        auto callback = CallFunc::create( CC_CALLBACK_0( HelloWorld::setLaser, this ) );
        auto sequence = Sequence::create(action, callback, NULL);
        sequence->setTag(21);
        player_laser->runAction(sequence);
    }

    return true;
}

//Updates various events in the game
void HelloWorld::update(float dt){
    
    //Move enemies
    if(enemies.at(enemies.size()-1)->getNumberOfRunningActions() < 1){
        
        moveEnemy(movInd.at(enemyMoveInd));
        CCLOG("ACTIONS: %i: ", movInd.at(enemyMoveInd));
        enemyMoveInd++;
        if(enemyMoveInd == movInd.size())
            enemyMoveInd = 0;
    }
     
    
    
    //Player laser collision
    if(laserOut){ //Checks if a laser is currently out
        Rect laserBoundBox = Rect(player_laser->getPosition().x - (player_laser->getContentSize().width/2),
                                  player_laser->getPosition().y - (player_laser->getContentSize().height/2),
                                  player_laser->getContentSize().width,
                                  player_laser->getContentSize().height);
    
        //Collision with shields
        for(unsigned i = 0; i < numShields; i++){
            Rect shieldToCheck = Rect(shields.at(i)->getPosition().x,
                                      shields.at(i)->getPosition().y,
                                      shields.at(i)->getContentSize().width,
                                      shields.at(i)->getContentSize().height);
            
            //If intersection found, delete shield
            if(laserBoundBox.intersectsRect(shieldToCheck)){
                numShields--;
                laserOut = false;
                player_laser->stopActionByTag(21);
                this->removeChild(player_laser,true);
                this->removeChild(shields.at(i), true);
                shields.erase(shields.begin()+i);
                break;
            }
        }
        
        //Collision with enemies
        for(unsigned i = 0; i < enemies.size(); i++){
            Rect enemyToCheck = Rect(enemies.at(i)->getPosition().x,
                                     enemies.at(i)->getPosition().y,
                                     enemies.at(i)->getContentSize().width,
                                     enemies.at(i)->getContentSize().height);
            
            //If intersection found, change texture of sprite and 'animate' death with fade
            if(laserBoundBox.intersectsRect(enemyToCheck)){
                FadeOut *fade = FadeOut::create(0.3);
                auto callback = CallFunc::create( CC_CALLBACK_0( HelloWorld::removeEnemy, this, enemies.at(i), i));
                auto sequence = Sequence::create(fade, callback, NULL);
                enemies.at(i)->setTexture("res/Enemies/invader_explosion.png");
                enemies.at(i)->runAction(sequence);
                laserOut = false;
                player_laser->stopActionByTag(21);
                this->removeChild(player_laser,true);
                score += 10;
                break;
            }
        }
    }
    
    //Update score
    label->setString("SCORE: " + std::to_string(score));
}

//Spawns enemy given height, width, number of enemies and the filepath of the picture
void HelloWorld::spawnEnemies(float h, float w, float numEnemy, const char * filepath){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //Loops through number of enemies (per row) and spawns them
    for(int i = 1; i <= numEnemy; i++) {
        cocos2d::Sprite* enemy = Sprite::create(filepath);
        enemy->setAnchorPoint(Vec2(0,0));
        enemy->setPosition(enemy->getBoundingBox().size.width + (30*i) + visibleSize.width/4,h);
        this->addChild(enemy, 0);
        enemies.push_back(enemy);
    }
}

//Deletes laser and sets bool for next laser
void HelloWorld::setLaser(){
    this->removeChild(player_laser,true);
    laserOut = false;
}

//Used in a sequence after FadeOut is called
void HelloWorld::removeEnemy(Sprite* s, int i){
    enemies_moving.at(i) = NULL;
    this->removeChild(s,true);
    enemies.erase(enemies.begin()+i);
}

//Controls enemy movement, takes in type argument, 1=left, 2=right, 3=down
void HelloWorld::moveEnemy(int type){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int seperator = 0;
    if(type == 1){
        for(unsigned i = 0; i < enemies.size(); i++){
            if((i % numEnemies) == 0)
                seperator = 0;
            if(enemies.at(i) != NULL){ //if not dead enemies
                auto action = MoveTo::create(10, Vec2(0 + (30*seperator),enemies.at(i)->getPosition().y));
                auto sequence = Sequence::create(action, NULL);
                enemies.at(i)->runAction(sequence);
            }
            seperator++;
            
        }
    }
    else if(type == 2){
        for(unsigned i = 0; i < enemies.size(); i++){
            if((i % numEnemies) == 0)
                seperator = 0;
            if(enemies.at(i) != NULL){ //if not dead enemies
                auto action = MoveTo::create(10, Vec2(visibleSize.width + (30*seperator) - visibleSize.width/2,enemies.at(i)->getPosition().y));
                auto sequence = Sequence::create(action, NULL);
                enemies.at(i)->runAction(sequence);
            }
            seperator++;
        }
    }
    else if(type == 3){
        for(unsigned i = 0; i < enemies.size(); i++){
            if((i % numEnemies) == 0)
                seperator = 0;
            if(enemies.at(i) != NULL){ //if not dead enemies
                auto action = MoveTo::create(2, Vec2(enemies.at(i)->getPosition().x,
                                                 enemies.at(i)->getPosition().y - enemies.at(i)->getContentSize().height));
                auto sequence = Sequence::create(action, NULL);
                enemies.at(i)->runAction(sequence);
            }
            seperator++;
        }
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
