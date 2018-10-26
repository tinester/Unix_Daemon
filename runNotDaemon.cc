/*
Zach Eagan
zse150030@utdallas.edu
CS 3376.002
 */
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <string>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "prog5.h"
using namespace std;

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


int runNotDaemon(map<int, string> confMap)
{

  // Initialize signal handler struct
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = sigHandler;

  
  sigaction(SIGINT, &act, 0);
  sigaction(SIGTERM, &act, 0);
  sigaction(SIGHUP, &act, 0);

  //Initialize inotify stuff
  int length, i = 0;
  int fd;
  int wd;
  char buffer[BUF_LEN];

  // Open log file
  ofstream myfile;
  myfile.open(confMap[LOGFILE].c_str());

  myfile << "Running as not a daemon" << endl;

  // buncha inotify stuff
  fd = inotify_init();
 
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  
  wd = inotify_add_watch( fd, confMap[WATCHDIR].c_str(), IN_MODIFY);

  // killed is a global boolean variable
  while(!killed)
    {
      length = read( fd, buffer, BUF_LEN );

      if ( length < 0 ) {
	perror( "read" );
      } 
  
      while(i < length)
	{
          myfile << "evaluating an event in " << confMap[WATCHDIR] << endl;
	  struct inotify_event *event = (struct inotify_event *) &buffer[i];

	  if(event->len)
	    {
	      if(event->mask & IN_MODIFY)
		{
		  // Logs modification
		  string nameTmp = event->name;
		  myfile << "modified " << nameTmp << endl;

		  // Prepares a date string (for testing just print to file to see if works)
		  
		  FILE *streamDate;
		  char dateBuff[1000];
		  string dateCmd = "date --rfc-3339='seconds'";
		  string modFile = event->name;

		  streamDate = popen(dateCmd.c_str(), "r");
		  // dateBuff has the date as a cstring
		  fgets(dateBuff, 1000, streamDate);
		  string date = dateBuff;
		  replace(date.begin(), date.end(), ' ', '-');
		  replace(date.begin(), date.end(), '\n', '\0');
		  pclose(streamDate);

		  string verFileName = nameTmp + "." + date;
		  myfile << "the modified file name is " << verFileName << endl;
		  string cpFileCmd = "cp " + confMap[WATCHDIR] + '/' + nameTmp + " " + confMap[WATCHDIR] + "/.versions/" + verFileName;
		  // for testing
		  myfile << "cpFileCmd is " << cpFileCmd << endl;
		  
		  system(cpFileCmd.c_str());

		}
	    }
	  i += EVENT_SIZE + event->len;
	}
      // Reset Counter
      i = 0;
    }
  // close inotify and log file
  (void)inotify_rm_watch(fd, wd);
  (void)close(fd);
  myfile.close();
  
  return 0;
} 
