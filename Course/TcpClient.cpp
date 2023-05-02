#include <cstdio>

#include "TcpClient.h"
#include "network_utils.h"

TcpClient::TcpClient(uint32_t ip_addr, uint16_t port_no) {
  this->ip_addr = ip_addr;
  this->port_no = port_no;
}

void TcpClient::Display() {
  printf("Tcp Client [%s, %d]\n",
         network_convert_ip_n_to_p(ntohl(this->ip_addr), nullptr),
         this->port_no);
}