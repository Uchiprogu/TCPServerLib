

#include "TcpNewConnectionAccepter.h"
#include "TcpServerController.h"
#include "network_utils.h"

TcpNewConnectionAccepter::TcpNewConnectionAccepter(
    TcpServerController *tcp_ctrl) {

  this->accept_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  // setsockopt();
  if (accept_fd < 0) {
    printf("Error: Could not create Acceoted FD\n");
    std::exit(0);
  }
  this->tcp_ctrl = tcp_ctrl;

  this->accept_new_conn_thread = (pthread_t *)calloc(1, sizeof(pthread_t));
}

TcpNewConnectionAccepter::~TcpNewConnectionAccepter() {}

/*
  1) start a thread
  2) create an infinity loop
  3) invoke accept() to new connections
  4) Notiffy the app for new connections
*/

static void *tcp_listen_for_new_connections(void *arg) {
  TcpNewConnectionAccepter *tcp_new_conn_ac =
      reinterpret_cast<TcpNewConnectionAccepter *>(arg);
  tcp_new_conn_ac->StartTcpNewConnectionAccepterThreadInternal();
}

void TcpNewConnectionAccepter::StartTcpNewConnectionAccepterThreadInternal() {

  int opt;
  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(this->tcp_ctrl->port_no);
  server_addr.sin_addr.s_addr = htonl(this->tcp_ctrl->ip_addr);

  if (setsockopt(this->accept_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    printf("%s() set sockport Faild SO_REUSEADDR . error = %d\n", __func__,
           errno);
    exit(0);
  }

  if (setsockopt(this->accept_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) <
      0) {
    printf("%s() set sockport Faild SO_REUSEPORT . error = %d\n", __func__,
           errno);
    exit(0);
  }

  if (bind(this->accept_fd, (struct sockaddr *)&server_addr,
           sizeof(struct sockaddr)) == -1) {
    printf("%s() Acceptor socked bind faild [%s(0x%x),%d]. Error = %d \n",
           __func__,
           network_convert_ip_n_to_p(this->tcp_ctrl->ip_addr, nullptr),
           this->tcp_ctrl->ip_addr, this->tcp_ctrl->port_no, errno);
    exit(0);
  }

  if (listen(this->accept_fd, 5) < 0) {
    printf("%s() listen faild. error = %d\n", __func__, errno);
    exit(0);
  }

  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int comm_socket_fd;

  while (true) {
    comm_socket_fd =
        accept(this->accept_fd, (struct sockaddr *)&client_addr, &addr_len);

    if (comm_socket_fd < 0) {
      printf("%s() Error in accepting new connection = %d\n", __func__, errno);
      continue;
    }
    printf("Connection Accepted from Client [%s, %d]\n",
           network_convert_ip_n_to_p(client_addr.sin_addr.s_addr, nullptr),
           client_addr.sin_port);
  }
}

void TcpNewConnectionAccepter::StartTcpNewConnectionAccepterThread() {
  if (pthread_create(this->accept_new_conn_thread, NULL,
                     tcp_listen_for_new_connections, (void *)this)) {
    printf("%s() Thread Creation faild. error = %d\n", __func__, errno);
    exit(0);
  }
  printf("Service started: TcpNewConnectionAccepter\n");
}