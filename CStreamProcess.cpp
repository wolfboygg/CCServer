//
// Created by 郭磊 on 2023/12/14.
//

#include "CStreamProcess.h"
#include "CThread.h"


CStreamProcess::CStreamProcess() {
}

CStreamProcess::CStreamProcess(int sockfd) : m_sockId(sockfd) {
    pthread_mutex_init(&m_recvMute, NULL);
    pthread_mutex_init(&m_sendMute, NULL);
}

void CStreamProcess::startWork() {
    detach_thread_create(NULL, (void *) startRecvThread, this);

    detach_thread_create(NULL, (void *) startSendThread, this);
}

void CStreamProcess::startRecvThread(long long userData) {
    CStreamProcess *client = reinterpret_cast<CStreamProcess *>(userData);
    if (client != NULL) {
        client->RunRecvWork();
    }
}

void CStreamProcess::RunRecvWork() {
    while (1) {
        usleep(2000 * 1000);
        CC_NetMsgHeader msgHeader;
        memset(&msgHeader, 0, sizeof(CC_NetMsgHeader));
        int result = recvSocketData((uint8_t *) &msgHeader, sizeof(CC_NetMsgHeader));
        if (result) {
            if (strncmp("CCTC", msgHeader.header, sizeof(msgHeader.header)) == 0) {
                if (NET_MESSAGE_TYPE_HEART_BEAT == msgHeader.type) {
                    printf("recv data heart beat success\n");
                }
            }
        }
    }
}

void CStreamProcess::startSendThread(long long userData) {
    CStreamProcess *client = reinterpret_cast<CStreamProcess *>(userData);
    if (client != NULL) {
        client->RunSendWork();
    }
}

void CStreamProcess::RunSendWork() {
    while (1) {
        usleep(2000 * 1000);
        printf("send data....\n");
    }
}


bool CStreamProcess::recvSocketData(uint8_t *buff, unsigned int len) {
    signal(SIGPIPE, SIG_IGN);

    pthread_mutex_lock(&m_recvMute);

    // 将从socket中读取到的数据写到buffer中
    int recvLen = 0;
    int nRet = 0;
    while (recvLen < len) {
        nRet = recv(m_sockId, buff, len - recvLen, 0);
        if (nRet < 0) {
            if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK) {
                printf("recv data mRet:%d\n", nRet);
                usleep(10 * 1000);
                continue;
            }
        }
        if (-1 == nRet || 0 == nRet) {
            pthread_mutex_unlock(&m_recvMute);
            printf("recv data error\n");
            return false;
        }
        recvLen += nRet;
        buff += nRet;
    }

    pthread_mutex_unlock(&m_recvMute);
    printf("recv data is success, data length %d\n", recvLen);
    return true;
}

bool CStreamProcess::sendSocketData(uint8_t *buff, unsigned int len) {
    signal(SIGPIPE, SIG_IGN);
    pthread_mutex_lock(&m_sendMute);
    // 将buffer中的数据发送到socket
    int sendLen = 0;
    int nRet = 0;
    while (sendLen < len) {
        nRet = send(m_sockId, buff, len - sendLen, 0);

        if (nRet < 0) {
            if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK) {
                printf("send data mRet:%d\n", nRet);
                usleep(10 * 1000);
                continue;
            }
        }
        if (-1 == nRet || 0 == nRet) {
            pthread_mutex_unlock(&m_sendMute);
            printf("send data error\n");
            return false;
        }

        sendLen += nRet;
        buff += nRet;
    }
    printf("send data is success\n");
    pthread_mutex_unlock(&m_sendMute);
    return true;
}

CStreamProcess::~CStreamProcess() {
    pthread_mutex_destroy(&m_recvMute);
    pthread_mutex_destroy(&m_sendMute);
}