//
//  MainScene3D.cpp
//  wallmm
//
//  Created by YanChunGuang on 16/6/15.
//
//

#include "MainScene3D.h"

#include "3d/CCTerrain.h"
#include "3d/CCBundle3D.h"
#include "physics3d/CCPhysics3D.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
#include "extensions/cocos-ext.h"

USING_NS_CC_EXT;



bool MainScene3D::init()
{
    if (!Scene::init()) return false;
    
    if (initWithPhysics())
    {
        getPhysics3DWorld()->setDebugDrawEnable(true);
        camera_v = Vec3(0.0f, 50.0f, 50.0f);
        
        Size size = Director::getInstance()->getWinSize();
        _camera = Camera::createPerspective(30.0f, size.width / size.height, 1.0f, 200.0f);
        _camera->setPosition3D(camera_v);
        _camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
        _camera->setCameraFlag(CameraFlag::USER1);
        this->addChild(_camera);
        
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(MainScene3D::onTouchesBegan, this);
        listener->onTouchesMoved = CC_CALLBACK_2(MainScene3D::onTouchesMoved, this);
        listener->onTouchesEnded = CC_CALLBACK_2(MainScene3D::onTouchesEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        TTFConfig ttfConfig("fonts/arial.ttf", 25);
        auto label = Label::createWithTTF(ttfConfig,"DebugDraw OFF");
         auto zoomin = Label::createWithTTF(ttfConfig,"zoomin");
         auto zoomout = Label::createWithTTF(ttfConfig,"zoomout");
        auto spin = Label::createWithTTF(ttfConfig,"spin");
        auto spout = Label::createWithTTF(ttfConfig,"spout");
        auto menuItem = MenuItemLabel::create(label, [=](Ref *ref){
            if (getPhysics3DWorld()->isDebugDrawEnabled()){
                getPhysics3DWorld()->setDebugDrawEnable(false);
                label->setString("DebugDraw OFF");
            }else{
                getPhysics3DWorld()->setDebugDrawEnable(true);
                label->setString("DebugDraw ON");
            }
        });
        auto menuItem2 = MenuItemLabel::create(zoomin, [=](Ref *ref){
            this->zoomIn(100);
        });
        auto menuItem3 = MenuItemLabel::create(zoomout, [=](Ref *ref){
            this->zoomOut(100);
        });
        auto menuItem4 = MenuItemLabel::create(spin, [=](Ref *ref){
            sprite->setPositionZ(sprite->getPositionZ()+0.1);
        });
        auto menuItem5 = MenuItemLabel::create(spout, [=](Ref *ref){
            sprite->setPositionZ(sprite->getPositionZ()-0.1);
        });
        
        auto menu = Menu::create(menuItem, menuItem2,menuItem3,menuItem4,menuItem5,nullptr);
        menu->setPosition(Vec2::ZERO);
        menuItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        menuItem->setPosition( Vec2(0, 600) );
        menuItem2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        menuItem2->setPosition( Vec2(0, 500) );
        menuItem3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        menuItem3->setPosition( Vec2(0, 550) );
        menuItem4->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        menuItem4->setPosition( Vec2(0, 400) );
        menuItem5->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        menuItem5->setPosition( Vec2(0, 450) );
        this->addChild(menu);
        
        _angle = 0.0f;
        initFloor();
        
         schedule( CC_SCHEDULE_SELECTOR(MainScene3D::update));
        
        return true;
    }
    return false;
}

void MainScene3D::initFloor()
{
    //create floor
    Physics3DRigidBodyDes rbDes;
    rbDes.mass = 0.0f;
    rbDes.shape = Physics3DShape::createBox(Vec3(60.0f, 1.0f, 60.0f));
    
    auto floor = PhysicsSprite3D::create("box.c3t", &rbDes);
    floor->setTexture("plane.png");
    floor->setScaleX(60);
    floor->setScaleZ(60);
    this->addChild(floor);
    floor->setCameraMask((unsigned short)CameraFlag::USER1);
    floor->syncNodeToPhysics();
    //static object sync is not needed
    floor->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NONE);

    this->setPhysics3DDebugCamera(_camera);
    
    auto zuozi = Sprite3D::create("zhuozi.c3b");
    zuozi->setScale(10.04);
    zuozi->setTexture("Icon.png");
    zuozi->setPosition3D(Vec3(0, 2, -3));
    zuozi->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(zuozi);

    initStart(4);
    
    TextureCube* textureCube = TextureCube::create("skybox/left.jpg",
                                       "skybox/right.jpg",
                                       "skybox/top.jpg",
                                       "skybox/bottom.jpg",
                                       "skybox/front.jpg",
                                       "skybox/back.jpg");
    Skybox* skyBox = Skybox::create();
    skyBox->setCameraMask((unsigned short)CameraFlag::USER1);
    skyBox->setTexture(textureCube);
    skyBox->setScale(300.f);
    this->addChild(skyBox,-1);
    
    auto rootps =
    PUParticleSystem3D::create("particle/blackHole.pu",
                               "particle/pu_mediapack_01.material");
    rootps->setScale(0.04);
    rootps->setPosition3D(Vec3(0, 2, -30));
    rootps->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(rootps);
    auto moveby = MoveBy::create(2.0f, Vec2(30.0f, 0.0f));
    auto moveby1 = MoveBy::create(4.0f, Vec3(0.0f, 0.0f,60.f));
    auto moveby2 = MoveBy::create(4.0f, Vec2(-60.0f, 0.0f));
    auto moveby3 = MoveBy::create(4.0f, Vec3(0.0f, 0.0f,-60.f));
    auto moveby4 = MoveBy::create(2.0f, Vec2(30.0f, 0.0f));
    rootps->runAction(RepeatForever::create(Sequence::create(moveby, moveby1,moveby2,moveby3,moveby4, nullptr)));
    rootps->startParticleSystem();
}

void MainScene3D::initStart(int num)
{
    Physics3DRigidBodyDes rbDes;
    //create several boxes using PhysicsSprite3D
    rbDes.mass = 1.f;
    rbDes.shape = Physics3DShape::createBox(Vec3(0.3f,8.0f,0.3f));
    
    float start_x = -2.4f;
    float start_y = 0;
    float start_z = 0;
    
    for (int k=0;k<1;k++)//y
    {
        for (int i=0;i<num;i++)//x
        {
            for(int j = 0;j<num;j++)//z
            {
                float x = 0.2*i + start_x;
                float y = 15.0f + start_y;
                float z = 0.2*j + start_z;
                
                auto sprite = PhysicsSprite3D::create("box.c3t", &rbDes);
                sprite->setTexture("CyanSquare.png");
                sprite->setPosition3D(Vec3(x, y, z));
                sprite->syncNodeToPhysics();
                sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);
                sprite->setCameraMask((unsigned short)CameraFlag::USER1);
                sprite->setScale(0.8f);
                this->addChild(sprite);
                
                sprites.push_back(sprite);
            }
        }
    }
    
    
    rbDes.mass = 0.f;
    rbDes.shape = Physics3DShape::createBox(Vec3(0.3f,8.0f,0.3f));
    sprite = PhysicsSprite3D::create("box.c3t", &rbDes);
    sprite->setTexture("CyanSquare.png");
    sprite->setPosition3D(Vec3(10, 0, 0.5));
    sprite->syncNodeToPhysics();
    sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::NODE_TO_PHYSICS);
    sprite->setCameraMask((unsigned short)CameraFlag::USER1);
    sprite->setScale(0.8f);
    this->addChild(sprite);
    
    sprite->getPhysicsObj()->setCollisionCallback([=](const Physics3DCollisionInfo &ci){
        if (!ci.collisionPointList.empty()){
           // log("-----coll-----");
        }
    });
}

void MainScene3D::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    _isMove = false;
    _isTwoTouch = false;
    _isTouchSprite = false;
    
    auto location = touches[0]->getLocationInView();
    Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);
    nearP = _camera->unproject(nearP);
    farP = _camera->unproject(farP);
    Vec3 dir(farP - nearP);
    Ray ray(_camera->getPosition3D(),dir);
    OBB obb = OBB(sprite->getAABB());
    if(ray.intersects(obb))
    {
        _isTouchSprite = true;
       
    }
    
    event->stopPropagation();
}

void MainScene3D::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    if (touches.size() && _camera)
    {
        auto touch = touches[0];
        auto delta = touch->getDelta();
        float len_delta = delta.lengthSquared();
        if (len_delta > 16)
        {
            _isMove = true;
        }
        
        if(_isTouchSprite)
        {
            auto rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj());
            rigidBody->setActive(true);
            
            for(int i=0;i<sprites.size();i++)
            {
                rigidBody = static_cast<Physics3DRigidBody*>(sprites.at(i)->getPhysicsObj());
                rigidBody->setActive(true);
            }
            
            float dt = Director::getInstance()->getDeltaTime();
            auto location = touch->getLocation();
            auto PreviousLocation = touch->getPreviousLocation();
            Vec2 newPos = (location - PreviousLocation)*dt*1.2f;
            
            Vec3 v = Vec3(newPos.x,newPos.y,0)+sprite->getPosition3D();
           
            sprite->setPosition3D(v);
            // log("----%f--%f--%f-",nearP.x,nearP.y,nearP.z);
        }
        
        if (touches.size() == 2)
        {
            _angle -= CC_DEGREES_TO_RADIANS(delta.x);
            _camera->setPosition3D(Vec3(camera_v.y*sinf(_angle), camera_v.y, camera_v.y*cosf(_angle)));
            _camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
            _isTwoTouch = true;
        }
        

        
        //拖动
        if (touches.size() == 1 && !_isTwoTouch && !_isTouchSprite)
        {
            float dt = Director::getInstance()->getDeltaTime();
            auto location = touch->getLocation();
            auto PreviousLocation = touch->getPreviousLocation();
            Point newPos = PreviousLocation - location;
            
            Vec3 cameraDir;
            Vec3 cameraRightDir;
            _camera->getNodeToWorldTransform().getForwardVector(&cameraDir);
            cameraDir.normalize();
            cameraDir.y=0;
            _camera->getNodeToWorldTransform().getRightVector(&cameraRightDir);
            cameraRightDir.normalize();
            cameraRightDir.y=0;
            Vec3 cameraPos=  _camera->getPosition3D();
            cameraPos+=cameraDir*newPos.y*0.5*dt;
            cameraPos+=cameraRightDir*newPos.x*0.5*dt;
//            cameraPos.x = cameraPos.x > 30.f ? 30.f : cameraPos.x;
//            cameraPos.x = cameraPos.x < -30.f ? -30.f : cameraPos.x;
//            cameraPos.z = cameraPos.z > 80.f ? 80.f : cameraPos.z;
//            cameraPos.z = cameraPos.z < 30.f ? 30.f : cameraPos.z;
            _camera->setPosition3D(cameraPos);
        }
        
        event->stopPropagation();
    }
}

void MainScene3D::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    if (_isMove || _isTwoTouch) return;
    if (!touches.empty())
    {
        auto location = touches[0]->getLocationInView();
        
//        Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);
//        nearP = _camera->unproject(nearP);
//        farP = _camera->unproject(farP);
//        Vec3 dir(farP - nearP);
//        shootBox(_camera->getPosition3D() + dir * 10.0f);
        
       
//        auto rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj());
//        rigidBody->setActive(true);
//        rigidBody->applyForce(Vec3(-100,0,1), sprite->getPosition3D());
        
        event->stopPropagation();
    }
}

MainScene3D::MainScene3D( void )
: _angle(0.0f)
, _camera(nullptr)
{
    
}

void MainScene3D::update(float dt)
{
  
}

MainScene3D::~MainScene3D( void )
{
    
}

void MainScene3D::shootBox( const cocos2d::Vec3 &des )
{
    Physics3DRigidBodyDes rbDes;
    Vec3 linearVel = des - _camera->getPosition3D();
    linearVel.normalize();
    linearVel *= 100.0f;
    rbDes.originalTransform.translate(_camera->getPosition3D());
    rbDes.mass = 1.f;
    rbDes.shape = Physics3DShape::createBox(Vec3(0.5f, 0.5f, 0.5f));
    auto sprite = PhysicsSprite3D::create("box.c3t", &rbDes);
    sprite->setTexture("Icon.png");
    
    auto rigidBody = static_cast<Physics3DRigidBody*>(sprite->getPhysicsObj());
    rigidBody->setLinearFactor(Vec3::ONE);
    rigidBody->setLinearVelocity(linearVel);
    rigidBody->setAngularVelocity(Vec3::ZERO);
    rigidBody->setCcdMotionThreshold(0.5f);
    rigidBody->setCcdSweptSphereRadius(0.4f);
    
    this->addChild(sprite);
    sprite->setPosition3D(_camera->getPosition3D());
    sprite->setScale(0.5f);
    sprite->syncNodeToPhysics();
    
    //optimize, only sync node to physics
    sprite->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE); //sync node to physics
    
    sprite->setCameraMask((unsigned short)CameraFlag::USER1);
}

int MainScene3D::isZoomRotate()
{
    if(touchs0.size() < 2 || touchs1.size() < 2)
        return 0;
    
    float dis1 = touchs0[1].getDistance(touchs1[1]);
    float dis2 = touchs0[0].getDistance(touchs1[0]);
    float r = touchs0[0].getDistance(touchs0[1]);
   
    if(dis1 > dis2)
    {
         zoomIn(r);
    }
    else
    {
        zoomOut(r);
    }
    
//    float maxAngle = 0;
//    float maxAngle2 = 0;
//    
//    float angle1 = touchs0[5].getAngle(touchs0[0]);
//    for(int i=5;i<touchs0.size()-5;i+=5)
//    {
//        float angle2 = touchs0[i+5].getAngle(touchs0[i]);
//        float angle = fabsf(angle2 - angle1);
//        maxAngle = angle > maxAngle ? angle : maxAngle;
//    }
//    angle1 = touchs1[5].getAngle(touchs1[0]);
//    for(int i=0;i<touchs1.size()-2;i++)
//    {
//        float angle2 = touchs1[i+5].getAngle(touchs1[i]);
//        float angle = fabsf(angle2 - angle1);
//        maxAngle2 = angle > maxAngle2 ? angle : maxAngle2;
//    }
//
//    //判断是否zoomin
//    if(maxAngle < 0.08f && maxAngle2 < 0.08f)
//    {
//        float dis1 = touchs0[1].getDistance(touchs1[1]);
//        float dis2 = touchs0[touchs0.size()-2].getDistance(touchs1[touchs1.size()-2]);
//        if(dis2 > dis1)
//        {
//            zoomIn();
//        }
//        else{
//            zoomOut();
//        }
//    }
//    else
//        log("----null----%f",maxAngle);
    return 0;
}

void MainScene3D::zoomIn(float r)
{
    Vec3 cameraPos = _camera->getPosition3D();
    if(cameraPos.length() >= 20)
    {
        cameraPos -= cameraPos.getNormalized()*(60.f/1000.f*r);
        _camera->setPosition3D(cameraPos);
        camera_v = cameraPos;
         log("-%f---%f----%f",camera_v.x,camera_v.y,camera_v.z);
    }
}

void MainScene3D::zoomOut(float r)
{
    Vec3 cameraPos = _camera->getPosition3D();
    if(cameraPos.length() <= 100)
    {
        cameraPos += cameraPos.getNormalized()*(60.f/1000.f*r);
        _camera->setPosition3D(cameraPos);
        camera_v = cameraPos;
    }
}

void MainScene3D::unproject(const Mat4& viewProjection, const Size* viewport, Vec3* src, Vec3* dst)
{
    assert(dst);
    
    assert(viewport->width != 0.0f && viewport->height != 0.0f);
    Vec4 screen(src->x / viewport->width, ((viewport->height - src->y)) / viewport->height, src->z, 1.0f);
    
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    viewProjection.getInversed().transformVector(screen, &screen);
    
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->set(screen.x, screen.y, screen.z);
}

void MainScene3D::calculateRayByLocationInView(Ray* ray, const Vec2& location)
{
    auto dir = Director::getInstance();
    auto view = dir->getWinSize();
    auto mat = dir->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    Vec3 src = Vec3(location.x, location.y, -1);
    Vec3 nearPoint;
    unproject(mat, &view, &src, &nearPoint);
    
    src = Vec3(location.x, location.y, 1);
    Vec3 farPoint;
    unproject(mat, &view, &src, &farPoint);
    
    Vec3 direction;
    Vec3::subtract(farPoint, nearPoint, &direction);
    direction.normalize();
    
    ray->_origin = nearPoint;
    ray->_direction = direction;
}


