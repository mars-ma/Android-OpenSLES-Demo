#include <android/log.h>

#ifndef ANDROID_OPENSLES_DEMO_LOG_H
#define ANDROID_OPENSLES_DEMO_LOG_H

#endif //ANDROID_OPENSLES_DEMO_LOG_H
#define LOG_OPEN 1
#if(LOG_OPEN==1)
    #define LOG(...) __android_log_print(ANDROID_LOG_DEBUG,"dev_mars",__VA_ARGS__)
#else
    #define LOG(...) NULL
#endif
