/************************************************************
    File Name : client_re_draw.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/05 16:08:47
************************************************************/

#include <cjson/cJSON.h>

#include "datatype.h"
#include "game.h"
#include "head.h"

extern WINDOW *Football, *Score;

void* __re_draw_court(void* arg);
void draw_player(cJSON* court_obj, const char* team_color);

// 新建线程重绘球场/球员
void re_draw_court(char* court_json) {
    pthread_t tid;
    char* json = (char*)calloc(2048, 1);
    strcpy(json, court_json);
    pthread_create(&tid, NULL, __re_draw_court, (void*)json);
}

// 重绘球场/球员
void* __re_draw_court(void* arg) {
    pthread_t tid = pthread_self();
    char* court_json = (char*)arg;

    pthread_detach(tid);
    werase(Football_t);
    box(Football_t, 0, 0);
    box(Football, 0, 0);

    cJSON* court_obj = cJSON_Parse(court_json);
    if (court_obj == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON Parse Error before: %s\n", error_ptr);
        }
        return NULL;
    }

    // 红队
    wattron(Football_t, COLOR_PAIR(2));
    draw_player(court_obj, "red");
    // 蓝队
    wattron(Football_t, COLOR_PAIR(6));
    draw_player(court_obj, "blue");

    // 球
    wattron(Football_t, COLOR_PAIR(3));
    cJSON* ball_obj = cJSON_GetObjectItemCaseSensitive(court_obj, "ball");
    cJSON* ball_x = cJSON_GetObjectItemCaseSensitive(ball_obj, "x");
    cJSON* ball_y = cJSON_GetObjectItemCaseSensitive(ball_obj, "y");
    if (ball_x == NULL || !cJSON_IsNumber(ball_x)) return NULL;
    if (ball_y == NULL || !cJSON_IsNumber(ball_y)) return NULL;
    w_gotoxy_putc(Football, (int)ball_x->valuedouble, (int)ball_y->valuedouble,
                  'o');

    wrefresh(Football_t);
    return NULL;
}

void re_draw_score() { return; }

// 绘制球员
void draw_player(cJSON* court_obj, const char* team_color) {
    const cJSON* player = NULL;
    cJSON* team_obj = cJSON_GetObjectItemCaseSensitive(court_obj, team_color);
    if (team_obj == NULL && !cJSON_IsArray(team_obj)) return;
    cJSON_ArrayForEach(player, team_obj) {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(player, "name");
        cJSON* x = cJSON_GetObjectItemCaseSensitive(player, "x");
        cJSON* y = cJSON_GetObjectItemCaseSensitive(player, "y");
        if (name == NULL || !cJSON_IsString(name)) return;
        if (x == NULL || !cJSON_IsNumber(x)) return;
        if (y == NULL || !cJSON_IsNumber(y)) return;
        char p = 'K';
        int pos_x = (int)x->valuedouble, pos_y = y->valuedouble;
        w_gotoxy_putc(Football_t, pos_x, pos_y, p);
        w_gotoxy_puts(Football_t, pos_x + 1, pos_y - 1, name->valuestring);
    }

    return;
}