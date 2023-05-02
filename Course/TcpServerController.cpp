
#include "TcpServerController.h"
#include "network_utils.h"

TcpServerController::TcpServerController(std::string ip, uint16_t port,
                                         std::string name) {
  this->ip_addr = htonl(network_convert_ip_p_to_n(ip.c_str()));
  this->name = name;
  this->port_no = port;

  this->tcp_new_con_acc = new TcpNewConnectionAccepter(this);
  this->tcp_client_ser_mng = new TcpClientServiceManager(this);
  this->tcp_client_db_mng = new TcpClientDbManager(this);
}

TcpServerController::~TcpServerController() {
  delete this->tcp_new_con_acc;
  delete this->tcp_client_ser_mng;
  delete this->tcp_client_db_mng;
}
void TcpServerController::start() {
  /*
    Start the CRS thread
    Start the DRS thread
    Initialize DBMRS
  */
  this->tcp_new_con_acc->StartTcpNewConnectionAccepterThread();
  this->tcp_client_ser_mng->StartTcpClientServiceManagerThread();
  this->tcp_client_db_mng->StartTcpClientDbManagerInit();

  printf("Tcp server is Uo and running [%s, %d]\n OK.\n",
         network_convert_ip_n_to_p(this->ip_addr, nullptr), this->port_no);
}

void TcpServerController::stop() {}
