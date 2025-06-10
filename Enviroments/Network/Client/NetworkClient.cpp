#include "NetworkClient.h"
#include "../../Common/Console.h"

NetworkClient::NetworkClient(void) {}

void NetworkClient::StartClientNetwork(const char* Port)
{
    SysPrint("Starting client...",4);
    if (WSAStartup(MAKEWORD(2,2),&this->ClientWindowsSocketData) != 0) {SysPrint("Client startup error!",4);}
    SysPrint("Startup succeeded",4);

    this->Client_ClientSocket = socket(PF_INET,SOCK_STREAM,0);
    if (this->Client_ClientSocket == INVALID_SOCKET) {SysPrint("Invalid socket error",4);}
    SysPrint("Socket succeeded",4);

    memset(&this->Client_ServerAddress,0,sizeof(this->Client_ServerAddress));
    this->Client_ServerAddress.sin_family       = AF_INET;
    this->Client_ServerAddress.sin_addr.s_addr  = inet_addr("127.0.0.1");
    this->Client_ServerAddress.sin_port         = htons(atoi(Port));


    char Buffer[30];
    int StringLength = 0;
    StringLength = recv(this->Client_ClientSocket,Buffer,sizeof(Buffer)-1,0);
    if (StringLength == -1) {SysPrint("Read error!",4);}
    SysPrint("Read from server! message is "+(std::string)Buffer,4);

    closesocket(this->Client_ClientSocket);
    WSACleanup();
}