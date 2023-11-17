#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "../MultiTimer.h"

MultiTimer timer1;
MultiTimer timer2;
MultiTimer timer3;
MultiTimer timer4;

uint64_t PlatformTicksGetFunc(void)
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return (uint64_t)((current_time.tv_sec * 1000) + (current_time.tv_nsec / 1000000));
}

void exampleTimer1Callback(MultiTimer* timer, void *userData)
{
    printf("[%012ld] Timer:%p callback-> %s.\r\n", PlatformTicksGetFunc(), timer, (char*)userData);
    MultiTimerStart(timer, 1000, exampleTimer1Callback, userData);
}

void exampleTimer2Callback(MultiTimer* timer, void *userData)
{
    printf("[%012ld] Timer:%p callback-> %s.\r\n", PlatformTicksGetFunc(), timer, (char*)userData);
}

void exampleTimer3Callback(MultiTimer* timer, void *userData)
{
    printf("[%012ld] Timer:%p callback-> %s.\r\n", PlatformTicksGetFunc(), timer, (char*)userData);
    MultiTimerStart(timer, 4567, exampleTimer3Callback, userData);
}

typedef struct CustomUserData {
    int count;
    char* str;
} CustomUserData;
void exampleTimer4Callback(MultiTimer* timer, void *userData)
{
    CustomUserData* customUserData = (CustomUserData*)userData;
    customUserData->count--;
    printf("[%012ld] Timer:%p callback-> %s.\r\n", PlatformTicksGetFunc(), timer, customUserData->str);
    if (customUserData->count > 0) {
        MultiTimerStart(timer, 2000, exampleTimer4Callback, customUserData);
    }
}

int main(int argc, char *argv[])
{
    MultiTimerInstall(PlatformTicksGetFunc);

    MultiTimerStart(&timer1, 1000, exampleTimer1Callback, "1000ms CYCLE timer");
    MultiTimerStart(&timer2, 5000, exampleTimer2Callback, "5000ms ONCE timer");
    MultiTimerStart(&timer3, 3456, exampleTimer3Callback, "3456ms delay start, 4567ms CYCLE timer");
    CustomUserData customUserData = {
        .count = 3,
        .str = "2000ms 3 timer"
    };
    MultiTimerStart(&timer4, 2000, exampleTimer4Callback, &customUserData);

    while (1) {
        MultiTimerYield();
    }
}