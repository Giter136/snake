#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>
#include<unordered_map>
#define size_x 500
#define size_y 500
class item{
public:
    item():item(0,0) {};
    item(int x,int y):m_x(x),m_y(y),m_color(RED){};
    virtual void draw(){
        setfillcolor(m_color);//设置填充颜色
        fillrectangle(m_x,m_y,m_x+10,m_y+10);//画一个矩形
    }
    void move(int x,int y){
        m_x+=x;
        m_y+=y;
    }
    bool collision(const item& itm){
        return this->m_x==itm.m_x && this->m_y==itm.m_y;
    }
    bool boardDectx1(){
        return m_x>size_x;
    }
    bool boardDectx2(){
        return m_x<0;
    }
    bool boardDecty1(){
        return m_y>size_y;
    }
    bool boardDecty2(){
        return m_y<0;
    }
protected:
    int m_x;
    int m_y;
    COLORREF m_color;
};
//设计一个食物类
class food:public item{
public:
    food():item(){
        m_x=rand()%50*10;
        m_y=rand()%50*10;
    }
    void draw()override{
        setfillcolor(m_color);
        solidellipse(m_x,m_y,m_x+10,m_y+10);
    }
    void changeposition(){
        m_x=rand()%50*10;
        m_y=rand()%50*10;
    }
};

class snake: public item{
public:
    snake():snake(0,0){};
    snake(int x,int y):item(x,y),dir(VK_RIGHT){
        node.push_back(item(20,0));
        node.push_back(item(10,0));
        node.push_back(item(0,0));
    };
    void draw() override{
        for(size_t i=0;i<node.size();i++){
            node[i].draw();//重写虚函数，绘制蛇身体节点
        }
    }
    void moveBody(){
        for(size_t i=node.size()-1;i>0;i--){
            node[i]=node[i-1];
            //每一节在更新的时候都会变成上一节的位置。为什么从尾巴开始遍历呢？
            //因为从头开始的话就永远只会有两节了因为是链式地赋值了整个节点数组
        }
        switch(dir){
            case VK_UP:{
                node[0].move(0,-10);
                break;
            }
            case VK_DOWN:{
                node[0].move(0,10);
                break;
            }
            case VK_RIGHT:{
                node[0].move(10,0);
                break;
            }
            case VK_LEFT:{
                node[0].move(-10,0);
                break;
            }
            default: break;
        }
    }
    bool collision(const item& itm){
        return node[0].collision(itm);
    }
    void increase(){
        node.push_back(item());
    }
    void boardSkip(){
        if(node[0].boardDectx1()){
            node[0].move(-size_x,0);
        }
        else if(node[0].boardDectx2()){
            node[0].move(size_x,0);
        }
        else if(node[0].boardDecty1()){
            node[0].move(0,-size_y);
        }
        else if(node[0].boardDecty2()){
            node[0].move(0,size_y);
        }
    }
    BYTE dir;
private:
    std::vector<item> node;//蛇身数组用于存储蛇身体节点位置
};

class game{
public:
    game(){
        um.insert(std::make_pair(VK_DOWN,VK_UP));
        um.insert(std::make_pair(VK_UP,VK_DOWN));
        um.insert(std::make_pair(VK_RIGHT,VK_LEFT));
        um.insert(std::make_pair(VK_LEFT,VK_RIGHT));
    }
    void run(){
        BeginBatchDraw();//这是什么东西？
        cleardevice();//这是什么东西？
        snk.moveBody();
        snk.boardSkip();//先位移，再跳跃，再检测是否吃到食物
        snakeEatFood();
        fd.draw();
        snk.draw();
        FlushBatchDraw();//把缓冲区画面真正显示到窗口
        ExMessage msg{};//创建一个消息对象.视频中使用={0}初始化，但是在我这会报错，暂时选择不初始化。
        while(peekmessage(&msg,EX_KEY)){//每一帧获取一次消息给msg对象,但是外面用的就是while true了，为什么这里选择使用while而不是if呢？
            onMsg(msg);
        }
        EndBatchDraw();//这是什么东西？
    }
    void onMsg(const ExMessage& msg){
        if(msg.message==WM_KEYDOWN){//监听键盘消息
            if(msg.vkcode!=um[snk.dir]&&(msg.vkcode==VK_UP||msg.vkcode==VK_DOWN||msg.vkcode==VK_LEFT||msg.vkcode==VK_RIGHT))
            snk.dir=msg.vkcode;
        }
    }
    void snakeEatFood(){
        if(snk.collision(fd)){
            //蛇节数增加
            snk.increase();
            //食物重新随机生成一个
            fd.changeposition();
        }
    }
private:
    snake snk;
    food fd;
    std::unordered_map<BYTE,BYTE> um;
};

int main(){
    srand(time(0));
    initgraph(size_x,size_y);
    game gm;//构建一个大的游戏类有什么好处吗？
    while(1){
        gm.run();
        Sleep(70);
    }
    return 0;
}
//之前有个错误导致会不停变长是什么错误？