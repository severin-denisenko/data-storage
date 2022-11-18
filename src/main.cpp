#include <iostream>
#include <optional>
#include <vector>
#include <queue>
#include <unordered_set>

#include "DataBase.h"
#include "Logger.h"
#include "Serial.h"

#include <boost/asio.hpp>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

using message_handler = std::function<void (std::string)>;
using error_handler = std::function<void ()>;

class session : public std::enable_shared_from_this<session>
{
public:

    session(tcp::socket&& socket)
            : m_socket(std::move(socket))
    {
    }

    void start(message_handler&& on_message, error_handler&& on_error)
    {
        this->m_on_message = std::move(on_message);
        this->m_on_error = std::move(on_error);
        async_read();
    }

    void post(std::string const& message)
    {
        bool idle = outgoing.empty();
        outgoing.push(message);

        if(idle)
        {
            async_write();
        }
    }

private:

    void async_read()
    {
        io::async_read_until(m_socket, m_stream_buffer, "\n", std::bind(&session::on_read, shared_from_this(), _1, _2));
    }

    void on_read(error_code error, std::size_t bytes_transferred)
    {
        if(!error)
        {
            std::stringstream message;
            message << m_socket.remote_endpoint(error) << ": " << std::istream(&m_stream_buffer).rdbuf();
            m_stream_buffer.consume(bytes_transferred);
            m_on_message(message.str());
            async_read();
        }
        else
        {
            m_socket.close(error);
            m_on_error();
        }
    }

    void async_write()
    {
        io::async_write(m_socket, io::buffer(outgoing.front()), std::bind(&session::on_write, shared_from_this(), _1, _2));
    }

    void on_write(error_code error, std::size_t bytes_transferred)
    {
        if(!error)
        {
            outgoing.pop();

            if(!outgoing.empty())
            {
                async_write();
            }
        }
        else
        {
            m_socket.close(error);
            m_on_error();
        }
    }

    tcp::socket m_socket;
    io::streambuf m_stream_buffer;
    std::queue<std::string> outgoing;
    message_handler m_on_message;
    error_handler m_on_error;
};

class server
{
public:

    server(io::io_context& io_context, std::uint16_t port)
            : m_io_context(io_context)
            , m_acceptor  (io_context, tcp::endpoint(tcp::v4(), port))
    {
    }

    void async_accept()
    {
        m_socket.emplace(m_io_context);

        m_acceptor.async_accept(*m_socket, [&] (error_code error)
        {
            auto client = std::make_shared<session>(std::move(*m_socket));
            client->post("Welcome to chat\n\r");
            post("We have a newcomer\n\r");

            m_clients.insert(client);

            client->start
                    (
                            std::bind(&server::post, this, _1),
                            [&, weak = std::weak_ptr(client)]
                            {
                                if(auto shared = weak.lock(); shared && m_clients.erase(shared))
                                {
                                    post("We are one less\n\r");
                                }
                            }
                    );

            async_accept();
        });
    }

    void post(std::string const& message)
    {
        for(auto& client : m_clients)
        {
            client->post(message);
        }
    }

private:

    io::io_context& m_io_context;
    tcp::acceptor m_acceptor;
    std::optional<tcp::socket> m_socket;
    std::unordered_set<std::shared_ptr<session>> m_clients;
};

int main()
{
    Logger::Init("./Logs", "DataBase.log");

    /*
    DataBase data_base = DataBase("Databases/testdb");

    std::vector<double> data = {1, 3, 4, 5};

    data_base.Put("some shit", Serial::Serialize(data));

    auto new_data = Serial::Deserialize<std::vector<double>>(data_base.Get("some shit"));

    for(auto item: new_data){
        LOG(INFO) << item << std::endl;
    }

    data_base.Iterate([](const std::string& str){
        LOG(INFO) << str << std::endl;
    });
     */

    io::io_context io_context;
    server srv(io_context, 15001);
    srv.async_accept();
    io_context.run();

    return 0;
}
