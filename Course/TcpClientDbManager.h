#pragma once
#include <list>

class TcpClient;
class TcpServerController;

class TcpClientDbManager {
private:
  std::list<TcpClient *> tcp_clietn_db;

public:
  TcpServerController *tcp_ctrl;
  TcpClientDbManager(TcpServerController *tcp_ctrl);
  ~TcpClientDbManager();
  void StartTcpClientDbManagerInit();
  void AddClient(TcpClient *client);
};
