#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>
#include <assert.h>
#include <sys/select.h>
#include <errno.h>
#include <pthread.h>
#define MAX_CON 1000000
pthread_mutex_t *count_mutex;
int max_connection = 0;

int fds[MAX_CON];

int main()
{
  count_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(count_mutex, NULL);

  int bind_port=41000;

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sockfd >= 0);

  // BIND到指定的端口
  int addr_lenth = 0;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(bind_port);

  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

  int bind_val = bind(sockfd, (const struct sockaddr *)(&addr), sizeof(addr));
  assert(bind_val >= 0);

  // 对CONNECTION_PORT端口进行监听
  int liston_val = listen(sockfd, 1000000);
  assert(liston_val >= 0);

  
  printf("wait on port %d\n", bind_port);
  int i=0;
  while(1)
  {
    int new_conFd = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&addr_lenth);
    if (new_conFd <= 0)
    {
      printf("accept error");
    }
    else if (new_conFd > 0)
    {
     
      pthread_mutex_lock(count_mutex);
      fds[i]=new_conFd;
      i++;
      max_connection++;
      if(max_connection%100000==0)
         printf("%d accept success! cur connection %d\n", bind_port, max_connection);
      pthread_mutex_unlock(count_mutex);
    }
    if(max_connection==1000000)
      break;
  }
  printf("connection over!\n");
  sleep(120);
  for(int j=0;j<MAX_CON;j++)
    {
      close(fds[j]);
    }
  return 0;
}
