//
// Created by 郭磊 on 2023/12/12.
//

#include "CStreamServer.h"

static bool m_bServerRunning = true;

CStreamServer::CStreamServer() {
    m_ListenSocketFd = -1;
}

CStreamServer::~CStreamServer() {
    CloseServer();
}

void CStreamServer::StartServer() {
    printf("start server ....\n");

    //  socket(int domain, int type, int protocol);
    m_ListenSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ListenSocketFd < 0) {
        printf("create socket failed\n");
        CloseServer();
        return;
    }
    printf("create socket is success, m_ListenSocketFd:%d\n", m_ListenSocketFd);

    // setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
    int opt = 1;
    int len = sizeof(int);
    if (setsockopt(m_ListenSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, len)) {
        printf("setsockopt set failed\n");
        CloseServer();
        return;
    }
    // 设置不阻塞
    fcntl(m_ListenSocketFd, F_SETFL, O_NONBLOCK);

    // 开启bind
    //  bind(int socket, const struct sockaddr *address, socklen_t address_len);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(m_ListenSocketFd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("bind socket failed\n");
        CloseServer();
        return;
    }
    printf("bind socket success\n");

    // listen
    //  listen(int socket, int backlog);
    ret = listen(m_ListenSocketFd, 10);
    if (ret < 0) {
        printf("listen socket failed\n");
        CloseServer();
        return;
    }
    printf("listen socket success\n");

    signal(SIGINT, serverExitSignalProcess);

    // 开启连接 获取客户端的连接
    while (m_bServerRunning) {
        struct sockaddr_in client_addr;
        bzero(&client_addr, sizeof(client_addr));
        socklen_t size = sizeof(struct sockaddr_in);
        //  accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
        int sockfd = accept(m_ListenSocketFd, (struct sockaddr *) &client_addr, &size);
        if (sockfd > 0) {
            printf("\n==============New Connected============\n");
            printf("connect socket:%d, addr:%s\n", sockfd, inet_ntoa(client_addr.sin_addr));
            CStreamProcess *cStreamClient = new CStreamProcess(sockfd);
            cStreamClient->startWork();
            m_clients.push_back(cStreamClient);
        }
        // 搞一个管理类进行管理然后在开启线程读写
        printf("\n Server listen client ...\n");
        usleep(2000 * 1000);
    }
    return;
}

void CStreamServer::serverExitSignalProcess(int num) {
    m_bServerRunning = false;
    printf("serverExitSignalProcess \n");
}

void CStreamServer::CloseServer() {
    if (m_ListenSocketFd >= 0) {
        close(m_ListenSocketFd);
        m_ListenSocketFd = -1;
    }
}



