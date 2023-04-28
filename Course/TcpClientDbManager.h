#pragma once
#include <list>

class TepClient;
class TcpServerController;

class TcpClientDbManager {
private:
  std::list<TepClient *> tcp_clietn_db;

public:
  TcpServerController *tcp_ctrl;
  TcpClientDbManager(TcpServerController *tcp_ctrl);
  ~TcpClientDbManager();
  void StartTcpClientDbManagerInit();
};
