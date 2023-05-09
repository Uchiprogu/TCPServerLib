
#include "TcpClientDbManager.h"
#include "TcpClient.h"

TcpClientDbManager::TcpClientDbManager(TcpServerController *tcp_ctrl) {
  this->tcp_ctrl = tcp_ctrl;
}

TcpClientDbManager::~TcpClientDbManager() {}

void TcpClientDbManager::StartTcpClientDbManagerInit() {}

void TcpClientDbManager::AddClient(TcpClient *client) {
  if (client == nullptr)
    return;
  auto &it_list = this->tcp_clietn_db[client->comm_fd];
  it_list.push_back(client);
}

void TcpClientDbManager::DisplayClientDB() {
  for (auto it : this->tcp_clietn_db) {
    // it->Display();
  }
}