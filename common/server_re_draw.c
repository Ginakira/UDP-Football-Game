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
        wattron(Football, COLOR_PAIR(6));
    } else {
        wattron(Football, COLOR_PAIR(2));
    }
    w_gotoxy_putc(Football, loc->x, loc->y, p);
    w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);
}

void re_draw_team(struct User *team) {
    for (int i = 0; i < MAX; ++i) {
        if (!team[i].online) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }
}

void re_draw_ball() {
    if (ball_status.v.x != 0 || ball_status.v.y != 0) {
        double t = 0.1;
        ball.x = ball.x + ball_status.v.x * t + 0.5 * ball_status.a.x * t * t;
        ball.y = ball.y + ball_status.v.y * t + 0.5 * ball_status.a.y * t * t;
        ball_status.v.x += ball_status.a.x * t;
        ball_status.v.y += ball_status.a.y * t;
        char buff[100] = {0};
        sprintf(buff, "[BALL]x=%d, y = %d", (int)ball.x, (int)ball.y);
        Show_Message(, , buff, 1);
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