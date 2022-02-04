#ifndef __TcpClientServiceManager__
#define __TcpClientServiceManager__

#include <semaphore.h>
#include <pthread.h>
#include <list>

#define MAX_CLIENT_SUPPORTED 127

class TcpServer;
class TcpClient;
class TcpClientServiceManager{

    private:
        int max_fd;
        int udp_fd;
        std::list<TcpClient *>tcp_client_db;
        fd_set active_fd_set;
        fd_set backup_fd_set;
        int GetMaxFd();
        pthread_t *client_svc_mgr_thread;
        sem_t wait_for_thread_operation_to_complete;
        sem_t sem0_1, sem0_2;
        void (*client_msg_recvd)(const TcpClient *, unsigned char *, uint16_t);
        void (*client_disconnected)(const TcpClient *);
        pthread_rwlock_t rwlock;
        bool listen_clients; 
        void ForceUnblockSelect();
        void TcpClientMigrate(TcpClient *);
    public:
        TcpServer *tcp_server;
        TcpClientServiceManager(TcpServer *);
        ~TcpClientServiceManager();

        void StartTcpClientServiceManagerThread();
        void StartTcpClientServiceManagerThreadInternal();
        void StopTcpClientServiceManagerThread();
        void StopListeningAllClients();
        void ClientFDStartListen(TcpClient *);
        void ClientFDStopListen(TcpClient *);
        TcpClient* ClientFDStopListen(uint32_t , uint16_t);
        TcpClient* LookUpClientDB(uint32_t , uint16_t);
        void Stop();
        void SetClientMsgRecvCbk(
                    void (*client_msg_recvd)(const TcpClient *, unsigned char *, uint16_t));
        void SetClientDisconnectCbk(void (*client_disconnected)(const TcpClient *));
        void SetListenAllClientsStatus(bool status);
};

#endif 
