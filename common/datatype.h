/************************************************************
    File Name : datatype.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/06 20:24:13
************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H

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
    char name[20];
    int flag;                  // 未响应的次数
    struct sockaddr_in *addr;  // MAYBE BUG
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

#endif