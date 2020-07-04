/************************************************************
    File Name : thread_pool.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/09 18:21:54
************************************************************/

#include "thread_pool.h"

#include "ball_status.h"
#include "game.h"
#include "show_data_stream.h"
#include "udp_epoll.h"

extern int repollfd, bepollfd;
extern struct BallStatus ball_status;

void do_echo(struct User *user) {
    struct FootBallMsg msg;
    char tmp[512] = {0};
    int size = recv(user->fd, (void *)&msg, sizeof(msg), 0);
    user->flag = 10;
    if (msg.type & FT_ACK) {  // 客户端心跳回执
        if (user->team) {     // Blue Team
            DBG(L_BLUE " %s " NONE "❤️\n", user->name);
        } else {
            DBG(L_RED " %s " NONE "❤️\n", user->name);
        }
    } else if (msg.type & (FT_WALL | FT_MSG)) {
        if (user->team) {  // Blue Team
            DBG(L_BLUE " %s : %s" NONE "\n", user->name, msg.msg);
        } else {
            DBG(L_RED " %s : %s" NONE "\n", user->name, msg.msg);
        }
        strcpy(msg.name, user->name);
        msg.team = user->team;
        Show_Message(, user, msg.msg, );
        send(user->fd, (void *)&msg, sizeof(msg), 0);
    } else if (msg.type & FT_FIN) {  // 客户端下线信息
        DBG(RED "%s logout" NONE "\n", user->name);
        sprintf(tmp, "%s logout!", user->name);
        user->online = 0;
        int epollfd_tmp = (user->team ? bepollfd : repollfd);
        del_event(epollfd_tmp, user->fd);
        Show_Message(, NULL, tmp, 1);
    } else if (msg.type & FT_CTL) {  // 客户端控制信息
        show_data_stream('n');
        Show_Message(, user, "Ctrl Messgae", 0);
        if (msg.ctl.dirx || msg.ctl.diry) {
            // 人-边界判断
            int tmpx = user->loc.x + msg.ctl.dirx,
                tmpy = user->loc.y + msg.ctl.diry;
            if (tmpx >= 0 && tmpx < court.width + 4) user->loc.x = tmpx;
            if (tmpy >= 0 && tmpy < court.height + 2) user->loc.y = tmpy;
        }

        // 控球动作
        if (msg.ctl.action & ACTION_KICK) {
            show_data_stream('k');
            int ret = can_kick(&(user->loc), msg.ctl.strength);
            char buff[50] = {0};
            sprintf(buff, "Can kick = %d", ret);
            Show_Message(, , buff, 1);
            if (ret) {
                ball_status.who = user->team;
                strcpy(ball_status.name, user->name);
            }
        } else if (msg.ctl.action & ACTION_STOP) {
            show_data_stream('s');
            int ret = can_access(&(user->loc));
            if (ret) {
                ball_status.who = user->team;
                ball_status.v.x = ball_status.v.y = 0;
                ball_status.a.x = ball_status.a.y = 0;
            }
            char buff[50] = {0};
            sprintf(buff, "Can stop = %d", ret);
            Show_Message(, , buff, 1);
        }
    }
    return;
}
void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd) {
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *));
    taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
    return;
}

void task_queue_push(struct task_queue *taskQueue, struct User *user) {
    pthread_mutex_lock(&taskQueue->mutex);
    taskQueue->team[taskQueue->tail] = user;
    DBG(L_GREEN "Thread pool : " NONE "Task Push %s\n", user->name);
    if (++taskQueue->tail == taskQueue->sum) {
        DBG(L_GREEN "Thread pool : " NONE "Task Queue End.\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
    return;
}

struct User *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    while (taskQueue->tail == taskQueue->head) {
        DBG(L_GREEN "Thread pool : " NONE
                    "Task Queue Empty, waiting for task...\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    struct User *user = taskQueue->team[taskQueue->head];
    DBG(L_GREEN "Thread Pool : " NONE "Task Pop %s.\n", user->name);
    if (++taskQueue->head == taskQueue->sum) {
        DBG(L_GREEN "Thread pool : " NONE "Task Queue End.\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run(void *arg) {
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        struct User *user = task_queue_pop(taskQueue);
        do_echo(user);
    }
    return NULL;
}