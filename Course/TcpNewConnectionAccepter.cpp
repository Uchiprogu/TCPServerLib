#include "TcpNewConnectionAccepter.h"

TcpNewConnectionAccepter::TcpNewConnectionAccepter(
    TcpServerController *tcp_ctrl) {
  this->tcp_ctrl = tcp_ctrl;
}

TcpNewConnectionAccepter::~TcpNewConnectionAccepter() {}

void TcpNewConnectionAccepter::StartTcpNewConnectionAccepterThread(){

}