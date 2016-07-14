//
//  MianScene3D.h
//  wallmm
//
//  Created by YanChunGuang on 16/6/15.
//
//

#ifndef MianScene3D_h
#define MianScene3D_h

#include "cocos2d.h"
#include "physics3d/CCPhysics3D.h"

USING_NS_CC;




class MainScene3D : public Scene
{
public:
    CREATE_FUNC(MainScene3D);
    MainScene3D(void);
    virtual ~MainScene3D(void);
    // overrides
    virtual bool init() override;
    void initFloor();
    void initStart(int num);
    virtual void update(float delta) override;
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
    
protected:
    //0 什么都没有 1 zoomin 2 zoomout 3 rotate top 4 rotate bottom
    int isZoomRotate();
    void shootBox(const Vec3 &des);
    
    void zoomIn(float r);
    void zoomOut(float r);
    
    void unproject(const Mat4& viewProjection, const Size* viewport, Vec3* src, Vec3* dst);
    void calculateRayByLocationInView(Ray* ray, const Vec2& location);
protected:
    std::string    _title;
    Camera *_camera;
    float _angle;
    bool _isMove;
    bool _isTwoTouch;
    bool _isTouchSprite;
    std::vector<Vec2> touchs0;
    std::vector<Vec2> touchs1;
    std::vector<PhysicsSprite3D*> sprites;
    PhysicsSprite3D* sprite;
    Vec3 camera_v;
};


#endif /* MianScene3D_h */
