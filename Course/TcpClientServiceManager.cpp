#include <cstdlib>
#include <list>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "TcpClient.h"
#include "TcpClientServiceManager.h"
#include "TcpServerController.h"

#define TCP_CLIENT_REVC_BUFFER_SIZE 1024
unsigned char client_recv_buff[TCP_CLIENT_REVC_BUFFER_SIZE];

TcpClientServiceManager::TcpClientServiceManager(
    TcpServerController *tcp_ctrl) {
  this->tcp_ctrl = tcp_ctrl;
  this->max_fd = 0;
  FD_ZERO(&this->active_fd_set);
  FD_ZERO(&this->backup_fd_set);
  client_svc_mng_thread = (pthread_t *)std::calloc(1, sizeof(pthread_t));
}

TcpClientServiceManager::~TcpClientServiceManager() {}

void *tcp_client_scv_manager_thread_fd(void *arg) {
  TcpClientServiceManager *svc_mngr =
      reinterpret_cast<TcpClientServiceManager *>(arg);

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
  svc_mngr->StartTcpClientServiceManagerThreadInternal();
  return nullptr;
}

void TcpClientServiceManager::StartTcpClientServiceManagerThread() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_create(this->client_svc_mng_thread, &attr,
                 tcp_client_scv_manager_thread_fd, (void *)this);
}

void TcpClientServiceManager::StopTcpClientServiceManagerThread() {
  pthread_cancel(*this->client_svc_mng_thread);
  pthread_join(*this->client_svc_mng_thread, nullptr);
  free(this->client_svc_mng_thread);
  this->client_svc_mng_thread = nullptr;
}

void TcpClientServiceManager::StartTcpClientServiceManagerThreadInternal() {
  /* Invoke select sys calls on all Clients present in Client DB*/

  int rev_bytes;
  TcpClient *tcp_client, *next_tcp_client;
  std::list<TcpClient *>::iterator it;
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  // GetMaxFd();
  printf("INFO: max_fd = %d\n", this->max_fd);
  FD_ZERO(&this->backup_fd_set);
  this->CopyClientFDtoFDSet(&this->backup_fd_set);

  while (true) {
    memcpy(&this->active_fd_set, &this->backup_fd_set, sizeof(fd_set));
    select(this->max_fd + 1, &this->active_fd_set, 0, 0, 0);

    for (it = this->tcp_clietn_db.begin(), tcp_client = *it;
         it != this->tcp_clietn_db.end(); tcp_client = next_tcp_client) {

      next_tcp_client = *(++it);

      if (FD_ISSET(tcp_client->comm_fd, &this->active_fd_set)) {
        // read data from FD
        rev_bytes = recvfrom(
            tcp_client->comm_fd, client_recv_buff, TCP_CLIENT_REVC_BUFFER_SIZE,
            0, reinterpret_cast<sockaddr *>(&client_addr), &addr_len);

        if (this->tcp_ctrl->client_msg_recived) {
          this->tcp_ctrl->client_msg_recived(this->tcp_ctrl, tcp_client,
                                             client_recv_buff, rev_bytes);
        }
      }
    }
  }
}

void TcpClientServiceManager::ClientFDStartListen(TcpClient *tcp_client) {
  this->StopTcpClientServiceManagerThread();
  printf("Client Svc Mgr thread is cancelled\n");

  this->AddClientToDB(tcp_client);

  this->client_svc_mng_thread = (pthread_t *)calloc(1, sizeof(pthread_t));
  this->StartTcpClientServiceManagerThread();
}

int TcpClientServiceManager::GetMaxFd() {
  printf("INFO: GetMaxFd(): %d\n", this->max_fd);
  for (auto it : this->tcp_clietn_db) {
    if (it->comm_fd > this->max_fd)
      this->max_fd = it->comm_fd;
  }
  return this->max_fd;
}

void TcpClientServiceManager::CopyClientFDtoFDSet(fd_set *fdset) {
  if (fdset == nullptr)
    return;
  for (auto &it : this->tcp_clietn_db) {
    FD_SET(it->comm_fd, fdset);
  }
}

TcpClient *TcpClientServiceManager::LookUpClientDB(uint32_t ip_addr,
                                                   uint16_t port_no) {
  TcpClient *tcp_client = nullptr;
  for (auto &it : this->tcp_clietn_db) {
    if (it->ip_addr == ip_addr && it->port_no == port_no) {
      tcp_client = it;
      break;
    }
  }

  return tcp_client;
}

void TcpClientServiceManager::AddClientToDB(TcpClient *tcp_client) {
  if (tcp_client == nullptr)
    return;
  if (this->max_fd < tcp_client->comm_fd) {
    this->max_fd = tcp_client->comm_fd;
  }
  this->tcp_clietn_db.push_front(tcp_client);
}