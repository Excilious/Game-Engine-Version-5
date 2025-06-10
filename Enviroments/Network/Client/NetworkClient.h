#ifndef __NETWORK_CLIENT_HEADER__
#define __NETWORK_CLIENT_HEADER__

#undef _WINSOCKAPI_
#include <winsock2.h>

class NetworkClient
{
    public:
                        NetworkClient(void);
        void            StartClientNetwork(const char* Port);
    private:
        WSADATA         ClientWindowsSocketData;
        SOCKET          Client_ClientSocket;
        SOCKADDR_IN     Client_ServerAddress;
};

#endif