#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

class TcpServerController;

/*
  New connections are accepted by 'accept()' sys call
  Wee need to a create 'accept_fd' using socket()

*/

class TcpNewConnectionAccepter {
private:
  int accept_fd;
  pthread_t *accept_new_conn_thread;

public:
  TcpServerController *tcp_ctrl;
  TcpNewConnectionAccepter(TcpServerController *tcp_ctrl);
  ~TcpNewConnectionAccepter();
  void StartTcpNewConnectionAccepterThread();
  void StartTcpNewConnectionAccepterThreadInternal();
};
