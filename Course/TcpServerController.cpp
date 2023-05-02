
#include "TcpServerController.h"
#include "TcpClient.h"
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

void TcpServerController::ProcessNewClient(TcpClient *tcp_client) {
  this->tcp_client_db_mng->AddClient(tcp_client);
  this->tcp_client_ser_mng->ClientFDStartListen(tcp_client);
}

void TcpServerController::SetServerNotifCallbaks(
    void (*client_connected)(const TcpServerController *, const TcpClient *),
    void (*client_disconnected)(const TcpServerController *, const TcpClient *),
    void (*client_msg_recived)(const TcpServerController *, const TcpClient *,
                               unsigned char *, uint16_t)) {
  this->client_connected = client_connected;
  this->client_disconnected = client_disconnected;
  this->client_msg_recived = client_msg_recived;
}

void TcpServerController::Display() {
  printf("Server name: %s\n", this->name.c_str());
  printf("Listenning on: [%s, %d]\n",
         network_convert_ip_n_to_p(ntohl(this->ip_addr), nullptr),
         this->port_no);

  this->tcp_client_db_mng->DisplayClientDB();
}