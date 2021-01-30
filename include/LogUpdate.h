//#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
//#include <atomic>
using namespace std;
#define Nor 0
#define War 1
#define Err 2
void initLog();
void logUpdate(string msg, int level);
