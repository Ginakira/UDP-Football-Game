/************************************************************
    File Name : client_recver.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/13 14:13:25
************************************************************/

#include "common.h"
#include "head.h"

extern int sockfd;

void *client_recv(void *arg) {
    while (1) {
        struct FootBallMsg msg;
        bzero(&msg, sizeof(msg));
        recv(sockfd, (void *)&msg, sizeof(msg), 0);
        if (msg.type & FT_TEST) {
            DBG(RED "HeartBeat from Server 💗" NONE "\n");
            msg.type = FT_ACK;
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & (FT_MSG | FT_WALL)) {
            DBG(GREEN "Server Msg : " NONE "%s\n", msg.msg);
        } else if (msg.type & FT_FIN) {
            DBG(GREEN "Server is going to stop!" NONE "\n");
            endwin();
            exit(0);
        } else {
            DBG(GREEN "Server Msg : " NONE "⚠️Unsupported message type.\n");
        }
    }
    return NULL;
}