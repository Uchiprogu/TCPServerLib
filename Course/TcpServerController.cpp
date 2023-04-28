#include "TcpServerController.h"

TcpServerController::TcpServerController(uint32_t ip, uint16_t port,
                                         std::string name) {
  this->ip_addr = ip;
  this->name = name;
  this->port_no = port;
}

TcpServerController::~TcpServerController() {}
void TcpServerController::start() {}

void TcpServerController::stop() {}
