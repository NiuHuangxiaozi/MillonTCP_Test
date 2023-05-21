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

#define LOW_PORT_NUM 5000
#define HIGH_PORT_NUM 65000
#define LOCAL_IP "192.168.3.69"
pthread_mutex_t *count_mutex;
int max_connection = 0;

void *tcpconnected(void *arg)
{

  int bind_port = (*(int *)arg);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
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
  int liston_val = listen(sockfd, 10);
  assert(liston_val >= 0);
  printf("wait on port %d\n", bind_port);
  while (1)
  {
    printf("aaa\n");
    int new_conFd = accept(sockfd, (struct sockaddr *)&addr, (socklen_t *)&addr_lenth);
    if (new_conFd <= 0)
    {
      printf("accept error");
    }
    if (new_conFd > 0)
    {
      pthread_mutex_lock(count_mutex);
      max_connection++;
      printf("%d accept success! cur connection %d\n", bind_port, max_connection);
      pthread_mutex_unlock(count_mutex);
    }
  }
  pthread_exit(NULL);
}

int max_thread_num = 21;
int bind_ports[21] = {40000, 40001, 40002, 40003, 40004, 40005, 40006, 40007, 40008, 40009,
                      40010, 40011, 40012, 40013, 40014, 40015, 40016, 40017, 40018, 40019,
                      40020};

int main()
{
  count_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(count_mutex, NULL);

  for (int pth = 0; pth < max_thread_num; pth++)
  {
    int *idx = (int *)malloc(sizeof(int));
    *idx = bind_ports[pth];
    pthread_t nbr_listen_thread;
    pthread_create(&nbr_listen_thread, NULL, tcpconnected, (void *)idx);
  }

  while (1)
  {
    sleep(10);
  }
  return 0;
}
