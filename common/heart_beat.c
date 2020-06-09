/************************************************************
    File Name : heart_beat.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/09 20:21:45
************************************************************/

#include "datatype.h"
#include "head.h"

#define MAX 50

extern struct User *rteam, *bteam;

void heart_beat_team(struct User *team) {
    struct FootBallMsg msg;
    msg.type = FT_TEST;
    for (int i = 0; i < MAX; ++i) {
        if (team[i].online) {
            if (!team[i].flag) {  // 超过最大尝试次数
                team[i].online = 0;
                continue;
            }
            send(team[i].fd, (void *)&msg, sizeof(msg), 0);
        }
    }
    return;
}

void *heart_beat(void *arg) {
    while (1) {
        sleep(10);
        heart_beat_team(bteam);
        heart_beat_team(rteam);
    }
    return NULL;
}