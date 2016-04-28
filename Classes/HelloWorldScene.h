#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
protected:
    cocos2d::Sprite *spaceship; //Spaceship of laser
    std::vector<cocos2d::Sprite*> enemies;
    std::vector<cocos2d::Sprite*> shields;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    //Updates various events in the game
    void update(float dt);
    
    //Controls spaceship with accelerometer
    void OnAcceleration(cocos2d::Acceleration *acc,cocos2d::Event *event);
    
    //Touch function, whenever a user touches the screen spaceship shoots a laser
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

protected:
    //Spawns enemies
    void spawnEnemies(float h, float w, float numEnemy, const char * filepath);
    
    //Used to control the number of lasers produced
    void setLaser();

    //Displays enemy death animation
    void removeEnemy(cocos2d::Sprite* s, int i);
    
    //Controls enemy movement, takes in type argument, 1=left, 2=right, 3=down
    void moveEnemy(int type);
    
    //Controls direction of enemy movment
    void changeDirection(int type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
