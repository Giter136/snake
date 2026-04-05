#include<iostream>
#include<easyx.h>
#include<vector>
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
protected:
    int m_x;
    int m_y;
    COLORREF m_color;
};

class snake: public item{
public:
    snake():item(){};
    snake(int x,int y):item(x,y){
        node.push_back(item(20,0));
        node.push_back(item(10,0));
        node.push_back(item(0,0));
    };
    void draw() override{
        for(int i=0;i<node.size();i++){
            node[i].draw();//重写虚函数，绘制蛇身体节点
        }
    }
    void moveBody(){

    }
    int dir;
private:
    std::vector<item> node;//蛇身数组用于存储蛇身体节点位置
};

class game{
public:
    void run(){
        snk.draw();
    }
    void onMsg(const ExMessage& msg){
        if(msg.message==WM_KEYDOWN){
            snk.dir=msg.vkcode;
        }
    }
private:
    snake snk;
};

int main(){
    initgraph(1000,1000);
    game gm;
    while(1){

        gm.run();
        Sleep(100);
    }
    return 0;
}