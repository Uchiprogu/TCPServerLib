#pragma once

class TcpServerController;
class TcpClient;

class TcpClientServiceManager {
private:
  int max_fd;
  fd_set active_fd_set;
  fd_set backup_fd_set;
  pthread_t *client_svc_mng_thread;
  std::list<TcpClient *> tcp_clietn_db;

  void CopyClientFDtoFDSet(fd_set *fdset);

public:
  TcpServerController *tcp_ctrl;

  TcpClientServiceManager(TcpServerController *tcp_ctrl);
  ~TcpClientServiceManager();
  void StartTcpClientServiceManagerThread();
  void ClientFDStartListen(TcpClient *tcp_client);
  void StartTcpClientServiceManagerThreadInternal();
  int GetMaxFd();
  TcpClient *LookUpClientDB(uint32_t ip_addr, uint16_t port_no);
  void AddClientToDB(TcpClient *tcp_client);
};
