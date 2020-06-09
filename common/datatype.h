/************************************************************
    File Name : datatype.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/06 20:24:13
************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H

#define MAXMSG 1024

struct LogRequest {
    char name[20];
    int team;  // 0 RED 1 BLUE
    char msg[512];
};

struct LogResponse {
    int type;  // 0 SUCCESS 1 FAILED
    char msg[512];
};

struct Point {
    int x, y;
};

struct User {
    int team;
    int fd;
    int flag;  // 未响应的次数
    char name[20];
    struct Point loc;
};

struct Map {
    int width;
    int height;
    struct Point start;
    int gate_width;
    int gate_height;
};

struct Ctrl {
    int carry;
    int kick;
};

struct TransMsg {
    int dirx;
    int diry;
    struct Ctrl ctrl;
};

#define FT_TEST 0x01  // 心跳
#define FT_WALL 0x02  // 广播
#define FT_MSG 0x04
#define FT_ACK 0x08

struct FootBallMsg {
    int type;
    int size;
    char msg[MAXMSG];
};

#endif