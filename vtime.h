//
// Created by bronyna on 2024/7/27.
//

#ifndef VIOLET_TIMER_H
#define VIOLET_TIMER_H

#include <sys/time.h>

#define  CURRENT_SYSTEM_TIME_IN_MILLIS   ({  \
   struct timeval time; \
   gettimeofday(&time, NULL); \
   long _time = time.tv_sec * 1000L + time.tv_usec / 1000L; \
   _time;  \
})

#define  CURRENT_MONOTONIC_TIME   ({  \
   struct timespec timestamp = { 0, 0 };  \
   clock_gettime(CLOCK_MONOTONIC, &timestamp); \
   long _duration = timestamp.tv_sec * 1000L + timestamp.tv_nsec / 1000000L;  \
   _duration;  \
})

#endif //VIOLET_TIMER_H
