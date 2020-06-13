/************************************************************
    File Name : game.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/02 18:35:46
************************************************************/

#ifndef _GAME_H
#define _GAME_H

#include "head.h"
#define MAX 50  // 最大游戏人数/队

struct Map court;

WINDOW *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int height, int start_x, int start_y);
void destory_win(WINDOW *win);

void gotoxy(int x, int y);
void gotoxy_puc(int x, int y, int c);
void gotoxy_puts(int x, int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x, int y, int c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);

void init_football();
void *draw(void *arg);

#endif