//
// Created by Vimii on 2021/05/15.
//

#ifndef RAYTRACING_FILENAME_H
#define RAYTRACING_FILENAME_H

#include <time.h>

std::string dayName(){
    char *format = ".png";

    time_t timer;
    struct tm *t_st;

    /* 現在時刻の取得 */
    time(&timer);
    t_st = localtime(&timer);

    std::string File;
    std::string month = std::to_string(t_st->tm_mon+1);
    std::string day = std::to_string(t_st->tm_mday);
    std::string hour = std::to_string(t_st->tm_hour);
    std::string minute = std::to_string(t_st->tm_min);
    std::string slash("_");
    std::string dir("./render/");
    File += dir;
    File += month;
    File += slash;
    File += day;
    File += slash;
    File += hour;
    File += slash;
    File += minute;
    File += format;

    return File;
}

#endif //RAYTRACING_FILENAME_H
