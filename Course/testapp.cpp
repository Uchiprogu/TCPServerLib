#include <iostream>
#include <stdint.h>
#include <string>

#include "TcpClient.h"
#include "TcpServerController.h"
#include "network_utils.h"

using namespace std;

void app_client_connected(const TcpServerController *, const TcpClient *);
void app_client_disconnected(const TcpServerController *, const TcpClient *);
void app_client_msg_recived(const TcpServerController *, const TcpClient *,
                            unsigned char *, uint16_t);

static void printClient(const TcpClient *tcp_client) {
  printf("[%s, %d]\n",
         network_convert_ip_n_to_p((tcp_client->ip_addr), nullptr),
         tcp_client->port_no);
}

static void printServer(const TcpServerController *tcp_server) {
  printf("[%s, %d]\n",
         network_convert_ip_n_to_p(ntohl(tcp_server->ip_addr), nullptr),
         tcp_server->port_no);
}

int main(int argc, char **argv) {
  TcpServerController *tcp_server =
      new TcpServerController("127.0.0.1", 40000, "Default TCP Server");
  tcp_server->SetServerNotifCallbaks(
      app_client_connected, app_client_disconnected, app_client_msg_recived);
  tcp_server->start();
  scanf("\n");
  tcp_server->Display();

  return 0;
}

void app_client_connected(const TcpServerController *tcp_serv,
                          const TcpClient *tcp_client) {
  printf("app_client_connected: To Server: ");
  printServer(tcp_serv);
  printf("app_client_connected: Client connected: ");
  printClient(tcp_client);
}
void app_client_disconnected(const TcpServerController *tcp_serv,
                             const TcpClient *tcp_client) {}
void app_client_msg_recived(const TcpServerController *tcp_serv,
                            const TcpClient *tcp_client, unsigned char *buff,
                            uint16_t msg_size) {}