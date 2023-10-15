#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "multmodulo.h"

struct Server {
  char ip[255];
  int port;
};

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        if (k <= 0) {
              printf("k is a positive number\n");
              return 1;
            }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        if (mod <= 0) {
          printf("mod is a positive number\n");
          return 1;
        }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  unsigned int servers_num = 2;
  uint64_t answerall = 1;
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  // TODO: delete this and parallel work between servers
  to[0].port = 20001;
  memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));
  to[1].port = 20002;
  memcpy(to[1].ip, "127.0.0.1", sizeof("127.0.0.1"));

  // TODO: work continiously, rewrite to make parallel

  int active_child_processes = 0;
  

  for (int i = 0; i < servers_num; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      active_child_processes += 1;
      if (child_pid == 0) {
        struct hostent *hostname = gethostbyname(to[i].ip);
        if (hostname == NULL) {
          fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
          exit(1);
        }

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(to[i].port);
        server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

        int sck = socket(AF_INET, SOCK_STREAM, 0);
        if (sck < 0) {
          fprintf(stderr, "Socket creation failed!\n");
          exit(1);
        }

        if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
          fprintf(stderr, "Connection failed\n");
          exit(1);
        }

        // TODO: for one server
        // parallel between servers
        uint64_t begin = i*k/servers_num + 1;
        uint64_t end = (i+1)*k/servers_num + 1;

        char task[sizeof(uint64_t) * 3];
        memcpy(task, &begin, sizeof(uint64_t));
        memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
        memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

        if (send(sck, task, sizeof(task), 0) < 0) {
          fprintf(stderr, "Send failed\n");
          exit(1);
        }

        char response[sizeof(uint64_t)];
        if (recv(sck, response, sizeof(response), 0) < 0) {
          fprintf(stderr, "Recieve failed\n");
          exit(1);
        }

        // TODO: from one server
        // unite results
        uint64_t answer = 0;
        memcpy(&answer, response, sizeof(uint64_t));
        answerall*=answer;
        answerall=answerall % mod;
        close(sck);
        return 0;
      }
    }
    else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  while (active_child_processes > 0) {
    // your code here
    wait(NULL);
    active_child_processes--;
  }
  printf("answerall: %lu\n", answerall);
  free(to);
  return 0;
}
