#include "music.h"
#include <cstdlib>
#include <signal.h>
void playSound(std::string file)
{
	std::string cmd = "exec ./play.sh ";
	cmd += file;
	signal(SIGCHLD,SIG_IGN);
	int pid = fork();
	if(!pid)
	{
		system(cmd.c_str());
		exit(0);
	}
	return;
}