#ifndef WEBSOCKET_SESSION_H
#define WEBSOCKET_SESSION_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
namespace beast = boost::beast;

class WebSocketServer; // 前置声明，避免循环包含

// 代表一个客户端连接（session）
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
    // 构造函数：传入 socket 和 server 指针
    WebSocketSession(tcp::socket socket, WebSocketServer* server);

    // 启动 session：握手 -> 循环读写
    void start();

    // 向该客户端发送消息（广播时调用）
    void send(const std::string& msg);

private:
    websocket::stream<tcp::socket> ws_;  // WebSocket 流
    WebSocketServer* server_;            // 指向管理器（用于广播）
    std::mutex write_mutex_;             // 写锁，防止多个线程同时写

    // 主循环：持续读取消息
    void do_read();

    // 读取到消息后调用
    void on_read(beast::error_code ec, std::size_t bytes_transferred, beast::flat_buffer* buffer);

    // session 关闭时调用
    void on_close();
};

#endif
