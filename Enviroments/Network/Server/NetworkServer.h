#ifndef __NETWORK_SERVER_HEADER__
#define __NETWORK_SERVER_HEADER__

#undef _WINSOCKAPI_
#include <winsock2.h>

class NetworkServer
{
    public:
                        NetworkServer(void);
        void            StartServerNetwork(const char* Port);
    private:
        WSADATA         ServerWindowSocketData; 
        SOCKET          Server_ServerSocket;
        SOCKET          Server_ClientSocket;
        SOCKADDR_IN     Server_ServerAddress;
        SOCKADDR_IN     Server_ClientAddress;
};

#endif