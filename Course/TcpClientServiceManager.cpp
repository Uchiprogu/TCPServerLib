#include <cstdlib>
#include <list>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <vector>

#include "TcpClient.h"
#include "TcpClientServiceManager.h"
#include "TcpServerController.h"

#define TCP_CLIENT_REVC_BUFFER_SIZE 1024
unsigned char client_recv_buff[TCP_CLIENT_REVC_BUFFER_SIZE];

TcpClientServiceManager::TcpClientServiceManager(
    TcpServerController *tcp_ctrl) {
  this->tcp_ctrl = tcp_ctrl;

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

  while (true) {
    // printf("------------ start poll \n");
    int num_ready =
        poll(this->pollfields_db.data(), this->pollfields_db.size(), 1000);
    if (num_ready < 0) {
      printf("ERROR: not ready field descriptors for poll\n");
    }
    // printf("------------ end poll \n");

    for (int i = 0; i < this->pollfields_db.size(); i++) {
      printf("i: %d fd: %d events: %d revents: %d \n", i,
             this->pollfields_db[i].fd, this->pollfields_db[i].events,
             this->pollfields_db[i].revents);
      if (this->pollfields_db[i].revents & POLLIN) {
        printf("INFO: handle input event on this file descriptor\n");
      }
      if (this->pollfields_db[i].revents & POLLOUT) {
        printf("INFO: handle output event on this file descriptor\n");
      }
      if (this->pollfields_db[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        printf("INFO: handle errors event on this file descriptor\n");
      }
    }
    // for (it = this->tcp_clietn_db.begin(), tcp_client = *it;
    //      it != this->tcp_clietn_db.end(); tcp_client = next_tcp_client) {

    //   next_tcp_client = *(++it);

    //   if (FD_ISSET(tcp_client->comm_fd, &this->active_fd_set)) {
    //     // read data from FD
    //     rev_bytes = recvfrom(
    //         tcp_client->comm_fd, client_recv_buff,
    //         TCP_CLIENT_REVC_BUFFER_SIZE, 0, reinterpret_cast<sockaddr
    //         *>(&client_addr), &addr_len);

    //     if (this->tcp_ctrl->client_msg_recived) {
    //       this->tcp_ctrl->client_msg_recived(this->tcp_ctrl, tcp_client,
    //                                          client_recv_buff, rev_bytes);
    //     }
    //   }
    // }
  }
}

void TcpClientServiceManager::ClientFDStartListen(TcpClient *tcp_client) {
  this->StopTcpClientServiceManagerThread();
  printf("Client Svc Mgr thread is cancelled\n");

  this->AddClientToDB(tcp_client);

  this->client_svc_mng_thread = (pthread_t *)calloc(1, sizeof(pthread_t));
  this->StartTcpClientServiceManagerThread();
}

void TcpClientServiceManager::SetPollEventForFd(int fd, short event) {
  for (auto &it : this->pollfields_db) {
    if (it.fd = fd) {
      it.events = event;
      break;
    }
  }
}

TcpClient *TcpClientServiceManager::LookUpClientDB(uint32_t ip_addr,
                                                   uint16_t port_no) {
  TcpClient *tcp_client = nullptr;
  for (auto it = this->tcp_clietn_db.begin(); it != this->tcp_clietn_db.end();
       ++it) {
    for (auto &list_it : it->second) {
      if (list_it->ip_addr == ip_addr && list_it->port_no == port_no) {
        tcp_client = list_it;
        break;
      }
    }
  }

  return tcp_client;
}

void TcpClientServiceManager::AddClientToDB(TcpClient *tcp_client) {
  if (tcp_client == nullptr)
    return;
  if (this->tcp_clietn_db.find(tcp_client->comm_fd) ==
      this->tcp_clietn_db.end()) {
    pollfd newfd = {tcp_client->comm_fd, POLLIN, 0};
    this->pollfields_db.push_back(newfd);
  }
  auto &it_list = this->tcp_clietn_db[tcp_client->comm_fd];
  it_list.push_back(tcp_client);
}