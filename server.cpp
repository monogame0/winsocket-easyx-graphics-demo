#include "includes.h"

int is = 0;
void close(SOCKET& servSock, SOCKET clntSock)
{
    closegraph();
    closesocket(clntSock);
    closesocket(servSock);
    WSACleanup();
}
bool init(SOCKET& servSock, SOCKET& clntSock)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0)
    {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return false;
    }

	servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(servSock == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        return false;
    }

	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));		
	sockAddr.sin_family = PF_INET;				
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
	sockAddr.sin_port = htons(ServerPort);			//�?�?
	if(bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }

	//进入监听状�?
	listen(servSock, 20);

	//接收客户�?消息
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

    if(clntSock == INVALID_SOCKET)
    {
        
        std::cout << "Error at accept(): " << WSAGetLastError() << std::endl;
        return false;
    }
    else
    {
        std::cout << "Client connected!" << std::endl;
    }
    initgraph(WindowWidth, WindowHeight);
    SetWindowText(GetHWnd(), "Server");
	return true;
}

void SendData(SOCKET& clntSock, int i)
{
    char* msg = (char*)malloc(sizeof(char) * 100);
    if(msg == NULL)
    {
        std::cout << "Memory allocation failed!" << std::endl;
        return;
    
    }
    MOUSEMSG Mmsg = GetMouseMsg();
    sprintf(msg, "%d|%d", Mmsg.x, Mmsg.y);
    send(clntSock, msg, 20, 0);
    free(msg);
}

int main(int argc, char* args[])
{
    std::cout << "Server started!" << std::endl;
    SOCKET servSock, clntSock;

    if(!init(servSock, clntSock))
    {
        std::cout << "Init failed!" << std::endl;
        close(servSock, clntSock);
        return 1;
    }
    else
    {
        std::cout << "Init success!" << std::endl;
    }
    int i = 0;
    while(true)
    {
        if(GetKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
        SendData(clntSock, i);
        i++;
    }
    close(servSock, clntSock);
    return 0;
}

