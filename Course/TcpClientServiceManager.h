#include <poll.h>
#include <vector>
#pragma once

class TcpServerController;
class TcpClient;

class TcpClientServiceManager {
private:
  std::vector<pollfd> pollfields_db;

  pthread_t *client_svc_mng_thread;
  std::map<int, std::list<TcpClient *>> tcp_clietn_db;

public:
  TcpServerController *tcp_ctrl;

  TcpClientServiceManager(TcpServerController *tcp_ctrl);
  ~TcpClientServiceManager();
  void StartTcpClientServiceManagerThread();
  void StopTcpClientServiceManagerThread();
  void ClientFDStartListen(TcpClient *tcp_client);
  void StartTcpClientServiceManagerThreadInternal();
  void SetPollEventForFd(int fd, short event);
  TcpClient *LookUpClientDB(uint32_t ip_addr, uint16_t port_no);
  void AddClientToDB(TcpClient *tcp_client);
};
