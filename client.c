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

#define TAR_IP "192.168.43.249"
#define MAX_CONNECTION 20000
#define tar_port 41000


int max_port_num = 20;
char * source_ip[50] = {
    "192.168.43.150",
    "192.168.43.151",
    "192.168.43.152",
    "192.168.43.153",
    "192.168.43.154",
    "192.168.43.155",
    "192.168.43.156",
    "192.168.43.157",
    "192.168.43.158",
    "192.168.43.159",
    "192.168.43.160",
    "192.168.43.161",
    "192.168.43.162",
    "192.168.43.163",
    "192.168.43.164",
    "192.168.43.165",
    "192.168.43.166",
    "192.168.43.167",
    "192.168.43.168",
    "192.168.43.169",
    "192.168.43.170",
    "192.168.43.171",
    "192.168.43.172",
    "192.168.43.173",
    "192.168.43.174",
    
    "192.168.43.175",
    "192.168.43.176",
    "192.168.43.177",
    "192.168.43.178",
    "192.168.43.179",
    "192.168.43.180",
    "192.168.43.181",
    "192.168.43.182",
    "192.168.43.183",
    "192.168.43.184",
    "192.168.43.185",
    "192.168.43.186",
    "192.168.43.187",
    "192.168.43.188",
    "192.168.43.189",
    "192.168.43.190",
    "192.168.43.191",
    "192.168.43.192",
    "192.168.43.193",
    "192.168.43.194",
    "192.168.43.195",
    "192.168.43.196",
    "192.168.43.197",
    "192.168.43.198",
    "192.168.43.199"
                      };

int main()
{
  sleep(10);
  int fdlists[MAX_CONNECTION];
  for(int ip_index=0;ip_index<50;ip_index++)
  {

    printf("source ip %s\n", source_ip[ip_index]);
    for (int index = 0; index < MAX_CONNECTION; index++)
    {
      printf("send connection %s\n",source_ip[ip_index]);
      int son_start_code =socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
      assert(son_start_code >= 0);

      struct sockaddr_in cliAddr;
      cliAddr.sin_family = AF_INET;
      cliAddr.sin_addr.s_addr = inet_addr(source_ip[ip_index]);
      cliAddr.sin_port = 0;
      if(bind(son_start_code,  (struct sockaddr *)&cliAddr, sizeof(cliAddr)) < 0)
      {
          printf("\n Error : Bind Failed \n");
      }

      // 建立连接
      struct sockaddr_in server_vaddr;
      server_vaddr.sin_family = AF_INET;
      server_vaddr.sin_addr.s_addr = inet_addr(TAR_IP);
      server_vaddr.sin_port = htons(tar_port);

      int connect_state = connect(son_start_code, (void *)&server_vaddr, sizeof(server_vaddr));
      fdlists[index]=son_start_code;
    }
  }
  sleep(120);
  for(int i=0;i<MAX_CONNECTION;i++)
  {
    close(fdlists[i]);
  }
  return 0;
}
