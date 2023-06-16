#include "CTurtle.hpp"
namespace ct = cturtle;
ct::InteractiveTurtleScreen scr(900, 900, "jing_zi_qi");
ct::Turtle **Creatpens(ct::Turtle **&Qtts)
{
    for (int i = 0; i < 9; i++)
    {
        Qtts[i] = new ct::Turtle(scr);
        Qtts[i]->hideturtle();
        Qtts[i]->speed(ct::TS_FASTEST);
        Qtts[i]->pencolor({223, 225, 0});
    }
    return Qtts;
}
// 绘制棋盘
void bgline()
{
    static ct::Turtle t(scr);
    t.pencolor({223, 225, 0});
    t.speed(ct::TS_FASTEST);
    scr.tracer(0, 10);
    t.hideturtle();
    t.penup();
    t.goTo(-150, -450);
    t.pendown();
    t.setheading(90);
    t.forward(900);

    t.penup();
    t.goTo(150, -450);
    t.pendown();
    t.setheading(90);
    t.forward(900);

    t.penup();
    t.goTo(-450, -150);
    t.pendown();
    t.setheading(0);
    t.forward(900);

    t.penup();
    t.goTo(-450, 150);
    t.pendown();
    t.setheading(0);
    t.forward(900);
    scr.tracer(1, 10);
}
// 绘制棋子
void cha(ct::Turtle *pen)
{
    scr.tracer(0, 10);
    pen->setheading(45);
    pen->forward(100);
    pen->back(100);
    pen->right(90);
    pen->forward(100);
    pen->back(100);
    pen->right(90);
    pen->forward(100);
    pen->back(100);
    pen->right(90);
    pen->forward(100);
    pen->back(100);
    pen->right(90);
    scr.tracer(1, 10);
}
// 绘制棋子
void Rectangle(ct::Turtle *pen)
{
    scr.tracer(0, 10);
    pen->penup();
    pen->setheading(90);
    pen->forward(50);
    pen->right(90);
    pen->forward(50);
    pen->pendown();

    pen->right(90);
    pen->forward(100);
    pen->right(90);
    pen->forward(100);
    pen->right(90);
    pen->forward(100);
    pen->right(90);
    pen->forward(100);
    scr.tracer(1, 10);
}
// 点击类，对点击进行反应
class ClickPosition
{
private:
    int x;
    int y;
    int count;         // 记录棋子个数
    ct::Turtle **pens; // 传入笔
    int Value[3][3];   // 记录棋盘数据

public:
    ClickPosition(ct::Turtle **ppens, int px = 0, int py = 0) // 构造函数，对数据初始化
    {
        count = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Value[i][j] = 0;
            }
        }
        pens = ppens;
    }
    void SetClickPostion(int px, int py);  // 设置点击成员坐标
    void recordpi();                       // （主要）规划点击后的调用流程
    int dmArea();                          // 判断点击位置属于那个区域
    int end();                             // 判断是否对局结束
    void DrawWinLine(int Mode, int order); // 绘制赢家连线
    void endgame(int winner);              // 结束游戏，显示赢家
};
int main()
{
    ct::Image img("background.jpeg");
    img.resize(900, 900);
    scr.bgpic(img);
    ct::Turtle **pens = new ct::Turtle *[9];
    pens = Creatpens(pens);
    bgline();
    // 将笔就位
    int n = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pens[n]->penup();
            pens[n]->goTo(-300 + 300 * j, 300 - 300 * i);
            pens[n]->pendown();
            n++;
        }
    } // 就位结束

    ClickPosition Po(pens);
    // 首先传入笔，再通过onclick传入点击坐标
    std::function<void(int, int)> M_Po_SetClickPostion = std::bind(&ClickPosition::SetClickPostion, &Po, std::placeholders::_1, std::placeholders::_2);
    scr.onclick(M_Po_SetClickPostion);
    scr.mainloop();
    return 0;
}

void ClickPosition::SetClickPostion(int px, int py)
{
    std::cout << "点击的位置为：(" << px << ' ' << py << ")" << std::endl;
    x = px;
    y = py;
    if (this->dmArea() == -1) // 点击到棋线上
        return;
    this->recordpi();
}

void ClickPosition::recordpi()
{
    if (count % 2 == 0)
    {
        if (!Value[0][this->dmArea() - 1]) // 该位置没有被下过棋，进入绘制
        {
            count++;
            cha(pens[this->dmArea() - 1]);                                                                    // 绘制棋子
            Value[0][this->dmArea() - 1] = 1;                                                                 // 记录棋局数据
            std::cout << "第" << this->dmArea() << "号的值变为" << Value[0][this->dmArea() - 1] << std::endl; // 提示输出
            // 判断是否结束
            if (this->end() == 1)
            {
                std::cout << "cha win" << std::endl;
                this->endgame(1);
            }
            else if (this->end() == 3)
            {
                std::cout << "nobody win" << std::endl;
                this->endgame(3);
            }
        }
        else // 该位置下过棋，退出，继续监听点击
            return;
    }
    else // 逻辑同上
    {
        if (!Value[0][this->dmArea() - 1])
        {
            count++;
            Rectangle(pens[this->dmArea() - 1]);
            Value[0][this->dmArea() - 1] = -1;
            std::cout << "第" << this->dmArea() << "号的值为" << Value[0][this->dmArea() - 1] << std::endl;
            if (this->end() == -1)
            {
                std::cout << "rectangle win" << std::endl;
                this->endgame(-1);
            }
            else if (this->end() == 3)
            {
                std::cout << "nobody win" << std::endl;
                this->endgame(3);
            }
        }
        else
            return;
    }
}

int ClickPosition::dmArea()
{
    /*棋局格子标号
    -------------
    | 1 | 2 | 3 |
    ----+---+----
    | 4 | 5 | 6 |
    ----+---+----
    | 7 | 8 | 9 |*/
    if (x > 150)
    {
        if (y > 150)
            return 3;
        if (y < 150 && y > -150)
            return 6;
        if (y < -150)
            return 9;
    }
    else if (x < 150 && x > -150)
    {
        if (y > 150)
            return 2;
        if (y < 150 && y > -150)
            return 5;
        if (y < -150)
            return 8;
    }
    else if (x < -150)
    {
        if (y > 150)
            return 1;
        if (y < 150 && y > -150)
            return 4;
        if (y < -150)
            return 7;
    }
    return -1; // 点到棋线上
}

int ClickPosition::end()
{
    // 计算行、列、对角线的数据和
    int isum[3] = {};
    int jsum[3] = {};
    int xsum[3] = {};
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            isum[i] += Value[i][j];
        }
    }
    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            jsum[j] += Value[i][j];
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == j)
            {
                xsum[0] += Value[i][j];
            }
            else if (i + j == 3)
            {
                xsum[1] += Value[i][j];
            }
        }
    }
    // 判断赢家
    for (int i = 0; i < 3; i++)
    {
        if (isum[i] == 3 || jsum[i] == 3 || xsum[i] == 3)
        {
            if (isum[i] == 3)
                this->DrawWinLine(1, i);
            if (jsum[i] == 3)
                this->DrawWinLine(2, i);
            if (xsum[i] == 3)
                this->DrawWinLine(3, i);
            return 1;
        }
        else if (isum[i] == -3 || jsum[i] == -3 || xsum[i] == -3)
        {
            if (isum[i] == -3)
                this->DrawWinLine(1, i);
            if (jsum[i] == -3)
                this->DrawWinLine(2, i);
            if (xsum[i] == -3)
                this->DrawWinLine(3, i);
            return -1;
        }
    }
    // 判断平局
    if (count == 9)
        return 3;
    return 0;
}

void ClickPosition::DrawWinLine(int Mode, int order) // mode == 1 is hang, mode ==2 is lie, 3 is x
{
    static ct::Turtle winTurtle(scr);
    winTurtle.penup();
    winTurtle.pencolor({223, 225, 0});
    if (Mode == 2)
    {
        winTurtle.goTo(-300 + 300 * order, 375);
        winTurtle.pendown();
        winTurtle.setheading(-90);
        winTurtle.forward(750);
    }
    if (Mode == 1)
    {
        winTurtle.goTo(-375, 300 - 300 * order);
        winTurtle.pendown();
        winTurtle.setheading(0);
        winTurtle.forward(750);
    }
    if (Mode == 3)
    {
        if (order == 0)
        {
            winTurtle.goTo(-375, 375);
            winTurtle.setheading(-45);
            winTurtle.pendown();
            winTurtle.forward(1080);
        }
        if (order == 1)
        {
            winTurtle.goTo(-375, -375);
            winTurtle.setheading(45);
            winTurtle.pendown();
            winTurtle.forward(1080);
        }
    }
}

void ClickPosition::endgame(int winer)
{
    scr.resetscreen();
    static ct::Turtle endgame(scr);
    switch (winer)
    {
    case 1:
        endgame.write("Cha win!", "default", {223,225,0}, 5.0f);
        break;
    case -1:
        endgame.write("Fang Win!", "default",{223,225,0}, 5.0f);
        break;
    case 3:
        endgame.write("Nobody Win!", "default", {223,225,0}, 5.0f);
        break;
    }
}
