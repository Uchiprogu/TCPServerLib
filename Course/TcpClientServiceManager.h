#pragma once

class TcpServerController;
class TcpClient;

class TcpClientServiceManager {
private:
  /* data */
public:
  TcpServerController *tcp_ctrl;

  TcpClientServiceManager(TcpServerController *tcp_ctrl);
  ~TcpClientServiceManager();
  void StartTcpClientServiceManagerThread();
  void ClientFDStartListen(TcpClient *tcp_client);
};
