#include "WebSocketSession.h"
#include "WebSocketServer.h"

WebSocketSession::WebSocketSession(tcp::socket socket, WebSocketServer* server)
    : ws_(std::move(socket)), server_(server)
{
}

void WebSocketSession::start()
{
    // WebSocket 握手
    ws_.accept();

    // 通知 server：此 session 加入连接列表
    server_->addClient(shared_from_this());

    // 开始读取消息
    do_read();
}

void WebSocketSession::do_read()
{
    auto buffer = new beast::flat_buffer;

    ws_.async_read(*buffer,
        [self = shared_from_this(), buffer](beast::error_code ec, std::size_t n)
        {
            self->on_read(ec, n, buffer);
        });
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t, beast::flat_buffer* buffer)
{
    if (ec) {
        delete buffer;
        return on_close();
    }

    // 提取消息字符串
    std::string msg = boost::beast::buffers_to_string(buffer->data());
    delete buffer;

    // 广播给所有客户端
    server_->broadcast(msg);

    // 继续读下一条消息
    do_read();
}

void WebSocketSession::send(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(write_mutex_);
    ws_.text(true);
    ws_.write(boost::asio::buffer(msg));
}

void WebSocketSession::on_close()
{
    server_->removeClient(shared_from_this());
}
