/************************************************************
    File Name : ball_status.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/02 18:18:47
************************************************************/

#include <math.h>

#include "game.h"
#include "head.h"
#define PI 3.1415926

extern WINDOW *Message;
extern struct Bpoint ball;
extern struct BallStatus ball_status;

int can_kick(struct Point *loc, int strength) {
    // 因为球和人的坐标不在同一个窗口上(人可以出界) 所以先进行坐标对齐
    int px = loc->x, py = loc->y;
    char buff[100] = {0};
    sprintf(buff, "[ORI]x=%d, y=%d  [FIX]x=%d, y=%d  [BALL]x=%d, y = %d",
            loc->x, loc->y, px, py, (int)ball.x, (int)ball.y);
    Show_Message(, , buff, 1);
    if (px == (int)ball.x && py == (int)ball.y) {
        return 0;
    }
    if (abs(px - (int)ball.x) > 2 || abs(py - (int)ball.y) > 2) {
        return 0;
    }
    double dx =
        (ball.x - px) / pow(pow(ball.x - px, 2) + pow(ball.y - py, 2), 0.5);
    double dy =
        (ball.y - py) / pow(pow(ball.x - px, 2) + pow(ball.y - py, 2), 0.5);
    ball_status.a.x = -3 * dx;
    ball_status.a.y = -3 * dy;
    ball_status.v.x = strength * 40 * 0.2 * dx;
    ball_status.v.y = strength * 40 * 0.2 * dy;
    return 1;
}