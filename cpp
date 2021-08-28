 
#include <vld.h> //基于VLD 测试内存泄漏,需要安装软件,和配置环境,不然报错
 
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<iostream>////C++ 标准的输入&输出
 
  

int main()//相当于程序的主线程函数
{
	//初始化winsock2.2相关的动态库
	WSADATA  wd;//获取socket相关信息
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)//0表示成功
	{
		std::cout << "WSAStartup  error:" << WSAGetLastError() << std::endl;
		return 0;
	}
	else
	{
		printf("Socket--->WSAStart版本指定为2.2====OK\n");
	}


	//底层之TCP_Socket2:第二步：创建TCP   socket , 流式套接字。此处无关  服务器&客户端
	SOCKET   socket_server_benzun = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_server_benzun == INVALID_SOCKET)
	{
		printf("创建TCP   socket , 流式套接字NG\n");
		std::cout << "socket  error:" << WSAGetLastError() << std::endl;
		return -1;
	}
	else
	{
		printf("创建TCP   socket , 流式套接字OK\n");
	}


	//底层之TCP_Socket2:第三步：绑定socket到一个IP地址和端口
	sockaddr_in   addr;//不建议使用sockaddr，建议用sockaddr_in
	addr.sin_port = htons(6000);//网络字节序
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//网络字节序    SDL检测需要设定为：否。不然报错
	addr.sin_family = AF_INET; //地址族
	int len = sizeof(sockaddr_in);
	if (bind(socket_server_benzun, (sockaddr*)&addr, len) == SOCKET_ERROR)//SOCKET_ERROR 本质是数值-1
	{
		std::cout << "bind  error:" << WSAGetLastError() << std::endl;
		return 0;
	}

	//底层之TCP_Socket:第四步：5代表正在等待完成相应的TCP三路握手过程的队列长度
	if (listen(socket_server_benzun, 5) == SOCKET_ERROR)
	{
		std::cout << "listen  error:" << WSAGetLastError() << std::endl;
		return 0;
	}

	//底层之TCP_Socket:第五步：.接受客户端请求，并且返回和客户端通讯的套接字
	sockaddr_in   addrClient;// 保存客户端IP地址端口 
	memset(&addrClient, 0, sizeof(sockaddr_in));
	len = sizeof(sockaddr_in);
	SOCKET gsz_huashen_client = accept(socket_server_benzun, (sockaddr*)&addrClient, &len);//成功返回套接字
	if (gsz_huashen_client == INVALID_SOCKET)
	{
		std::cout << "accept  error:" << WSAGetLastError() << std::endl;
		return 0;
	}

	//底层之TCP_Socket:第六步：发送，接受消息
	int  ret = 0;
	do
	{
		//向客户端发送数据,不能用监听套接字s，而应该用accept返回的套接字c
		ret = send(gsz_huashen_client, "I am  Server!", strlen("I am  Server!"), 0);//把flag置0

		//接受客户端的消息
		char recv_buf[64] = { '\0' };
		ret = recv(gsz_huashen_client, recv_buf, 64, 0);//把flag置0
		std::cout << "recv" << inet_ntoa(addrClient.sin_addr) << ":    " << recv_buf << std::endl;// inet_ntoa转换为IP字符串
	} while (ret != SOCKET_ERROR && ret != 0);//对方关闭，返回0 ，错误返回SOCKET_ERROR




	//底层之TCP_Socket:第七步：.关闭套接字
	closesocket(socket_server_benzun);





	if (SOCKET_ERROR == WSACleanup())
	{
		printf("WSACleanup 出错了！！");
	}
	else
	{
		printf("WSACleanup 正常完成了");
	}
 
	return 0;

}
