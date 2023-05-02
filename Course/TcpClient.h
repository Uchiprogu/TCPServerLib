#pragma once

#include <stdint.h>

class TcpServerController;

class TcpClient {
private:
public:
  uint32_t ip_addr;
  uint16_t port_no;
  int comm_fd;
  TcpServerController *tcp_ctrl;
  TcpClient(uint32_t ip_addr, uint16_t port_no);
  ~TcpClient();

  void Display();
};