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

void re_draw() {
    wclear(Football);
    box(Football, 0, 0);
    re_draw_team(rteam);
    re_draw_team(bteam);
    return;
}