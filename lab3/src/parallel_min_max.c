#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

void KILLCHILD (pid_t id) {
  kill(id, SIGKILL);
  printf("Hello I am die");
  int status;
  wait(&status);
}

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;
  int time = 0;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {"time", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            // your code here
            // error handling
            if (seed <= 0) {
              printf("seed is a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0) {
              printf("array_size is a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            if (pnum <= 0) {
              printf("pnum is a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;
          case 4:
            time = atoi(optarg);
            // error handling
            if (time <= 0) {
              printf("pnum is a positive number\n");
              return 1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  alarm(time);
  sleep(time/2);

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
  
  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  FILE *fp;
  fp=fopen("test.txt", "w");

  int pipefd[2];
  pipe(pipefd);
  pid_t kill_pid = -1;
  int done_flag = 0;

  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        // parallel somehow
        struct MinMax min_max = GetMinMax(array, (array_size*i)/pnum, (array_size*(i+1))/pnum);
        if (with_files) {
          // use files here	
          fprintf(fp, "%d %d\n", min_max.max,min_max.min);
        } else {
          // use pipe here
            close(pipefd[0]);
            write(pipefd[1], &min_max.max, sizeof(min_max.max));
            write(pipefd[1], &min_max.min, sizeof(min_max.min));
            //close(pipefd[1]);
        }
        
        return 0;
      }
      else {
        kill_pid = child_pid;
        signal(SIGALRM, KILLCHILD);
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  fclose(fp);
  while (active_child_processes > 0) {
    // your code here
    //wait(NULL);
    signal (SIGALRM, KILLCHILD);
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  
  fp=fopen("test.txt", "r");
  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      fscanf(fp,"%d %d",&max,&min);
      printf("Max_read: %d %d\n", max, min);
    } else {
      // read from pipes
      read(pipefd[0], &max, 4);
      read(pipefd[0], &min, 4);
      printf("Max_read: %d Min_read: %d\n", max, min);
      //close(pipefd[0]);

    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }
  fclose(fp);

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
