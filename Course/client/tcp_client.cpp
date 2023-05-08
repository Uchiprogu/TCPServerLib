#include <iostream>
#include <unistd.h>
#include "../network_utils.h"

#define DEST_PORT            40000
#define SERVER_IP_ADDRESS   "127.0.0.1"

using namespace std;

struct  test_struct_t
{
    unsigned int a;
    unsigned int b;
};

typedef struct result_struct_{

    unsigned int c;

} result_struct_t;

int main(int argc, char **argv) {
    test_struct_t client_data;
    result_struct_t result;
    int sockfd = 0;
    int sent_recv_bytes = 0;
    struct sockaddr_in serv_addr;
    socklen_t addr_len = 0;
    uint32_t ip_addr;
    uint16_t port_no;

    addr_len = sizeof(sockaddr_in);

    if(argc <= 1){
        printf("Warning: usage %s hostname %s port %d\n", argv[0], SERVER_IP_ADDRESS, DEST_PORT);
        ip_addr = network_convert_ip_p_to_n(SERVER_IP_ADDRESS);
        port_no = DEST_PORT;
    }else{
        ip_addr =  network_convert_ip_p_to_n(argv[2]);
        port_no =  atoi(argv[1]);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    bcopy(&ip_addr, &serv_addr.sin_addr.s_addr,sizeof(uint32_t));
    if (sockfd < 0){
        printf("Error: opening socket\n");
        exit(0);
    }

    int if_connected = connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (if_connected == 0) {
    	printf("connected\n");
    }
    else {
    	printf("Error: connection failed, error no %d\n", if_connected);
    	exit(0);
    }
    printf("intput a number: ");
    scanf("%u",&client_data.a);
    printf("intput b number: ");
    scanf("%u", &client_data.b);

	if(client_data.a == 0 && client_data.b == 0) {
		close(sockfd);
		exit(0);
	}
    sent_recv_bytes = sendto(sockfd,
                             &client_data,
                             sizeof(test_struct_t),
                             0,
                             reinterpret_cast<sockaddr*>(&serv_addr),
                             addr_len);
    
    printf("No of bytes sent = %d\n", sent_recv_bytes);
    printf("Waiting for response:\n");

    sent_recv_bytes = recvfrom( sockfd,
                                &result,
                                sizeof(result_struct_t),
                                0,
                                reinterpret_cast<sockaddr*>(&serv_addr),
                                &addr_len);

    printf("No of bytes recvd = %d\n", sent_recv_bytes);
    
    printf("Result recvd = %u\n", result.c);

    return 0;
}