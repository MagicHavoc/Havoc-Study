#include "mySocket.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include "afl-fuzz.h"
#include "myQueue.h"

#define MAXLINE 65536
int TIME_LIMIT = 10;
int MAX_ITERATION = 5;
int PORT = 6002;

int isServer = 1;

int listenfd, connfd, sockfd;
struct sockaddr_in servaddr;
char sendline[MAXLINE], recvline[MAXLINE];
char buf[MAXLINE];
int n;
time_t start_time;
struct queue* myqueue;
int cur_iteration = 0;

void initServer() {
  isServer = 1;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
    printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  if (listen(listenfd, 10) == -1) {
    printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  printf("======waiting for client's request======\n");
  while (1) {
    if ((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
      printf("waiting for clients...");
      continue;
    }
    break;
  }

  myqueue = create_queue();
  time(&start_time);
}

void block_if_server(char** argv) {
  if (isServer) wait_and_run(argv);
  time(&start_time);
}

void finalizeServer() {
  if (isServer) {
    close(connfd);
    close(listenfd);
  } else {
    close(sockfd);
  }
}

void initClient() {
  isServer = 0;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  while (1) {
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
      printf("waiting for response from server...");
      sleep(5);
      continue;
    }
    break;
  }

  while (1) {
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
      printf("waiting for connection from server...");
      sleep(5);
      continue;
    }
    break;
  }

  printf("client init done\n");
  myqueue = create_queue();
  time(&start_time);
}

void wait_and_run(char** argv) {
  printf("wait_and_run\n");
  int fd = isServer ? connfd : sockfd;

  while (1) {
    memset(recvline, 0, sizeof(recvline));

    while (1) {
      memset(buf, 0, sizeof(buf));
      recv(fd, buf, MAXLINE, 0);
      strcat(recvline, buf);
      if (strstr(recvline, " ") != NULL) break;
    }

    int i, flag = 1;
    for (i = 0; i < strlen(recvline); i++) {
      if (recvline[i] == ' ' || recvline[i] == '#') {
        if (recvline[i] == '#') flag = 0;
        recvline[i] = 0;
        memset(buf, 0, sizeof(buf));
        strcpy(buf, recvline);
        break;
      }
    }

    if(strlen(buf) > 0)
      handle_testcase(argv, buf);
    send(fd, "OK", MAXLINE, 0);
    if(!flag) break;
  }
  time(&start_time);
}

void check_and_send(char** argv) {
  time_t current_time;
  time(&current_time);

  if (current_time - start_time >= TIME_LIMIT) {
    printf("delta = %d\n", current_time - start_time);

    struct qentry* p = myqueue->head->next;
    int fd = isServer ? connfd : sockfd;

    if(p == NULL) {
      send(fd, "# ", MAXLINE, 0);
      usleep(50000);
      recv(fd, recvline, MAXLINE, 0);
    }

    while (p != NULL) {
      memset(sendline, 0, sizeof(sendline));
      getcwd(sendline, MAXLINE);
      strcat(sendline, "/");
      strcat(sendline, p->fname);

      if (p->next == NULL) strcat(sendline, "#");

      strcat(sendline, " ");
      send(fd, sendline, MAXLINE, 0);

      p = p->next;
      memset(recvline, 0, sizeof(recvline));
      usleep(50000);
      recv(fd, recvline, MAXLINE, 0);
    }

    myqueue = renew_queue(myqueue);
    cur_iteration++;
    if (cur_iteration >= MAX_ITERATION) exit(0);

    usleep(100000);
    wait_and_run(argv);
  }
}

void insert_queue(unsigned char* fname) {
  struct qentry* qe = (struct qentry*)malloc(sizeof(struct qentry));
  memset(qe, 0, sizeof(struct qentry));

  qe->fname = (char*)malloc(strlen(fname) + 1);
  qe->next  = NULL;
  
  strcpy(qe->fname, fname);
  add_entry_to_queue(myqueue, qe);
}

int get_file_length(unsigned char* fname) {
  FILE* fp;
  fp = fopen(fname, "rb");
  fseek(fp, 0L, SEEK_END);
  int len = ftell(fp);
  close(fp);
  return len;
}

void set_time_limit(unsigned int t) { TIME_LIMIT = t; }

void set_iteration(unsigned int t) { MAX_ITERATION = t; }

void set_port(unsigned int t) { PORT = t; }