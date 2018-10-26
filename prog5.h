/*
Zach Eagan
zse150030@utdallas.edu
CS 3376.002
*/

#ifndef PROG5_H
#define PROG5_H

#include <map>
#include <string>
#include <unistd.h>
using namespace std;

int runNotDaemon(map<int, string> something);
int runAsDaemon(map<int, string> something);

enum myArgs{DAEMON, CONFIG, VERBOSE, LOGFILE, PASSWORD, NUMVERSIONS, WATCHDIR};

// Initialize all the signal handlers
void sigHandler(int signo);

// Check if a file exists
int file_exist(const char *filename);

// global boolean variable that tells you whether or not a SIGINT or SIGTERM
// signal has been sent yet
extern bool killed;

// Declare Map var
extern map<int, string> daemonMap;

#endif
