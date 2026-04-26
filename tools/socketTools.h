#ifndef MWCU_SOCKETTOOLS_H
#define MWCU_SOCKETTOOLS_H

#ifdef _WIN32
#include <winsock2.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif
#define SOCKET_ERROR_CODE WSAGetLastError()
#define CLOSE_SOCKET closesocket
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <cerrno>
#define SOCKET_ERROR_CODE errno
#define CLOSE_SOCKET close
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

// TCP客户端类
class TcpClient {
    SOCKET socket_fd;
    bool initialized;

public:
    TcpClient();

    ~TcpClient();

    // 连接到服务器
    bool connectToServer(const char *ip, int port);

    // 发送数据
    bool sendData(const void *data, size_t length);

    // 接收数据
    bool receiveData(void *buffer, size_t length);

    // 断开连接
    void disconnect();
};

#endif //MWCU_SOCKETTOOLS_H
