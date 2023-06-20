#ifndef __LOG_H__
#define __LOG_H__

#include "Main.h"
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>

template<class... Args>
void WriteGameLog(Args... args){
    const time_t _t = time(0);
    std::string path = "game.log";
    std::ofstream i(path.c_str(), std::ios_base::app);
    const char* timeString = strtok(ctime(&_t), "\n");
    (i << '[' << timeString << "] " << ... << args) << "\n";
}
#endif // __LOG_H__