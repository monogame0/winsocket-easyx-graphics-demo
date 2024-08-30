#include "includes.h"

void close(SOCKET& sock)
{
    closegraph();
    closesocket(sock);
    WSACleanup();
}

bool init(SOCKET& clntSock)
{
	
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult!= 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }
    

	clntSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clntSock == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        return false;
    }

	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));			
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ServerIP);
	sockAddr.sin_port = htons(ServerPort);
	connect(clntSock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));
    
    initgraph(WindowWidth, WindowHeight);
	return true;
}
int GetStringNumber1(char* buffer)
{
    char* return_ = new char[10];
    for(int i = 0; i < 20; i++)
    {
        if(*(buffer+i) == '|')
        {
            for(int j = 0; j < i; j++)
            {
                return_[j] = *(buffer+j);
            }
        }
    }
    return std::stoi(return_);
}
int GetStringNumber2(char* buffer)  //将字符串中第一个'|'后的字符提取出来并转换为int
{
    char* return_ = new char[10];
    for(int i = 0; i < 20; i++)
    {
        if(*(buffer+i) == '|')
        {
            for(int j = i; j < strlen(buffer); j++)
            {
                return_[j - i - 1] = *(buffer+j);
            }
        }
    }
    return std::stoi(return_);
}
int main(int argc, char* argv[])
{
	SOCKET clntSock;
    
    if(!init(clntSock))
    {
        close(clntSock);
        return 0;
    }
    char* buffer = new char[1024];
    while(true)
    {
        if(GetKeyState(VK_ESCAPE) & 0x8000)
        {
            break;
        }
        recv(clntSock, buffer, 20, 0);
        std::cout << buffer << std::endl;
        putpixel(GetStringNumber1(buffer), GetStringNumber2(buffer), RGB(255, 0, 0));
    }
    close(clntSock);
    return 0;
}
