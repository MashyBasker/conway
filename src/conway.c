#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>

#include "conway.h"

int ARENA[HEIGHT][WIDTH] = { 0 };

void generate_random_arena()
{
    for(size_t y = 0; y < HEIGHT / 2; y++) {
        for(size_t x = 0; x < WIDTH / 2; x++) {
            ARENA[y][x] = rand() % 2;
        }
    }
}

void dead_rules(int y, int x) 
{   
    int ones = 0;
    int change1[3] = { -1, 0, 1 };
    int change2[3] = { -1, 0, 1 };

    /* Iterating over all neighbours */
    for(size_t j = 0; j < 3; j++) {
        for(size_t i = 0; i < 3; i++) {
            if(i == 0 && j == 0) continue;
            else {
                int neighbour = ARENA[y + change1[j]][x + change2[i]];
                if(neighbour == 1) {
                    ones++;
                }
            }
        }
    }
    /* If the number of alive cells around a dead cell == 3 */
    if(ones == over_lim) {
        ARENA[y][x] = 1;
    }
}

void alive_rules(int y, int x)
{
    int ones = 0;
    int change1[3] = { -1, 0, 1 };
    int change2[3] = { -1, 0, 1 };

    /* Iterating over all neighbours */
    for(size_t j = 0; j < 3; j++) {
        for(size_t i = 0; i < 3; i++) {
            if(i == 0 && j == 0) continue;
            else {
                int neighbour = ARENA[y + change1[j]][x + change2[i]];
                if(neighbour == 1) {
                    ones++;
                }
            }
        }
    }
    if(ones == 2 || ones == 3) {
        ARENA[y][x] = 1;
    } else {
        ARENA[y][x] = 0;
    }
}

void update_arena()
{
    for(size_t y = 0; y < HEIGHT; y++) {
        for(size_t x = 0; x < WIDTH; x++) {
            if(ARENA[y][x] == 0) {
                dead_rules(y, x);
            } else if(ARENA[y][x] == 1) {
                alive_rules(y, x);
            }
        }
    }
}

int delay_milli(long t)
{
    struct timespec ts;
    int ret;
    if(t < 0) {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = t / 1000;
    ts.tv_nsec = (t % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while(ret && errno == EINTR);

    return ret;
}

void display_arena()
{
    int ret = system("clear");
    if(ret == -1) {
        perror("[Error] `system` command unsuccessful");
    }
    for(size_t y = 0; y < HEIGHT; y++) {
        for(size_t x = 0; x < WIDTH; x++) {
            char c = symbol[ARENA[y][x]];
            
            fputc(c, stdout);
        }
        fputc('\n', stdout);
    }
}

int main()
{
    srand(time(0));
    generate_random_arena();
    display_arena();
    long long it = 0;
    for(;;) {
        update_arena();
        display_arena();
        printf("Iteration: %lld\n\n", it++);
        if(delay_milli(100) == -1) {
            perror("[error] could not delay");
        }
    }
    return 0;
}