#include "socketTools.h"
#include <climits>
#include <algorithm>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

TcpClient::TcpClient() : socket_fd(INVALID_SOCKET), initialized(false) {
#ifdef _WIN32
    WSADATA wsaData;
    if (const auto result = WSAStartup(MAKEWORD(2, 2), &wsaData); result == 0) {
        initialized = true;
    }
    // WSAStartup 失败时 initialized 保持 false，后续操作会失败
#else
    initialized = true;
#endif
}

TcpClient::~TcpClient() {
    disconnect();
#ifdef _WIN32
    if (initialized) {
        WSACleanup();
    }
#endif
}

// 连接到服务器
bool TcpClient::connectToServer(const char *ip, int port) {
    if (!initialized) {
        return false;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET;
        return false;
    }

    if (connect(socket_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET;
        return false;
    }

    return true;
}

// 发送数据
bool TcpClient::sendData(const void *data, const size_t length) {
    if (socket_fd == INVALID_SOCKET) {
        return false;
    }
    size_t total_sent = 0;
    while (total_sent < length) {
        const auto send_size = static_cast<int>(std::min(length - total_sent, static_cast<size_t>(INT_MAX)));
#ifdef _WIN32
        const auto sent = send(socket_fd, static_cast<const char *>(data) + total_sent, send_size, 0);
        if (sent == SOCKET_ERROR) {
            return false;
        }
        total_sent += static_cast<size_t>(sent);
#else
        const ssize_t sent = send(socket_fd, static_cast<const char *>(data) + total_sent, send_size, 0);
        if (sent == SOCKET_ERROR) {
            return false;
        }
        total_sent += static_cast<size_t>(sent);
#endif
    }
    return true;
}

// 接收数据
bool TcpClient::receiveData(void *buffer, const size_t length) {
    if (socket_fd == INVALID_SOCKET) return false;
    size_t total_received = 0;
    while (total_received < length) {
        // 安全地将size_t转换为int，避免窄化转换警告
        const auto recv_size = static_cast<int>(std::min(length - total_received, static_cast<size_t>(INT_MAX)));
#ifdef _WIN32
        const auto received = recv(socket_fd, static_cast<char *>(buffer) + total_received, recv_size, 0);
        if (received == SOCKET_ERROR) {
            return false;
        }
        if (received == 0) {
            // 连接已关闭
            return false;
        }
        total_received += static_cast<size_t>(received);
#else
        const auto received = recv(socket_fd, static_cast<char *>(buffer) + total_received, recv_size, 0);
        if (received == SOCKET_ERROR) {
            return false;
        }
        if (received == 0) {
            // 连接已关闭
            return false;
        }
        total_received += static_cast<size_t>(received);
#endif
    }
    return true;
}

// 断开连接
void TcpClient::disconnect() {
    if (socket_fd != INVALID_SOCKET) {
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET;
    }
}