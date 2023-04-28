#include "network_utils.h"

char *network_convert_ip_n_to_p(uint32_t ip_addr, char *output_buffer) {
  char *buff = nullptr;
  static char str[16];
  buff = (output_buffer == nullptr) ? str : output_buffer;
  memset(buff, 0, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &ip_addr, buff, INET_ADDRSTRLEN);
  buff[15] = '\0';
  return buff;
}

uint32_t network_convert_ip_p_to_n(const char *ip_addr) {
  uint32_t ip = 0;
  inet_pton(AF_INET, ip_addr, &ip);
  return ip;
}
