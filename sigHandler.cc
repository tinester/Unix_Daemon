/*
Zach Eagan
zse150030@utdallas.edu
CS 3376.002
 */
#include <stdio.h>
#include <rude/config.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "prog5.h"
using namespace std;

// This program catches all the SIGINT, SIGTERM, AND SIGHUP
void sigHandler(int signo)
{
  ofstream myfile;
  myfile.open(daemonMap[LOGFILE].c_str());
  
  if(signo == SIGINT || signo == SIGTERM)
    {
      myfile << "Program shutting down" << endl;
      system("rm -f cs3376dirmond.pid");

      // To test if SIGHUP signal works
      myfile << daemonMap[VERBOSE] << " is the new verbose value" << endl;
     
     // Set global killed variable to true to 
     // indicate that this program should be killed
     killed = true;     
    }
  else if(signo == SIGHUP)
    {
      myfile << "updating conf info" << endl;
      rude::Config config;

      config.load(daemonMap[CONFIG].c_str());

      config.setSection("Parameters");
      daemonMap[VERBOSE] = config.getBoolValue("Verbose") ? "true" : "false";
      daemonMap[LOGFILE] = config.getStringValue("LogFile");
      daemonMap[NUMVERSIONS] = config.getStringValue("NumVersions");
      myfile << "new conf values are " << daemonMap[VERBOSE] << " " << daemonMap[LOGFILE] << " " << daemonMap[NUMVERSIONS] << endl;
    }
}
