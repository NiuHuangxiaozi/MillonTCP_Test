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
#define TAR_IP "192.168.3.72"
#define MAX_CONNECTION 48000

int max_connect = 0;
pthread_mutex_t *count_mutex;

void *tcpconnect(void *arg)
{

  int tar_port = (*(int *)arg);
  printf("tar port %d\n", tar_port);
  for (int index = 0; index < MAX_CONNECTION; index++)
  {
    int son_start_code = socket(AF_INET, SOCK_STREAM, 0);
    assert(son_start_code >= 0);

    // 建立连接
    struct sockaddr_in less_node_vaddr;
    less_node_vaddr.sin_family = AF_INET;
    less_node_vaddr.sin_addr.s_addr = inet_addr(TAR_IP);
    less_node_vaddr.sin_port = htons(tar_port);

    int connect_state = connect(son_start_code, (void *)&less_node_vaddr, sizeof(less_node_vaddr));
    if (connect_state < 0)
    {
      printf("%s\n", strerror(errno));
    }
    assert(connect_state >= 0);
    if (connect_state < 0)
    {
      pthread_exit(NULL);
      break;
    }
    else
    {
      pthread_mutex_lock(count_mutex);
      max_connect++;
      printf("PORT : %d   |  cur max connection : %d \n", tar_port, max_connect);
      pthread_mutex_unlock(count_mutex);
    }
  }
  pthread_exit(NULL);
}

int max_thread_num = 21;
int target_ports[21] = {40000, 40001, 40002, 40003, 40004, 40005, 40006, 40007, 40008, 40009,
                        40010, 40011, 40012, 40013, 40014, 40015, 40016, 40017, 40018, 40019,
                        40020};
int main()
{

  count_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(count_mutex, NULL);

  sleep(10);

  for (int pth = 0; pth < max_thread_num; pth++)
  {
    int *idx = (int *)malloc(sizeof(int));
    *idx = target_ports[pth];
    pthread_t nbr_listen_thread;
    pthread_create(&nbr_listen_thread, NULL, tcpconnect, (void *)idx);
    pthread_detach(nbr_listen_thread);
  }

  while (1)
  {
    sleep(10);
  }
  return 0;
}
