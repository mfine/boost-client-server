#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

#include "message.hpp"

using boost::asio::ip::tcp;

void write(tcp::socket& sock, Message m)
{
  char req[3] = { static_cast<char>(m), '\r', '\n' };
  boost::asio::write(sock, boost::asio::buffer(req));
}

bool read(tcp::socket& sock, Message& msg)
{
    char resp[3];
    boost::system::error_code error;
    size_t len = sock.read_some(boost::asio::buffer(resp), error);

    if (error == boost::asio::error::eof)
    {
      return true;
    }
    else if (error)
    {
      throw boost::system::system_error(error);
    }
    else if (len != 3)
    {
      throw missing_input();
    }
    else if (resp[1] != '\r' || resp[2] != '\n')
    {
      throw bad_input();
    }

    msg = static_cast<Message>(*resp);
    return false;
}
