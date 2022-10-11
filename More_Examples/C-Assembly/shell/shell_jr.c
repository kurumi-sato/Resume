#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>

#define MAX_LEN 1024


int main() {
  char *argv[3];
  char line[MAX_LEN + 1];
  char command[MAX_LEN + 1] = "", 
    dir[MAX_LEN + 1] = "", extra[MAX_LEN + 1] = "";
  pid_t pid = 0;
  int status;

   
  printf("shell_jr: ");
  fflush(stdout);
    
  while(fgets(line, MAX_LEN + 1, stdin) != NULL ) {
    sscanf(line, " %s %s %s", command, dir, extra);
    argv[0] = command;
    argv[1] = dir;
    argv[2] = extra;
    
      
    /* Null at the end */
    if(argv[1] != NULL) {
      argv[2] = NULL;
    } else {
      argv[1] = NULL;
    }
   
    if(strcmp(command, "exit") == 0) {
      printf("See you\n");
      fflush(stdout);
      exit(EXIT_SUCCESS);
      
    } else if(strcmp(command, "hastalavista") == 0) {
      printf("See you\n");
      fflush(stdout);
      exit(EXIT_SUCCESS);
      
    } else if(strcmp(command, "cd") == 0) {
      status = chdir(dir);
      if(status == -1) {
	err(EX_OSERR, "Cannot change to directory %s\n", dir);
	fflush(stdout);
      }
      
    } else {
      /* dealing with the rest of the commands by forking */
      if((pid = fork()) < 0) {
	err(EX_OSERR, "fork error");
      }
      if(pid) { /* parent */
	wait(NULL);
	
      } else { /* child */
	execvp(command, argv);
	printf("Failed to execute %s\n", command);
	fflush(stdout);
	exit(EX_OSERR);
      }
    }
    printf("shell_jr: ");
    fflush(stdout);
  }
  fclose(stdin);
  return 0;
}
