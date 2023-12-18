//
// Created by 郭磊 on 2023/12/12.
//

#ifndef CSTREAMSERVER_CSTREAMSERVER_H
#define CSTREAMSERVER_CSTREAMSERVER_H

#include <vector>
#include "CStreamServerHeaderDef.h"
#include "CStreamProcess.h"

class CStreamServer {

public:
    CStreamServer();
    ~CStreamServer();

    void StartServer();

    void CloseServer();

    static void serverExitSignalProcess(int num);

private:
    int m_ListenSocketFd = -1;

    std::vector<CStreamProcess*> m_clients;

};


#endif //CSTREAMSERVER_CSTREAMSERVER_H
