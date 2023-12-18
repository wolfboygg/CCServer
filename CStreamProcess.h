//
// Created by 郭磊 on 2023/12/14.
//

#ifndef CSTREAMSERVER_CSTREAMPROCESS_H
#define CSTREAMSERVER_CSTREAMPROCESS_H

#include "CStreamServerHeaderDef.h"

class CStreamProcess {
public:
    CStreamProcess();

    CStreamProcess(int sockfd);

    ~CStreamProcess();

    void startWork();

    void RunSendWork();

    void RunRecvWork();

private:

    static void startRecvThread(long long userData);

    static void startSendThread(long long userData);

private:
    bool recvSocketData(uint8_t *buff, unsigned int len);

    bool sendSocketData(uint8_t *buff, unsigned int len);


private:
    int m_sockId;

    pthread_mutex_t  m_recvMute;
    pthread_mutex_t  m_sendMute;

};


#endif //CSTREAMSERVER_CSTREAMPROCESS_H
