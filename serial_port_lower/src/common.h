#ifndef PLATFORM_H
#define PLATFORM_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cjson/cJSON.h>


#define PROTOCOLS_1 1
#define PROTOCOLS_2 2

#ifdef MTK
    #define PLATFORM    "MTK"
    #define VERSION     "1.0.0"
    #define PROTOCOLS    PROTOCOLS_1
#endif

#ifdef AML
    #define PLATFORM    "AML"
    #define VERSION     "2.1.0"
    #define PROTOCOLS    PROTOCOLS_2
#endif



#endif // PLATFORM_H


