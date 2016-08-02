//
//  MainScene2D.cpp
//  wallmm
//
//  Created by YanChunGuang on 16/6/17.
//
//

#include "MainScene2D.h"
#include "GResource.h"
#include "audio/include/SimpleAudioEngine.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#endif



using namespace CocosDenshion;

#define DRAG_BODYS_TAG 1
#define MAX_LEVEL 20
#define DT_ISSTART 0.2


static MainScene2D* _instance = nullptr;
bool MainScene2D::init()
{
    if (!Scene::init() )
    {
        return false;
    }
    
    _debugDraw = false;
    
    if(initWithPhysics())
    {
        _physicsWorld->setGravity(Vec2(0, -980));
        
        Size size = Director::getInstance()->getWinSize();
        Vec2 oriv = Director::getInstance()->getVisibleOrigin();
        
        int boder_left = 2;
        int boder_right = size.width-2;
        int boder_top = size.height;
        int boder_bottom = 0;
        
        auto bg = Sprite::create("bg.jpg");
        bg->setPosition(size.width/2, size.height/2);
        this->addChild(bg);
        
        auto tanzi = Sprite::create("tanzi.png");
        tanzi->setPosition(size.width/2, 100);
        this->addChild(tanzi);
        
        auto bottom = Sprite::create("bottom.png");
        bottom->setAnchorPoint(Vec2(0.5,0));
        bottom->setPosition(size.width/2, 0);
        this->addChild(bottom);
        
        auto qq = Sprite::create("qq.png");
        qq->setPosition(250, 335);
        qq->setScale(0.7);
        this->addChild(qq);
        
        auto qq2 = Sprite::create("qq1.png");
        qq2->setPosition(size.width-250, 335);
        qq2->setScale(0.7);
        this->addChild(qq2);
        
        auto uilayer = Layer::create();
        uilayer->setName("uilayer");
        this->addChild(uilayer,100);
        
        auto diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(0,size.height);
        uilayer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(100,size.height);
        uilayer->addChild(diban);
        btn = Button::create("music.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("music");
        if(!UserDefault::getInstance()->getBoolForKey("music"))
        {
            btn->loadTextureNormal("music2.png");
        }
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(200,size.height);
        uilayer->addChild(diban);
        btn = Button::create("sound.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("sound");
        if(!UserDefault::getInstance()->getBoolForKey("sound"))
        {
            btn->loadTextureNormal("sound2.png");
        }
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(300,size.height);
        uilayer->addChild(diban);
        btn = Button::create("btn_guanqia.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("guanqia");
        
        
        diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(size.width*0.9f,size.height);
        uilayer->addChild(diban);
        btn = Button::create("help.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("help");
        
        btn = Button::create("hongbao.png");
        btn->setAnchorPoint(Vec2(1,1));
        btn->setPosition(Vec2(diban->getPositionX()+5 ,size.height));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        uilayer->addChild(btn);
        btn->setName("hongbao");
        
        btn = Button::create("suaxin.png");
        btn->setAnchorPoint(Vec2(1,1));
        btn->setPosition(Vec2(diban->getPositionX() - 100,size.height-10));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        uilayer->addChild(btn);
        btn->setName("shuaxin");
        
        int suaxin_num = UserDefault::getInstance()->getIntegerForKey("suaxin_num");
        char c[7];
        sprintf(c, "%d", suaxin_num);
        auto num_bg = Sprite::create("dian_1.png");
        num_bg->setAnchorPoint(Vec2(1,0));
        num_bg->setPosition(btn->getContentSize().width-2, 2);
        btn->addChild(num_bg);
        l_suaxin = Label::createWithSystemFont(c, "", 24);
        l_suaxin->setColor(Color3B::BLUE);
        l_suaxin->setPosition(num_bg->getContentSize().width/2, num_bg->getContentSize().height/2);
        num_bg->addChild(l_suaxin);
  
        
        btn = Button::create("wudi.png");
        btn->setAnchorPoint(Vec2(1,1));
        btn->setPosition(Vec2(diban->getPositionX() - 180,size.height-10));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        uilayer->addChild(btn);
        btn->setName("wudi");
        
        int wudi_num = UserDefault::getInstance()->getIntegerForKey("wudi_num");
        sprintf(c, "%d", wudi_num);
        num_bg = Sprite::create("dian_1.png");
        num_bg->setAnchorPoint(Vec2(1,0));
        num_bg->setPosition(btn->getContentSize().width-2, 2);
        btn->addChild(num_bg);
        l_wudi = Label::createWithSystemFont(c, "", 24);
        l_wudi->setColor(Color3B::BLUE);
        l_wudi->setPosition(num_bg->getContentSize().width/2, num_bg->getContentSize().height/2);
        num_bg->addChild(l_wudi);

        if(Director::getInstance()->isDisplayStats())
        {
            // menu for debug layer
            MenuItemFont::setFontSize(18);
            auto item = MenuItemFont::create("Toggle debug", CC_CALLBACK_1(MainScene2D::toggleDebug, this));
            item->setAnchorPoint(Vec2(0,0.5));
            auto menu = Menu::create(item, nullptr);
            menu->setAnchorPoint(Vec2(0,0.5));
            menu->setPosition(Vec2(10, 500));
            uilayer->addChild(menu);
        }
        
        auto shetou = Sprite::create("shetou.png");
        shetou->setAnchorPoint(Vec2(0.5,1));
        shetou->setPosition(size.width/2, size.height);
        uilayer->addChild(shetou);
        
        Label * label = Label::createWithSystemFont("", "", 38);
        label->setColor(Color3B::BLUE);
        label->setPosition(size.width/2, size.height*0.6f);
        uilayer->addChild(label,100);
        label->setOpacity(0);
        label->setName("label");
        
        Label *label2 = Label::createWithSystemFont("", "", 28);
        label2->setColor(Color3B::BLUE);
        label2->setPosition(size.width/2, size.height*0.6f+70);
        uilayer->addChild(label2);
        label2->setOpacity(0);
        label2->setName("label2");
        
        l_score = Label::createWithSystemFont("", "", 28);
        l_score->setColor(Color3B::BLUE);
        l_score->setAnchorPoint(Vec2(0,0));
        l_score->setPosition(124, 22);
        shetou->addChild(l_score,100);
        
        l_time = Label::createWithSystemFont("", "", 24);
        l_time->setColor(Color3B::BLUE);
        l_time->setAnchorPoint(Vec2(0.5,1));
        l_time->setPosition(size.width/2, size.height-120);
        uilayer->addChild(l_time,100);

        l_coin = Label::createWithSystemFont("", "", 28);
        l_coin->setColor(Color3B::BLUE);
        l_coin->setAnchorPoint(Vec2(0,0));
        l_coin->setPosition(100, 55);
        shetou->addChild(l_coin,100);
        
        _gameLayer = Layer::create();
        this->addChild(_gameLayer);
        
        auto node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_bottom), Vec2(boder_left, boder_top),PhysicsMaterial(0.1f,1,1.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_right, boder_bottom), Vec2(boder_right, boder_top),PhysicsMaterial(0.1f,1,1.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        node->addComponent(PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_bottom), Vec2(boder_right, boder_bottom),PhysicsMaterial(0.1f,0.1f,0.1f)));
        _gameLayer->addChild(node);
        
        node = Node::create();
        top = PhysicsBody::createEdgeSegment(Vec2(boder_left, boder_top), Vec2(boder_right, boder_top),PhysicsMaterial(0.1f,0.1f,0.1f));
        node->addComponent(top);
        _gameLayer->addChild(node);
        
        
        node = Node::create();
        Vec2 edges[] = {Vec2(mx(0), 145),Vec2(mx(40), 167),Vec2(mx(90), 167),Vec2(mx(120), 150),Vec2(mx(142), 127),Vec2(mx(200), 125),Vec2(mx(230), 115),Vec2(mx(260), 91),Vec2(mx(300), 101),Vec2(mx(322), 99),Vec2(mx(355), 120),Vec2(mx(390), 128),Vec2(mx(430), 118),Vec2(mx(459), 91),Vec2(mx(500), 115),Vec2(mx(540), 122),Vec2(mx(590), 118),Vec2(mx(625), 98),Vec2(mx(660), 101),Vec2(mx(700), 95),Vec2(mx(730), 113),Vec2(mx(760), 119),Vec2(mx(790), 115),Vec2(mx(810), 108),Vec2(mx(840), 139),Vec2(mx(875), 147),Vec2(mx(915), 135),Vec2(mx(937), 110),Vec2(mx(980), 120),Vec2(mx(1015), 110),Vec2(mx(1038), 159),Vec2(mx(1070), 169),Vec2(mx(1110), 165),Vec2(mx(1136), 149),Vec2(mx(1170), 155),Vec2(mx(1200), 140)};
        node->addComponent(PhysicsBody::createEdgeChain(edges, 36));
        _gameLayer->addChild(node);
        
        node = Node::create();
        auto circle_body = PhysicsBody::createCircle(40,PhysicsMaterial(0.f,0.1f,0.1f));
        circle_body->setDynamic(false);
        node->addComponent(circle_body);
        node->setPosition(qq->getPosition());
        _gameLayer->addChild(node);
        
        node = Node::create();
        auto circle_body2 = PhysicsBody::createCircle(40,PhysicsMaterial(0.f,0.1f,0.1f));
        circle_body2->setDynamic(false);
        node->addComponent(circle_body2);
        node->setPosition(qq2->getPosition());
        _gameLayer->addChild(node);
        
//        node = Node::create();
//        Vec2 tv = tanzi->getPosition();
//        tv.x = tv.x - 105;
//        tv.y = tv.y - 45;
//        
//        Vec2 edges2[] = {Vec2(tv.x, tv.y+45),Vec2(tv.x+20, tv.y+82),Vec2(tv.x+110, tv.y+89),Vec2(tv.x+200, tv.y+82),Vec2(tv.x+217, tv.y+45),Vec2(tv.x+185, tv.y+3),Vec2(tv.x+110, tv.y+0),Vec2(tv.x+30, tv.y+3),Vec2(tv.x, tv.y+45)};
//        node->addComponent(PhysicsBody::createEdgeChain(edges2, 9));
//        _gameLayer->addChild(node);

        
        
//        hand = Sprite::create("hand.png");
//        hand->setPosition(size.width*0.9f, size.height*0.9f);
//        _gameLayer->addChild(hand,1000);

        _physicsWorld->setAutoStep(false);
//        auto contactListener = EventListenerPhysicsContact::create();
//        contactListener->onContactBegin = CC_CALLBACK_1(MainScene2D::onContactBegin, this);
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        schedule(CC_SCHEDULE_SELECTOR(MainScene2D::update));
        
        playMusic();
        
        v_font =  FileUtils::getInstance()->getValueVectorFromFile("fonts/font.plist");
        
        auto rain = ParticleRain::create();
        rain->setStartColor(Color4F::MAGENTA);
        rain->setPosition(Vec2(size.width/2,size.height));
        this->addChild(rain);
    }
    _instance = this;
    return true;
}

bool MainScene2D::onContactBegin(PhysicsContact& contact)
{
//    PhysicsBody* a = contact.getShapeA()->getBody();
//    PhysicsBody* b = contact.getShapeB()->getBody();
   
    log("----");
    return true;
}

void MainScene2D::update(float dt)
{
    for (int i = 0; i < 2; ++i)
    {
        _physicsWorld->step(dt/3);
    }
    
    Size size = Director::getInstance()->getWinSize();
    for(int i=0;i<sprites.size();i++)
    {
        if(sprites[i]->getPositionX() > size.width || sprites[i]->getPositionX() < 0
           || sprites[i]->getPositionY() > size.height || (sel == sprites[i] && isWuDi))
        {
            if(sel == sprites[i])
            {
                if(isWuDi)
                {
                    isWuDi = false;
                    isStart = false;
                    judgeStart = true;
                    dt_stop = 0;
                    dt_judgeStart = 1.5f;
                    Label * node = (Label *)(this->getChildByName("uilayer")->getChildByName("label"));
                    node->setOpacity(255);
                    node->setString(v_font.at(4).asString());
                    
                    sel->runAction(Sequence::create(TintTo::create(0.2f, 255, 0, 0),
                                                    FadeOut::create(0.8f),RemoveSelf::create(), NULL));
                }
                else
                {
                    sel->runAction(Sequence::create(TintTo::create(0.2f, 255, 255, 255),RemoveSelf::create(), NULL));
                }
                isSelRemove = true;
            
                score += 1;
                char c[8];
                sprintf(c, "%i", score);
               // std::string s = v_font.at(7).asString()+c;
                l_score->setString(c);
                
                int sco =  UserDefault::getInstance()->getIntegerForKey("score");
                if(score > sco)
                {
                    UserDefault::getInstance()->setIntegerForKey("score", score);
                }
            }
            else{
                sprites[i]->runAction(Sequence::create(TintTo::create(0.2f, 255, 255, 255),RemoveSelf::create(), NULL));
            }
            
           // removes.push_back(sprites[i]);
            if(vecs.size() && vecs_min.size() && vecs_max.size())
            {
                vecs.erase(vecs.begin()+i);
                vecs_min.erase(vecs_min.begin()+i);
                vecs_max.erase(vecs_max.begin()+i);
            }
            
            for(int j=0;j<moves.size();j++)
            {
                if(moves[j] == sprites[i])
                {
                    moves.erase(moves.begin()+j);
                    break;
                }
            }
            
            sprites.erase(sprites.begin()+i);
            
            if(sprites.size() == 0)
            {
                gameWin();
            }
            break;
        }
        
    }
    
    if(sel && isStart)
    {
        for(int i=0;i<sprites.size();i++)
        {
            if(sprites[i] == sel)
                continue;
            if(sprites[i]->getPosition().getDistance(vecs[i]) > 3)
            {
                addMoves(sprites[i]);
            }
            Vec2 v1,v2;
            Rect r = sprites[i]->getBoundingBox();
            v1.x = r.getMinX();
            v1.y = r.getMinY();
            v2.x = r.getMaxX();
            v2.y = r.getMaxY();
            if(v1.getDistance(vecs_min[i]) > 3)
            {
                 addMoves(sprites[i]);
            }
            if(v2.getDistance(vecs_max[i]) > 3)
            {
                 addMoves(sprites[i]);
            }
        }
        if(moves.size() > 0)
        {
            gameOver();
        }
    }
    
    
    if(judgeStart)
    {        
        dt_stop += dt;
        if(dt_stop > dt_judgeStart)
        {
            dt_stop = 0;
            bool isb = false;
            for(int i=0;i<sprites.size();i++)
            {
                if(sprites[i]->getPosition().getDistance(vecs[i]) > 2)
                {
                    isb = true;
                }
                Vec2 v1,v2;
                Rect r = sprites[i]->getBoundingBox();
                v1.x = r.getMinX();
                v1.y = r.getMinY();
                v2.x = r.getMaxX();
                v2.y = r.getMaxY();
                if(v1.getDistance(vecs_min[i]) > 2)
                {
                    isb = true;
                }
                if(v2.getDistance(vecs_max[i]) > 2)
                {
                    isb = true;
                }
                
                vecs[i] = sprites[i]->getPosition();
                vecs_min[i] = v1;
                vecs_max[i] = v2;
            }
            if(!isb)
            {
                dt_judgeStart = DT_ISSTART;
                judgeStart = false;
                Label * node = (Label *)(this->getChildByName("uilayer")->getChildByName("label"));
                node->setString(v_font.at(5).asString());
                auto action = Sequence::create(CallFunc::create( std::bind(&MainScene2D::startEnd, this)),FadeOut::create(1), nullptr);
                node->runAction(action);
                
                node = (Label *)(this->getChildByName("uilayer")->getChildByName("label2"));
                node->setString("");
            }
        }
    }
    
    if(isStart)
    {
        dt_time -= dt;
        int t = dt_time;
        if(t >= 0)
        {
            char c[8];
            sprintf(c, "%i", t);
            l_time->setString(v_font.at(17).asString() + c);
        }
    }

    dt_playSound+=dt;
}
void MainScene2D::addMoves(Sprite* sp)
{
    bool b = false;
    for(int i=0;i<moves.size();i++)
    {
        if(moves.at(i) == sp)
        {
            b = true;
            break;
        }
    }
    if(!b)
    {
        moves.push_back(sp);
    }
}

void MainScene2D::gameWin()
{
    isStart = false;
  

    this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&MainScene2D::addGameTouchLayer, this,true)), NULL));
    
    Size size = Director::getInstance()->getWinSize();
    
    Vec2 v(size.width/2,size.height/2);
    
    particle = ParticleSystemQuad::create("huochai.plist");
    particle->setPosition(v+Vec2(0,50));
    _gameLayer->addChild(particle);

    auto seq = Sequence::create(MoveBy::create(0.2, Vec2(100,0)), MoveBy::create(0.2, Vec2(0,-100)),
                                MoveBy::create(0.2, Vec2(-100,0)),MoveBy::create(0.2, Vec2(0,100)),NULL);
    particle->runAction(Sequence::create(Repeat::create(seq, 3),RemoveSelf::create(true) , NULL));
}

void MainScene2D::gameOver()
{
   // isStart = false;
    if(!isFirstDie)
    {
        moves.insert(moves.begin(), sel);
        this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&MainScene2D::addGameTouchLayer, this,false)), NULL));
    }
    isFirstDie = true;
   
    for(int i=0;i<moves.size();i++)
    {
        Sprite* sp = moves.at(i);
       
        if(sp->getTag() == 2)
            continue;
        auto seq = Sequence::create(DelayTime::create(0.1f*i),
                         CallFunc::create(std::bind(&MainScene2D::updateParticle, this,sp)),
                         TintTo::create(0.2f, 0, 0, 0),
                                    NULL);
        sp->setTag(2);
        sp->runAction(seq);
    }
}

void MainScene2D::updateParticle(Node* node)
{
    Node* particle = node->getChildByTag(2);
    particle->setVisible(true);
    
    if(dt_playSound > 0.2)
    {
        dt_playSound = 0;
        playSound("die.wav");
    }
   
}

void MainScene2D::addGameTouchLayer(bool isWin)
{
    isStart = false;
    isShuaXin = false;
    auto layer = TouchLayer::create();
    layer->setName("touchLayer");
    this->addChild(layer, 101);
    
    int curr_level =  UserDefault::getInstance()->getIntegerForKey("curr_level");
    
    Size size = Director::getInstance()->getWinSize();
    
    if(isWin)
    {
        int model = UserDefault::getInstance()->getIntegerForKey("game_model");
        if(model == 1)
        {
            int level =  UserDefault::getInstance()->getIntegerForKey("level");
            if(curr_level == level && level < MAX_LEVEL)
            {
                UserDefault::getInstance()->setIntegerForKey("level", level+1);
                UserDefault::getInstance()->setIntegerForKey("curr_level", curr_level+1);
            }
            
            if(curr_level < level)
                UserDefault::getInstance()->setIntegerForKey("curr_level", curr_level+1);
        }
        
        randZongFen();
        
        std::string s = v_font.at(3).asString();
        Text* label = Text::create(s, "", 50);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(size.width / 2.0f, size.height * 0.78f));
        layer->addChild(label);
        
        s = v_font.at(15).asString();
        char c[8];
        sprintf(c, "%d", zongfen);
        Text* label2 = Text::create(s+c+v_font.at(14).asString(), "", 30);
        label2->setColor(Color3B::WHITE);
        label2->setPosition(Vec2(size.width / 2.0f, size.height * 0.78f-50));
        layer->addChild(label2);
        
        auto diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.3f+60, size.height * 0.5f+30));
        layer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.7f-60, size.height * 0.5f+30));
        layer->addChild(diban);
        btn = Button::create("jixu.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("replay");
        
        
        btn = Button::create("fenxiang2.png");
        btn->setPosition(Vec2(size.width*0.5f, size.height * 0.3f));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        layer->addChild(btn);
        btn->setName("fenxiang");
        
        int coin = UserDefault::getInstance()->getIntegerForKey("coin");
        int award = score;
        
        sprintf(c, "%i", curr_level);
        std::string level_s = "level_";
        level_s+=c;
        
        if(dt_time > 0 && !UserDefault::getInstance()->getBoolForKey(level_s.c_str()))
        {
            award = score * 2;
            
            UserDefault::getInstance()->setBoolForKey(level_s.c_str(),true);
        }
        UserDefault::getInstance()->setIntegerForKey("coin", coin+award);
        sprintf(c, "%i", coin+award);
        l_coin->setString(c);
  
        sprintf(c, "%d", award);
        Text* label3 = Text::create(v_font.at(19).asString()+c, "", 20);
        label3->setColor(Color3B::BLUE);
        label3->setPosition(Vec2(size.width / 2.0f, size.height * 0.78f-100));
        layer->addChild(label3);
        
        showAd(1);
    }
    else
    {
        randZongFen();
        
        std::string s = v_font.at(2).asString();
        Text* label = Text::create(s, "", 50);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(size.width / 2.0f, size.height * 0.78f));
        layer->addChild(label);
        
//        s = v_font.at(13).asString();
//        char c[8];
//        sprintf(c, "%d", zongfen);
        Text* label2 = Text::create(v_font.at(16).asString(), "", 30);
        label2->setColor(Color3B::WHITE);
        label2->setPosition(Vec2(size.width / 2.0f, size.height * 0.78f-50));
        layer->addChild(label2);
        
        auto diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.3f+60, size.height * 0.5f+30));
        layer->addChild(diban);
        Button* btn = Button::create("home.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("home");
        
        diban = Sprite::create("diban.png");
        diban->setPosition(Vec2(size.width*0.7f-60, size.height * 0.5f+30));
        layer->addChild(diban);
        btn = Button::create("replay.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("replay");
        
        btn = Button::create("fenxiang2.png");
        btn->setPosition(Vec2(size.width*0.5f, size.height * 0.3f));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        layer->addChild(btn);
        btn->setName("fenxiang");
        
        
        showAd(2);
    }
    
}

void MainScene2D::startGame(Ref *sender)
{
    if(sender)
        playSound("btn.wav");
    
    isStart = false;
    isFirstDie = false;
    judgeStart = false;
    sel = nullptr;
    
    dt_stop = 0;
    dt_time = 60;
    l_time->setString(v_font.at(17).asString() + "60");
    
    int coin = UserDefault::getInstance()->getIntegerForKey("coin");
    char c[8];
    sprintf(c, "%i", coin);
    l_coin->setString(c);
    
    if(!isShuaXin)
    {
        score = 0;
        l_score->setString("0");
    }
    
    preInit();
}

void MainScene2D::randZongFen()
{
    int model = UserDefault::getInstance()->getIntegerForKey("game_model");
    int r = 3;
    if(model == 1)
    {
        int curr_level =  UserDefault::getInstance()->getIntegerForKey("curr_level");
        int level =  UserDefault::getInstance()->getIntegerForKey("level");
        if(curr_level == 0)
        {
            UserDefault::getInstance()->setIntegerForKey("curr_level", r);
        }
        else
        {
            r = curr_level;
        }
        
        if(level == 0)
        {
            UserDefault::getInstance()->setIntegerForKey("level", r);
        }
    }
    else{
        int num = UserDefault::getInstance()->getIntegerForKey("self_num");
        r = num;
    }
    zongfen = r;
}

void MainScene2D::preInit()
{
    top->setEnabled(true);
    for(int i=0;i<sprites.size();i++)
    {
        sprites[i]->removeFromParent();
    }
    sprites.clear();
   // removes.clear();
    moves.clear();
    
    vecs.clear();
    vecs_min.clear();
    vecs_max.clear();

    
     Size size = Director::getInstance()->getWinSize();
    int num = zongfen;
    if(isShuaXin)
        num = num - score;
    for(int i=0;i<num;i++)
    {
        addSprite(Vec2(size.width/2,size.height*0.6f));
    }
//
//    auto action = Sequence::create(DelayTime::create(0.8f), CallFunc::create( std::bind(&MainScene2D::start, this)),nullptr);
//    this->runAction(action);
    start();
   
}

void MainScene2D::start()
{
    Vec2 v(1500,1);
    Vec2 v1,v2;
    int ra = random(30, 90);
    for(int i=0;i<sprites.size();i++)
    {
        PhysicsBody* body = (PhysicsBody*)sprites[i]->getComponent("box");
        body->setEnabled(true);
        v.rotate(Vec2::ZERO, ra);
        body->setVelocity(v);
        
        vecs.push_back(sprites[i]->getPosition());
        Rect r = sprites[i]->getBoundingBox();
        v1.x = r.getMinX();
        v1.y = r.getMinY();
        v2.x = r.getMaxX();
        v2.y = r.getMaxY();
        vecs_min.push_back(v1);
        vecs_max.push_back(v2);
    }

    dt_judgeStart = DT_ISSTART;
    judgeStart = true;
    dt_stop = 0;
    Label * node = (Label *)(this->getChildByName("uilayer")->getChildByName("label"));
    node->setOpacity(255);
    node->setString(v_font.at(4).asString());
    
    node = (Label *)(this->getChildByName("uilayer")->getChildByName("label2"));
    node->setOpacity(255);
    char c[8];
    sprintf(c, "%i", zongfen);
    node->setString(v_font.at(13).asString() + c + v_font.at(14).asString());
}

void MainScene2D::startEnd()
{
    top->setEnabled(false);
    isStart = true;
    log("start game");
    //hand->setVisible(true);
}

void MainScene2D::addSprite(Vec2 v)
{
    int z = random(1, 100);
    
    auto ball = Sprite::create("huochai.png");
    //ball->setAnchorPoint(Vec2::ZERO);
    auto body = PhysicsBody::createBox(Size(22,215),PhysicsMaterial(1,0.8,1));
    body->setName("box");
    body->setDynamic(true);
    body->setEnabled(false);
    body->setRotationEnable(true);
    ball->addComponent(body);
    body->setTag(DRAG_BODYS_TAG);
    ball->setPosition(v);
    
    //body->setCategoryBitmask(z-1);    // 1000
   // body->setContactTestBitmask(z-2); // 0010
   // body->setCollisionBitmask(2);

    _gameLayer->addChild(ball,z);
    
    particle = ParticleSystemQuad::create("huochai.plist");
    particle->setRotation(90);
    particle->setPosition(Vec2(ball->getContentSize().width/2,ball->getContentSize().height/2));
    particle->setTag(2);
    particle->setVisible(false);
    ball->addChild(particle,101);
    
    sprites.push_back(ball);
}

void MainScene2D::getSel(Touch* touch)
{
    auto location = touch->getLocation();
    Vec2 v = location;
    auto arr = _physicsWorld->getShapes(v);
    
    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        if ((obj->getBody()->getTag() & DRAG_BODYS_TAG) != 0)
        {
            body = obj->getBody();
            break;
        }
    }
    
    if (body != nullptr)
    {
        Node* mouse = Node::create();
        auto physicsBody = PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY);
        physicsBody->setDynamic(false);
        mouse->addComponent(physicsBody);
        mouse->setPosition(v);
        this->addChild(mouse);
        
        
        for(int i=0;i<sprites.size();i++)
        {
            if(sprites[i]->getComponent("box") == body)
            {
                sel = sprites[i];
                isSelRemove = false;
                sprites[i]->runAction(TintTo::create(0.2f, 255, 0, 0));
                playSound("sel.wav");
                break;
            }
        }
        
        PhysicsJointPin* joint = PhysicsJointPin::construct(physicsBody, body, v);
        joint->setMaxForce(8000.0f * body->getMass());
        _physicsWorld->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));
        
        if(sel)
        {
            vecs.clear();
            vecs_min.clear();
            vecs_max.clear();
            Vec2 v1,v2;
            for(int i=0;i<sprites.size();i++)
            {
                vecs.push_back(sprites[i]->getPosition());
                Rect r = sprites[i]->getBoundingBox();
                v1.x = r.getMinX();
                v1.y = r.getMinY();
                v2.x = r.getMaxX();
                v2.y = r.getMaxY();
                vecs_min.push_back(v1);
                vecs_max.push_back(v2);
            }
            
        }
    }

}

void MainScene2D::duihuanResult(int type)
{
    int coin = UserDefault::getInstance()->getIntegerForKey("coin");
    std::string s = v_font.at(23).asString();
    if(coin < 100)
    {
        s = v_font.at(24).asString();
        TostLayer::show(this, s);
        return;
    }
    else
    {
         char c[7];
        if(type == 1)
        {
            int suaxin_num = UserDefault::getInstance()->getIntegerForKey("suaxin_num");
            UserDefault::getInstance()->setIntegerForKey("suaxin_num",suaxin_num+1);
            sprintf(c, "%d", suaxin_num+1);
            l_suaxin->setString(c);
        }
        else{
            int wudi_num = UserDefault::getInstance()->getIntegerForKey("wudi_num");
            UserDefault::getInstance()->setIntegerForKey("wudi_num",wudi_num+1);
            sprintf(c, "%d", wudi_num+1);
            l_wudi->setString(c);
        }
        coin = coin - 100;
        UserDefault::getInstance()->setIntegerForKey("coin",coin);
        sprintf(c, "%d", coin);
        l_coin->setString(c);
        TostLayer::show(this, s);
    }
}

void MainScene2D::duihuan(int type)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    auto layer = TouchLayer::create();
    layer->setName("touchLayer");
    this->addChild(layer, 101);
    
    auto bg = LayerColor::create(Color4B(0,0,0,160),winSize.width,winSize.height);
    layer->addChild(bg);
    
    auto bg2 = Sprite::create("duihuan_1.png");
    bg2->setPosition(winSize.width/2, winSize.height/2+80);
    layer->addChild(bg2);
    
    
    Label* l = Label::createWithSystemFont("100", "", 24);
    l->setColor(Color3B::RED);
    l->setPosition(183, 194);
    bg2->addChild(l);
    
    auto daoju = Sprite::create();
    if(type == 1)
    {
        daoju->initWithFile("suaxin.png");
    }
    else
    {
        daoju->initWithFile("wudi.png");
    }
    daoju->setPosition(206, 96);
    bg2->addChild(daoju);

    Button* duihuan = Button::create("duihuan_2.png");
    duihuan->setAnchorPoint(Vec2(0.5,1));
    duihuan->setPosition(Vec2(bg2->getContentSize().width/2,-10));
    duihuan->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    bg2->addChild(duihuan);
    duihuan->setTag(type);
    duihuan->setName("duihuan");
    
    Button* btn = Button::create("002.png","003.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setPosition(Vec2(bg2->getContentSize().width-10,bg2->getContentSize().height-10));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    bg2->addChild(btn);
    btn->setName("close_duihuan");
}

void MainScene2D::shuaxin()
{
    int suaxin_num = UserDefault::getInstance()->getIntegerForKey("suaxin_num");
    if(suaxin_num < 1)
    {
        duihuan(1);
        return;
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey("suaxin_num",suaxin_num-1);
        char c[7];
        sprintf(c, "%d", suaxin_num-1);
        l_suaxin->setString(c);
    }
    isShuaXin = true;
    startGame(nullptr);
}

void MainScene2D::wudi()
{
    int wudi_num = UserDefault::getInstance()->getIntegerForKey("wudi_num");
    if(wudi_num < 1)
    {
        duihuan(2);
        return;
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey("wudi_num",wudi_num-1);
        char c[7];
        sprintf(c, "%d", wudi_num-1);
        l_wudi->setString(c);
    }
    sel = nullptr;
    isWuDi = true;
}

void MainScene2D::guanqia()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    auto layer = TouchLayer::create();
    layer->setName("touchLayer");
    this->addChild(layer, 101);
    
    auto bg = LayerColor::create(Color4B(0,0,0,160),winSize.width,winSize.height);
    layer->addChild(bg);
    
    auto sp = Sprite::create("guanqia_sel.png");
    sp->setPosition(Vec2(winSize.width/2,winSize.height/2));
    bg->addChild(sp);
    
    // Create the page view
    PageView* pageView = PageView::create();
    pageView->setName("guanqia");
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setContentSize(Size(836,430));
    pageView->setPosition(Vec2(12,30));
    pageView->setCustomScrollThreshold(40);
    
    pageView->removeAllPages();
    
    int pageCount = 3;
    int row = 2;
    int col = 6;
    float item_w = (pageView->getContentSize().width - 20)/col;
    float item_h = (pageView->getContentSize().height - 20)/row;
    float h = pageView->getContentSize().height - 40;
    char c[7];
    int level =  UserDefault::getInstance()->getIntegerForKey("level");
    for (int i = 0; i < pageCount; ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(pageView->getContentSize());
        
        for(int j=0;j<row;j++)
        {
            for(int q=0;q<col;q++)
            {
                int tag = i * row * col + j*col + q + 3;
                Button* btn = Button::create("guanqia_1.jpg");
                btn->setTag(tag);
                btn->setAnchorPoint(Vec2(0,1));
                btn->setPosition(Vec2(26+item_w*q,h - item_h*j*0.8));
                btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
                layout->addChild(btn);
                btn->setName("sel_guanqia");
                
                sprintf(c, "%d", tag-2);
                btn->setTitleColor(Color3B::RED);
                btn->setTitleFontSize(34);
                btn->setTitleText(c);
                
                if(tag > level)
                {
                    btn->loadTextureNormal("guanqia_2.jpg");
                }
                else
                {
                    sprintf(c, "%d", tag);
                    std::string level_s = "level_";
                    level_s+=c;
                    
                    auto pro = Sprite::create();
                    if(UserDefault::getInstance()->getBoolForKey(level_s.c_str()))
                    {
                        pro->initWithFile("wanmei.png");
                    }
                    else
                    {
                         pro->initWithFile("putong.png");
                    }
                    pro->setAnchorPoint(Vec2(0.5,0));
                    pro->setTag(1);
                    pro->setPosition(btn->getContentSize().width/2, 6);
                    btn->addChild(pro);
                }
            }
        }
        pageView->addPage(layout);
        
        auto dian = Sprite::create("dian_2.png");
        float b = 0;
        if(i < pageCount/2)
            b = -(pageCount/2 - i)*50;
        else if(i > pageCount/2)
             b = (i - pageCount/2)*50;
        float x = sp->getContentSize().width/2 + b;
        dian->setPosition(x, winSize.height*0.08f);
        //dian->setScale(0.7);
        dian->setTag(i+1);
        sp->addChild(dian,1);
        
        if(i == 0)
        {
            dian->initWithFile("dian_1.png");
        }
    }
    
    //pageView->removePageAtIndex(0);
    // pageView->scrollToPage(0);
    
    Button* btn = Button::create("002.png","003.png");
    btn->setAnchorPoint(Vec2(0,1));
    btn->setPosition(Vec2(10,winSize.height-10));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    bg->addChild(btn);
    btn->setName("close_guanqia");
    
    pageView->addEventListener(CC_CALLBACK_2(MainScene2D::pageViewEvent, this));
    
    sp->addChild(pageView,1);
}

bool MainScene2D::onTouchBegan(Touch* touch, Event* event)
{
    if(!isStart)
        return false;
    sel = nullptr;
    getSel(touch);
    lastDir = touch->getLocation();
    return true;
}

void MainScene2D::onTouchMoved(Touch* touch, Event* event)
{
    if(!isStart)
        return ;
   
    if(sel)
    {
        auto it = _mouses.find(touch->getID());
        
        if (it != _mouses.end())
        {
            it->second->setPosition(touch->getLocation());
        }
        lastDir = touch->getPreviousLocation();
    }
    else
    {
        getSel(touch);
    }
}

void MainScene2D::onTouchEnded(Touch* touch, Event* event)
{
    if(sel)
    {
//        bool isRemove = false;
//        for(int i=0;i<removes.size();i++)
//        {
//            if(sel == removes[i])
//            {
//                isRemove = true;
//                removes.erase(removes.begin()+i);
//                break;
//            }
//        }

        if(!isSelRemove && sel->getTag() != 2)
            sel->runAction(TintTo::create(0.2f, 255, 255, 255));
        
        auto it = _mouses.find(touch->getID());
        
        if (it != _mouses.end())
        {
            this->removeChild(it->second);
            _mouses.erase(it);
        }
        if(touch->getLocation().getDistance(lastDir) > 5)
        {
            PhysicsBody* body = (PhysicsBody*)sel->getComponent("box");
            Vec2 dir =  touch->getLocation() - lastDir;
            dir.normalize();
            body->setVelocity(dir*2000);
        }
    }
}

void MainScene2D::onEnter()
{
    Scene::onEnter();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(MainScene2D::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MainScene2D::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MainScene2D::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //初始化分数
    randZongFen();
    this->runAction(Sequence::create(DelayTime::create(1),CallFunc::create(std::bind(&MainScene2D::startGame, this,nullptr)), NULL));
     //startGame(nullptr);
    
    bool yindao = UserDefault::getInstance()->getBoolForKey("yindao");
    if(!yindao)
    {
       // UserDefault::getInstance()->setIntegerForKey("level",3);
        UserDefault::getInstance()->setBoolForKey("yindao",true);
        
        Size size = Director::getInstance()->getWinSize();
        
        auto bg = TouchLayer::create();
        bg->setName("touchLayer");
        this->addChild(bg,1001);
        
        auto diban = Sprite::create("diban.png");
        diban->setAnchorPoint(Vec2(0,1));
        diban->setPosition(size.width*0.9f,size.height);
        bg->addChild(diban);
        auto btn = Button::create("help.png");
        btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
        btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
        diban->addChild(btn);
        btn->setName("help");
        
        Sprite* shou = Sprite::create("hand.png");
        shou->setAnchorPoint(Vec2(0.5,0.5));
        shou->setRotation(30);
        shou->setPosition(Vec2(diban->getContentSize().width/2+10, -diban->getContentSize().height+20)+diban->getPosition());
        bg->addChild(shou);
        
        auto action = Sequence::create(Blink::create(1, 3),DelayTime::create(2), nullptr);
        shou->runAction(RepeatForever::create(action));
        
        //createPages();
    }
}


void MainScene2D::toggleDebug(Ref* sender)
{
    playSound("btn.wav");
#if CC_USE_PHYSICS
    _debugDraw = !_debugDraw;
    _physicsWorld->setDebugDrawMask(_debugDraw ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
#endif
}

void MainScene2D::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            playSound("btn.wav");
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
            
            if(name == "home")
            {
                _instance = nullptr;
                auto scene = TransitionFade::create(1, HomeScene::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
                stopMusic();
            }
            else if(name == "music")
            {
                bool music = UserDefault::getInstance()->getBoolForKey("music");
                UserDefault::getInstance()->setBoolForKey("music", !music);
               if(music)
               {
                    pauseMusic();
                   btn->loadTextureNormal("music2.png");
               }
               else
               {
                   resumeMusic();
                   btn->loadTextureNormal("music.png");
               }
            }
            else if(name == "sound")
            {
                bool sound = UserDefault::getInstance()->getBoolForKey("sound");
                UserDefault::getInstance()->setBoolForKey("sound", !sound);
                if(sound)
                {
                    btn->loadTextureNormal("sound2.png");
                }
                else
                {
                    btn->loadTextureNormal("sound.png");
                }
            }
            else if(name == "help")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                createPages();
            }
            else if(name == "hongbao")
            {
                createHongBao();
            }
            else if(name == "close")
            {
                Node* node = this->getChildByName("pageView");
                if(node)
                    node->removeFromParent();
                node = nullptr;
                node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
            }
            else if(name == "replay")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                startGame(nullptr);
            }
            else if(name == "fenxiang")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                fenxiang(score);
                startGame(nullptr);
            }
            else if(name == "copy")
            {
                std::string s = v_font.at(9).asString();
                copyStr(s);
            }
            else if(name == "shuaxin")
            {
                shuaxin();
            }
            else if(name == "wudi")
            {
                wudi();
            }
            else if(name == "guanqia")
            {
                guanqia();
            }
            else if(name == "close_guanqia")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
            }else if(name == "sel_guanqia")
            {
                if(!btn->getChildByTag(1))
                    break;
                
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
                UserDefault::getInstance()->setIntegerForKey("curr_level", btn->getTag());
                randZongFen();
                startGame(nullptr);
            }else if(name == "close_duihuan")
            {
                Node* node = this->getChildByName("touchLayer");
                if(node)
                    node->removeFromParent();
            }else if(name == "duihuan")
            {
                duihuanResult(btn->getTag());
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void MainScene2D::playMusic()
{
    if(UserDefault::getInstance()->getBoolForKey("music"))
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bg.mp3",true);
}

void MainScene2D::pauseMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void MainScene2D::stopMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void MainScene2D::resumeMusic()
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        playMusic();

}

void MainScene2D::playSound(std::string sound)
{
    if(UserDefault::getInstance()->getBoolForKey("sound"))
        SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
}

void MainScene2D::createPages()
{
    Node* node = this->getChildByName("pageView");
    if(node)
        node->removeFromParent();
    Size winSize = Director::getInstance()->getWinSize();
    // Create the page view
    PageView* pageView = PageView::create();
    pageView->setName("pageView");
    pageView->setDirection(PageView::Direction::VERTICAL);
    pageView->setContentSize(winSize);
    pageView->setPosition(Vec2(0,0));
    pageView->setCustomScrollThreshold(40);
    
    pageView->removeAllPages();
    
    
    int pageCount = 2;
    for (int i = 0; i < pageCount; ++i)
    {
        Layout* layout = Layout::create();
        layout->setContentSize(winSize);
    
        
        ImageView* bg = ImageView::create("bg.jpg");
        // bg->setContentSize(winSize);
        //bg->setAnchorPoint(Vec2(0.5,0.5));
        bg->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height / 2.0f));
        layout->addChild(bg);
        
        
        ImageView* huocai = ImageView::create("huochai.png");
        huocai->setRotation(80);
        huocai->setAnchorPoint(Vec2(0.5,0.5));
        huocai->setPosition(Vec2(layout->getContentSize().width / 2.0f, 80.0f));
        layout->addChild(huocai);
        
        ImageView* shou = ImageView::create("hand.png");
        shou->setAnchorPoint(Vec2(0.5,0.5));
        shou->setPosition(huocai->getPosition() + Vec2(100, -80));
        layout->addChild(shou);
        
        if(i == 0)
        {
            auto action = Sequence::create(MoveTo::create(1, huocai->getPosition() + Vec2(23, -25)),MoveTo::create(2, Vec2(huocai->getPositionX(),450) + Vec2(23, -25)),FadeOut::create(1.1), MoveTo::create(0.f,huocai->getPosition() + Vec2(100, -80)),FadeIn::create(1), nullptr);
            shou->runAction(RepeatForever::create(action));
            
            auto action2 = Sequence::create(DelayTime::create(0.9),TintTo::create(0.1, 255, 0, 0),MoveTo::create(2, Vec2(huocai->getPositionX(),450)),TintTo::create(0.1, 255, 255, 255),FadeOut::create(1), MoveTo::create(0.f,huocai->getPosition()),FadeIn::create(1), nullptr);
            huocai->runAction(RepeatForever::create(action2));
            
            ImageView* shou2 = ImageView::create("hand.png");
            shou2->setAnchorPoint(Vec2(0.5,0.5));
            shou2->setRotation(210);
            shou2->setPosition(Vec2(layout->getContentSize().width-100, 80));
            layout->addChild(shou2);
            
            std::string s = v_font.at(6).asString();
            Text* label = Text::create(s, "", 18);
            label->setColor(Color3B::RED);
            label->setPosition(shou2->getPosition() + Vec2(0, -45));
            layout->addChild(label);

            
            auto action3 = Sequence::create(Blink::create(1, 3),DelayTime::create(2), nullptr);
            shou2->runAction(RepeatForever::create(action3));
        }
        else if(i == 1){
            shou->setPosition(Vec2(huocai->getPositionX(),450) + Vec2(23, -25));
            huocai->setPosition(Vec2(huocai->getPositionX(),450));
            
            auto action = Sequence::create(EaseOut::create(MoveBy::create(0.2f, Vec2(100,100)), 0.1f),FadeOut::create(1),MoveTo::create(0.f,shou->getPosition()), FadeIn::create(1),nullptr);
            shou->runAction(RepeatForever::create(action));
            
            auto sp = Spawn::create(RotateTo::create(0.2, 90),EaseOut::create(MoveBy::create(0.2f, Vec2(180,180)), 0.1f) ,nullptr);
            auto action2 = Sequence::create(TintTo::create(0.f, 255, 0, 0),sp,FadeOut::create(1),MoveTo::create(0.f,huocai->getPosition()), TintTo::create(0.f, 255, 255, 255),RotateTo::create(0.f, 0),FadeIn::create(1),nullptr);
            huocai->runAction(RepeatForever::create(action2));
        }
        
        std::string s = v_font.at(i).asString();
        Text* label = Text::create(s, "", 30);
        label->setColor(Color3B::BLUE);
        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height * 0.9f));
        layout->addChild(label);
        
        s = v_font.at(12).asString();
        label = Text::create(s, "", 20);
        label->setColor(Color3B::BLUE);
        label->setPosition(Vec2(layout->getContentSize().width / 2.0f, layout->getContentSize().height * 0.9f-30));
        layout->addChild(label);
        
        pageView->addPage(layout);
    }
    
    //pageView->removePageAtIndex(0);
   // pageView->scrollToPage(0);
    
    Button* btn = Button::create("002.png","003.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setPosition(Vec2(winSize.width-10,winSize.height-10));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    pageView->addChild(btn);
    btn->setName("close");
    
    pageView->addEventListener(CC_CALLBACK_2(MainScene2D::pageViewEvent, this));
    
    this->addChild(pageView,100);
}

void MainScene2D::pageViewEvent(Ref *pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            if(pageView->getName() == "guanqia")
            {
                int index = pageView->getCurPageIndex() + 1;
                Sprite* sp = (Sprite*)pageView->getParent()->getChildByTag(index);
                sp->initWithFile("dian_1.png");
                
                sp = (Sprite*)pageView->getParent()->getChildByTag(index+1);
                if(sp)
                {
                    sp->initWithFile("dian_2.png");
                }
                sp = (Sprite*)pageView->getParent()->getChildByTag(index-1);
                if(sp)
                {
                    sp->initWithFile("dian_2.png");
                }
            }
        }
            break;
            
        default:
            break;
    }
}

void MainScene2D::createHongBao()
{
    Size size = Director::getInstance()->getWinSize();
    
    auto layer = TouchLayer::create();
    layer->setName("touchLayer");
    this->addChild(layer, 101);
    
    auto bg = LayerColor::create(Color4B::WHITE,size.width,size.height);
    layer->addChild(bg);
    
    Button* btn = Button::create("002.png","003.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setPosition(Vec2(size.width-10,size.height-10));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    bg->addChild(btn);
    btn->setName("close");
    
    std::string s = v_font.at(8).asString();
    Text* label = Text::create(s, "", 26);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(size.width / 2.0f, size.height * 0.9f));
    bg->addChild(label);
    
    s = v_font.at(9).asString();
    label = Text::create(s, "", 26);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(size.width / 2.0f, size.height * 0.9f-40));
    bg->addChild(label);
    
    btn = Button::create("copy.png");
    btn->setAnchorPoint(Vec2(0,0.5));
    btn->setPosition(Vec2(label->getContentSize().width/2 + label->getPositionX() + 40,label->getPositionY()));
    btn->addTouchEventListener(CC_CALLBACK_2(MainScene2D::touchEvent, this));
    bg->addChild(btn);
    btn->setName("copy");
    
    s = v_font.at(10).asString();
    label = Text::create(s, "", 36);
    label->setColor(Color3B::RED);
    label->setPosition(Vec2(size.width / 2.0f, size.height * 0.9f-100));
    bg->addChild(label);
    
    s = v_font.at(11).asString();
    label = Text::create(s, "", 20);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(size.width / 2.0f, size.height * 0.6));
    bg->addChild(label);
    

    ImageView* erweima = ImageView::create("erweima.jpg");
    erweima->setAnchorPoint(Vec2(0.5,0.5));
    erweima->setPosition(Vec2(size.width / 2.0f, size.height * 0.3));
    bg->addChild(erweima);

}

float MainScene2D::mx(float x)
{
    x -= 32;
    Size s = Director::getInstance()->getWinSize();
    float sca = (1136.0f-s.width)/2.0f;
    return x-sca;
}


//------------------------------------
static bool jump_home = false;
bool HomeScene::init()
{
    if (!Scene::init() )
    {
        return false;
    }
    
    if(jump_home)
    {
        goHome();
        return true;
    }
    Size s = Director::getInstance()->getWinSize();
    
    auto layer = LayerColor::create(Color4B::WHITE);
    this->addChild(layer);
    
    auto logo = Sprite::create("logo.png");
    logo->setPosition(s.width/2,s.height/2);
    this->addChild(logo);
    
    bool first = UserDefault::getInstance()->getBoolForKey("first");
    if(!first)
    {
        UserDefault::getInstance()->setBoolForKey("music", true);
        UserDefault::getInstance()->setBoolForKey("sound", true);
        UserDefault::getInstance()->setBoolForKey("first", true);
    }
    int day = UserDefault::getInstance()->getIntegerForKey("day");
    int curr_day = getcurrDay();
    if(day != curr_day)
    {
        UserDefault::getInstance()->setIntegerForKey("day",curr_day);
        int wudi_num = UserDefault::getInstance()->getIntegerForKey("wudi_num");
        if(wudi_num < 2)
            UserDefault::getInstance()->setIntegerForKey("wudi_num",2);
        int suaxin_num = UserDefault::getInstance()->getIntegerForKey("suaxin_num");
        if(suaxin_num < 2)
            UserDefault::getInstance()->setIntegerForKey("suaxin_num",2);
    }
    
    if(FileUtils::getInstance()->isDirectoryExist("end"))
    {
         this->runAction(Sequence::create(DelayTime::create(3),CallFunc::create(std::bind(&HomeScene::goHome, this)), NULL));
    }
    else
    {
        this->label_text = Label::createWithSystemFont("", "", 24);
        this->label_text->setTextColor(Color4B::BLACK);
        this->label_text->setPosition(3,3);
        this->label_text->setAnchorPoint(Vec2(0,0));
        this->addChild(this->label_text);
        
        
        this->_num = 0;
        this->scheduleOnce(schedule_selector(HomeScene::decData), 0.3);
        this->schedule(schedule_selector(HomeScene::updateLabel), 0.1);
       
    }

    jump_home = true;
    _instance = nullptr;
    return true;
}

void HomeScene::goHome()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto homebg = Sprite::create("bg.jpg");
    homebg->setPosition(s.width/2,s.height/2);
    this->addChild(homebg);
    
    
    auto tanzi = Sprite::create("tanzi.png");
    tanzi->setPosition(0, 80);
    this->addChild(tanzi);
    
    auto bottom = Sprite::create("bottom.png");
    bottom->setAnchorPoint(Vec2(0.5,0));
    bottom->setPosition(s.width/2, 0);
    this->addChild(bottom);
 

    Button* btn_start = Button::create("cuangguan.png");
    btn_start->setPosition(Vec2(s.width/2.0f,s.height*0.5f));
    btn_start->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    this->addChild(btn_start);
    btn_start->setName("guoguan");
    
    
   /* auto editBoxSize = Size(80, 40);
    
    btn_start = Button::create("suiji.png");
    btn_start->setPosition(Vec2(s.width/2.0f+editBoxSize.width,s.height*0.4f));
    btn_start->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    this->addChild(btn_start);
    btn_start->setName("suiji");
    
    
    auto editBg = Sprite::create("orange_edit.png");
    editBg->setPosition(btn_start->getPosition() + Vec2(-btn_start->getContentSize().width/2,0));
     this->addChild(editBg);
    // top
    _editNum = EditBox::create(editBoxSize + Size(0,20), Scale9Sprite::create());
    _editNum->setPosition(editBg->getPosition());
    _editNum->setFontColor(Color3B::BLUE);
    //_editNum->setPlaceHolder("Name:");
    _editNum->setPlaceholderFontColor(Color3B::WHITE);
    _editNum->setMaxLength(2);
    _editNum->setFontSize(editBoxSize.height*0.8f);
    _editNum->setText("3");
    _editNum->setInputMode(ui::EditBox::InputMode::NUMERIC);
    _editNum->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    _editNum->setDelegate(this);
    this->addChild(_editNum);*/

    
    auto diban = Sprite::create("diban.png");
    diban->setAnchorPoint(Vec2(0.5,1));
    diban->setPosition(s.width/3.0f+60,s.height);
    this->addChild(diban);
    
    Button* btn = Button::create("fankui.png");
    btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
    btn->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    diban->addChild(btn);
    btn->setName("fankui");
    
    
    diban = Sprite::create("diban.png");
    diban->setAnchorPoint(Vec2(0.5,1));
    diban->setPosition(s.width/3.0f*2-60,s.height);
    this->addChild(diban);
    
    btn = Button::create("fenxiang.png");
    btn->setPosition(Vec2(diban->getContentSize().width/2,diban->getContentSize().height/2));
    btn->addTouchEventListener(CC_CALLBACK_2(HomeScene::touchEvent, this));
    diban->addChild(btn);
    btn->setName("fenxiang");
    //Director::getInstance()->replaceScene(MainScene2D::create());
    
    tanzi->runAction(RepeatForever::create(Sequence::create(
                                                            MoveTo::create(12, Vec2(s.width+tanzi->getContentSize().width/2,tanzi->getPositionY())),
                                                            MoveTo::create(0, Vec2(-tanzi->getContentSize().width/2,tanzi->getPositionY())),NULL)));
}

void HomeScene::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
             MainScene2D::playSound("btn.wav");
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
           
            if(name == "guoguan")
            {
                UserDefault::getInstance()->setIntegerForKey("game_model", 1);
                auto scene = TransitionFade::create(1, MainScene2D::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
                MainScene2D::level_guoguan();
            }
            else if(name == "suiji")
            {
                int num = atoi(_editNum->getText());
                if(num > 30 || num < 3)
                {
                    std::string s = "火柴必须在3~30之间！";
                    TostLayer::show(this,s);
                    if(num > 30)
                    {
                        _editNum->setText("30");
                    }
                    else
                    {
                        _editNum->setText("3");
                    }
                    return;
                }
                UserDefault::getInstance()->setIntegerForKey("self_num", num);
                UserDefault::getInstance()->setIntegerForKey("game_model", 2);
                auto scene = TransitionFade::create(1, MainScene2D::create(), Color3B::WHITE);
                Director::getInstance()->replaceScene(scene);
                MainScene2D::level_suiji();
            }
            else if(name == "fankui")
            {
                MainScene2D::fankui();
            }
            else if(name == "fenxiang")
            {
                int sco =  UserDefault::getInstance()->getIntegerForKey("score");
                if(sco < 3)
                    sco = 3;
                MainScene2D::fenxiang(sco);
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void HomeScene::decData(float dt)
{
    std::string tar_path = GResource::getInstance()->getWritePath() + "data2.dat";
    std::string data = FileUtils::getInstance()->fullPathForFilename("data.dat");
    FileUtils::getInstance()->createDirectory(GResource::getInstance()->getWritePath());
    FileUtils::getInstance()->createDirectory(GResource::getInstance()->getWriteResPath());
    bool b = FileUtils::getInstance()->writeDataToFile(FileUtils::getInstance()->getDataFromFile(data), tar_path);
   // bool b = GResource::getInstance()->copyFile(data, tar_path);
    if(b)
    {
        b = GResource::getInstance()->decompress(tar_path, true);
        if(b)
        {
            GResource::getInstance()->removeFile(tar_path);
            
            GResource::getInstance()->createDirForUrlFile("end/", false);
             this->runAction(Sequence::create(DelayTime::create(2),CallFunc::create(std::bind(&HomeScene::goHome, this)), NULL));
        }
        else
        {
            this->unschedule(schedule_selector(HomeScene::updateLabel));
            this->label_text->setString("文件解压失败！");
        }
    }
    else
    {
        this->unschedule(schedule_selector(HomeScene::updateLabel));
        this->label_text->setString("文件拷贝失败！");
    }
}

void HomeScene::updateLabel(float dt)
{
    std::string s = "正在解压文件，请稍后";
    for(int i=0;i<this->_num;i++)
    {
        s += ".";
    }
    this->_num ++;
    this->_num = this->_num >= 4 ? 0 : this->_num;
    this->label_text->setString(s);
    
}

void HomeScene::editBoxEditingDidBegin(EditBox* editBox)
{
    //log("editBox %p DidBegin !", editBox);
}

void HomeScene::editBoxEditingDidEnd(EditBox* editBox)
{
    //log("editBox %p DidEnd !", editBox);
}

void HomeScene::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    //log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
    //int num = atoi(text.c_str());
    
}

void HomeScene::editBoxReturn(EditBox* editBox)
{
   // log("editBox %p was returned !",editBox);
   
}

int HomeScene::getcurrDay()
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    
    struct timeval now;
    struct tm* time;
    
    gettimeofday(&now, NULL);
    
    
    time = localtime(&now.tv_sec);
    //int year = time->tm_year + 1900;
    //log("year = %d",year);
    
    //char date[32] = {0};
    //sprintf(date, "%d%02d%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday);
    //log("%s",date);
    //return StringUtils::format("%s",date);
    return (int)time->tm_mday;
#endif
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    
    struct tm* tm;
    time_t timep;
    time(timep);
    
    tm = localtime(&timep);
    char date[32] = {0};
    sprintf(date, "%d-%02d-%02d",(int)time->tm_year + 1900,(int)time->tm_mon + 1,(int)time->tm_mday);
    log("%s",date);
    return StringUtils::format("%s",date);
    
#endif
    
}


bool TouchLayer::init()
{
    if (!Layer::init() )
    {
        return false;
    }
    Size s = Director::getInstance()->getWinSize();
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 160),s.width,s.height);
    this->addChild(bg);
    
    return true;
}

bool TouchLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void TouchLayer::onEnter()
{
    Layer::onEnter();
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TouchLayer::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
   
}

void TouchLayer::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(touchListener);
}

void TostLayer::show(Node* node,std::string& text)
{
    Size s = Director::getInstance()->getWinSize();
    
    Label* l_text = Label::createWithSystemFont(text, "", 20);
    
    Size l_s = l_text->getContentSize();
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 255),l_s.width+10,l_s.height+2);
    bg->setPosition(Vec2(s.width/2-bg->getContentSize().width/2,s.height/2));
    node->addChild(bg,1000);
    
    l_text->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
    bg->addChild(l_text);
 
    bg->runAction(Sequence::create(FadeIn::create(0.1f),DelayTime::create(2.0f), FadeOut::create(0.2f),RemoveSelf::create(true), nullptr));
}

MainScene2D* MainScene2D::getInstance()
{
    return _instance;
}

void MainScene2D::fenxiang(int f)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "fenxiang","(I)V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, f);
    }
#endif
}

void MainScene2D::fankui()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "fankui","()V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    }
#endif

}

void MainScene2D::level_guoguan()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "level_guoguan","()V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    }
#endif
    
}

void MainScene2D::level_suiji()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "level_suiji","()V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    }
#endif
    
}

void MainScene2D::copyStr(std::string str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "copyStr","(Ljava/lang/String;)V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        jstring s = methodInfo.env->NewStringUTF(str.c_str());
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,s);
    }
#endif
    
}

void MainScene2D::showAd(int i)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "com/xugu/bonimei/AppActivity", "showAd","(I)V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, i);
    }
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
    JNIEXPORT void JNICALL Java_com_xugu_bonimei_AppActivity_shareSuccess(JNIEnv *env, jobject obj)
    {
        if(MainScene2D::getInstance())
        {
            int model = UserDefault::getInstance()->getIntegerForKey("game_model");
            if(model == 1)
            {
                int level =  UserDefault::getInstance()->getIntegerForKey("level");
                int curr_level =  UserDefault::getInstance()->getIntegerForKey("curr_level");
                if(curr_level == level && level < MAX_LEVEL)
                {
                    UserDefault::getInstance()->setIntegerForKey("level", level+1);
                    UserDefault::getInstance()->setIntegerForKey("curr_level", curr_level+1);
                }
                if(curr_level < level)
                    UserDefault::getInstance()->setIntegerForKey("curr_level", curr_level+1);
               
            }
            //初始化分数
            MainScene2D::getInstance()->randZongFen();
            MainScene2D::getInstance()->startGame(nullptr);
        }
               return;
    }

    JNIEXPORT void JNICALL Java_com_xugu_bonimei_AppActivity_sharefailure(JNIEnv *env, jobject obj)
    {
        if(MainScene2D::getInstance())
            MainScene2D::getInstance()->startGame(nullptr);
        return;
    }
}
#endif