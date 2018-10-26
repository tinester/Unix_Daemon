/*
Zach Eagan
zse150030@utdallas.edu
CS 3376.002
 */
#include <iostream>
#include <fstream>
#include <rude/config.h>
#include <tclap/CmdLine.h>
#include <map>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "prog5.h"
using namespace std;


//Function for checking if a file exists
int file_exist(const char *filename)
{
  struct stat buffer;
  return (stat(filename, &buffer) == 0);
}
// initialize killed
 bool killed = false;

// Define daemonMap
map<int, string> daemonMap;



int main(int argc, char *argv[])
{

  // Create forkValue for later forking
  pid_t forkValue;

 
  // Create config object
  //
  rude::Config config;
  
  try
    {
      TCLAP::CmdLine cmd("cs3376dirmond Directory Monitor Daemon", ' ', "5.38.75");
      
      
      TCLAP::SwitchArg dSwitch("d", "daemon", "Run in daemon mode (forks to run as a daemon", cmd, false);
      
      
      TCLAP::UnlabeledValueArg<std::string> configArg("config", "The name of the configuration file", false, "cs3376dirmond.conf", "config filename", true);

      
      // Add arguments to cmd object
      cmd.add(configArg);

            
      // Parse cmd
      cmd.parse(argc, argv);
      
      daemonMap[DAEMON] = dSwitch.getValue() ? "true" : "false";
      daemonMap[CONFIG] = configArg.getValue();
           
      // Load the config file into Rude Config 
      // And place the conf file's values in map
      config.load(daemonMap[CONFIG].c_str());
      
      // set section
      config.setSection("Parameters");
      daemonMap[VERBOSE] = config.getBoolValue("Verbose") ? "true" : "false";
      daemonMap[LOGFILE] = config.getStringValue("LogFile");
      daemonMap[PASSWORD] = config.getStringValue("Password");
      daemonMap[NUMVERSIONS] = config.getStringValue("NumVersions");
      daemonMap[WATCHDIR] = config.getStringValue("WatchDir");
      


      //For testing conf map
      //cout << daemonMap[VERBOSE] << " " << daemonMap[LOGFILE] << " " << daemonMap[PASSWORD] << " " << daemonMap[NUMVERSIONS]  << " " << daemonMap[WATCHDIR] << " " <<  "something else" << endl;
    }
  catch(TCLAP::ArgException &e)
    {cerr << "error: " << e.error() << " for arg " << e.argId() << endl;}

  // Create .versions if doesn't exist
  string pathToVerDir = daemonMap[WATCHDIR] + "/.versions";
  string verDirCmd = "mkdir " + pathToVerDir;
  if(!file_exist(pathToVerDir.c_str()))
    {
      system(verDirCmd.c_str());
    }
  else
    {
      cout << ".versions file already exists" << endl;
    }

  // Check if file if 
  // Run as daemon
  if(daemonMap[DAEMON] == "true")
    {
      // For testing the pidvalue
      /*forkValue = fork();

      if(forkValue == -1)
	{
	  cerr << "Error forking" << endl;
	  return 1;
	}
      // If this process is the chile
      else if(forkValue == 0)
	{
	  cout << "this is the chile. My pid be " << getpid() << endl;
	  return 0;
	}
      // nonnegative/nonzero forkValue mean forkValue is a PID
      // So we're the parent
      else
      {
	cout << "This is the parent. My pid be " << getpid() << endl;

	  // Create the PID file if it doesn't exist
	  if(file_exist("cs3376dirmond.pid"))
	    {
	      cout << "pid is there so don't create daemon" << endl;
	    }
	  else
	    {
	      cout << "no pid so create that daemon" << endl;
	    }


	  return 0;
	  }
      */
      
      // Create the PID file if it doesn't exist
      if(file_exist("cs3376dirmond.pid")) 
	{
	  cout << "A pid file already exists, so we exit" << endl;
	}
      else
	{
	  // Fork and use the child to run in the background
	  forkValue = fork();
	  if(forkValue == -1)
	    {
	      cout << "There was an error in forking. Terminating..." << endl;
	      return 1;
	    }
	  // If child, continue running
	  else if(forkValue == 0)
	    {
	      runAsDaemon(daemonMap);

	    }
	  // If parent, exit
	  else
	    {
	      return 0;
	    }

	}
          
    }
  // Run program as not a daemon
  else
    {
      runNotDaemon(daemonMap);
    }

  return 0;
}
