#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

#define SERVICE_IP "192.168.200.130"

int send_file(char *file_path);
int recv_result();
int close_file_socket();
int send_wechat(char *data , unsigned int len);
int recv_wechat(char *msg_buff , unsigned int len);
int close_wechat_socket();


#endif