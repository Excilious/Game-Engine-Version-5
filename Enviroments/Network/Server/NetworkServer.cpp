#include "NetworkServer.h"
#include "../../Common/Console.h"

NetworkServer::NetworkServer(void) {}

void NetworkServer::StartServerNetwork(const char* Port)
{
    SysPrint("Requesting permission...",5);
    SysPrint("Starting Server...",5);
    if (WSAStartup(MAKEWORD(2,2),&this->ServerWindowSocketData) != 0)
    {SysPrint("Server startup failed!",5); return;}
    SysPrint("Started server!",5);

    this->Server_ServerSocket = socket(PF_INET,SOCK_STREAM,0);
    if (this->Server_ServerSocket == INVALID_SOCKET) {SysPrint("Socket error!",5); return;}
    SysPrint("Started socket!",5);

    memset(&this->Server_ServerAddress,0,sizeof(this->Server_ServerAddress));
    Server_ServerAddress.sin_family        = AF_INET; //Ipv4
    Server_ServerAddress.sin_addr.s_addr   = htonl(INADDR_ANY);
    Server_ServerAddress.sin_port          = htons(std::atoi(Port));

    if (bind(this->Server_ServerSocket,(SOCKADDR*)&this->Server_ServerAddress,sizeof(this->Server_ServerAddress)) == SOCKET_ERROR)
    {SysPrint("listen() error",5);}
    SysPrint("Binding on port "+(std::string)Port,5);

    if (listen(this->Server_ServerSocket,5) == SOCKET_ERROR) {SysPrint("Listen socket error!",5);}
    SysPrint("Listen socket succeeded",5);

    int ClientAddressServer = sizeof(Server_ClientAddress);
    this->Server_ClientSocket = accept(this->Server_ServerSocket,(SOCKADDR*)&this->Server_ClientAddress,&ClientAddressServer);
    if (this->Server_ClientSocket == INVALID_SOCKET) {SysPrint("Accept error!",5);}
    SysPrint("Accept succeeded",5);

    char buff[] = "Hello there!";
    int SendLength = send(this->Server_ClientSocket,buff,sizeof(buff),0);

    closesocket(this->Server_ClientSocket);
    closesocket(this->Server_ServerSocket);
    WSACleanup();
}