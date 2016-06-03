#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum Message
{
  HelloMsg,
  HelloAckMsg,
  ByeMsg,
  ByeAckMsg,
};

struct end_of_file : std::exception
{
};

struct missing_input : std::exception
{
};

struct bad_input : std::exception
{
};

void write(boost::asio::ip::tcp::socket& sock, Message m);
bool read(boost::asio::ip::tcp::socket& sock, Message& m);

#endif // MESSAGE_HPP
