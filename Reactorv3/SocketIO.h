#ifndef _SOCKETIO_H
#define _SOCKETIO_H

#include <unistd.h>

/// @brief 从套接字中读取数据、写入数据的类
class SocketIO 
{
private: 
    int _fd;

public: 
    explicit SocketIO(int fd);
    ~SocketIO();
    int readn(char* buf, int len);

    /// @brief 循环按行/换行符读取数据
    /// @param buf 接收套接字数据缓冲区
    /// @param len 接收数据长度
    /// @return 实际读取字节数 
    int readLine(char* buf, int len);

    /// @brief 循环写入数据
    /// @param buf 写入套接字数据缓冲区
    /// @param len 写入数据长度
    /// @return 实际写入字节数
    int writen(const char* buf, int len);

};

#endif //_SOCKETIO_H