#pragma once

class TcpServerController;
class TepClient;

class TcpClientServiceManager {
private:
  /* data */
public:
  TcpServerController *tcp_ctrl;

  TcpClientServiceManager(TcpServerController *tcp_ctrl);
  ~TcpClientServiceManager();
};
