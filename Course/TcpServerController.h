#pragma once

#include "TcpClientDbManager.h"
#include "TcpClientServiceManager.h"
#include "TcpNewConnectionAccepter.h"
#include <stdint.h>
#include <string>

class TcpServerController {
private:
  TcpNewConnectionAccepter *tcp_new_con_acc;
  TcpClientServiceManager *tcp_client_ser_mng;
  TcpClientDbManager *tcp_client_db_mng;

public:
  uint32_t ip_addr;
  uint16_t port_no;
  std::string name;

  TcpServerController(uint32_t ip, uint16_t port, std::string name);
  ~TcpServerController();
  void start();
  void stop();
};