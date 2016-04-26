#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
private:
    float _shipPointsPerSecY;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    //Updates game
    void update(float dt);
    
    //Controls spaceship with accelerometer
    void OnAcceleration(cocos2d::Acceleration *acc,cocos2d::Event *event);
    
    //Touch function
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    
    //Spawns enemies
    void spawnEnemies(float h, float w, float numEnemy, const char * filepath);
    
    //void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    //void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
