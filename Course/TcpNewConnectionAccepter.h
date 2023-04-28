#pragma once

class TcpServerController;

class TcpNewConnectionAccepter {
private:
  TcpServerController *tcp_ctrl;

public:
  TcpNewConnectionAccepter(TcpServerController *tcp_ctrl);
  ~TcpNewConnectionAccepter();
};
