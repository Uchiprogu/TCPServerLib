#include <iostream>
#include <stdint.h>
#include <string>

#include "TcpServerController.h"

using namespace std;

int main(int argc, char **argv) {
  TcpServerController *tcp_server =
      new TcpServerController("127.0.0.1", 40000, "Default TCP Server");
  tcp_server->start();
  scanf("\n");
  return 0;
}