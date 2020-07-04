/************************************************************
    File Name : server_re_draw.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/14 20:30:07
************************************************************/

#include "game.h"
#include "head.h"
#define MAX 50

extern struct User *rteam, *bteam;
extern WINDOW *Football;
extern struct Bpoint ball;
extern struct BallStatus ball_status;

void re_draw_player(int team, char *name, struct Point *loc) {
    char p = 'K';
    if (team) {  // Blue Team
        wattron(Football_t, COLOR_PAIR(6));
    } else {
        wattron(Football_t, COLOR_PAIR(2));
    }
    w_gotoxy_putc(Football_t, loc->x, loc->y, p);
    w_gotoxy_puts(Football_t, loc->x + 1, loc->y - 1, name);
}

void re_draw_team(struct User *team) {
    for (int i = 0; i < MAX; ++i) {
        if (!team[i].online) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }
}

void re_draw_ball() {
    if (ball_status.v.x || ball_status.v.y) {
        double t = 0.1;
        // 计算下一画面位置
        ball.x = ball.x + ball_status.v.x * t + 0.5 * ball_status.a.x * t * t;
        ball.y = ball.y + ball_status.v.y * t + 0.5 * ball_status.a.y * t * t;
        // 计算下一画面的速度
        double next_vx = ball_status.v.x + ball_status.a.x * t;
        double next_vy = ball_status.v.y + ball_status.a.y * t;

        // 判断前后速度异号 如异号则代表已减速到0
        if (next_vx * ball_status.v.x < 0) {
            ball_status.a.x = 0;
            ball_status.v.x = 0;
        } else {
            ball_status.v.x = next_vx;
        }

        if (next_vy * ball_status.v.y < 0) {
            ball_status.a.y = 0;
            ball_status.v.y = 0;
        } else {
            ball_status.v.y = next_vy;
        }

        char buff[100] = {0};
        sprintf(
            buff,
            "[BALL]x=%d, y = %d, [SPD]vx=%.2f, vy=%.2f, [ASPD]ax=%.2f, ay=%.2f",
            (int)ball.x, (int)ball.y, ball_status.v.x, ball_status.v.y,
            ball_status.a.x, ball_status.a.y);
        Show_Message(, , buff, 1);
    }

    // 出界 停止运动 停留在边界
    int out_x = (ball.x < 0 || ball.x > court.width);
    int out_y = (ball.y < 0 || ball.y > court.height);
    if (out_x || out_y) {
        ball_status.a.x = ball_status.a.y = 0;
        ball_status.v.x = ball_status.v.y = 0;
        if (out_x) ball.x = ball.x < 0 ? 0 : court.width - 1;
        if (out_y) ball.y = ball.y < 0 ? 0 : court.height - 1;
    }

    w_gotoxy_putc(Football, (int)ball.x, (int)ball.y, 'o');
    return;
}

void re_draw() {
    werase(Football_t);
    box(Football_t, 0, 0);
    box(Football, 0, 0);
    re_draw_ball();
    re_draw_team(rteam);
    re_draw_team(bteam);
    wrefresh(Football_t);
    return;
}