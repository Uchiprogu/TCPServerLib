#pragma once

#include <stdint.h>
#include <string>

#include "TcpClientDbManager.h"
#include "TcpClientServiceManager.h"
#include "TcpNewConnectionAccepter.h"

class TcpClient;
class TcpServerController {
private:
  TcpNewConnectionAccepter *tcp_new_con_acc;
  TcpClientServiceManager *tcp_client_ser_mng;
  TcpClientDbManager *tcp_client_db_mng;

public:
  uint32_t ip_addr;
  uint16_t port_no;
  std::string name;

  TcpServerController(std::string ip, uint16_t port, std::string name);
  ~TcpServerController();
  void start();
  void stop();
  void Display();
  void ProcessNewClient(TcpClient *tcp_client);
  void (*client_connected)(const TcpServerController *, const TcpClient *);
  void (*client_disconnected)(const TcpServerController *, const TcpClient *);
  void (*client_msg_recived)(const TcpServerController *, const TcpClient *,
                             unsigned char *, uint16_t);

  void SetServerNotifCallbaks(
      void (*client_connected)(const TcpServerController *, const TcpClient *),
      void (*client_disconnected)(const TcpServerController *,
                                  const TcpClient *),
      void (*client_msg_recived)(const TcpServerController *, const TcpClient *,
                                 unsigned char *, uint16_t));
};