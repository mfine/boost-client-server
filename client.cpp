#include <cstdlib>
#include <iostream>
#include <map>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "message.hpp"

enum State
{
  InitState,
  HelloState,
  ByeState,
};

struct Pair
{
  State state;
  Message msg;

  Pair() { }
  Pair(State s, Message m) : state(s), msg(m) { }

  bool operator<(const Pair &p) const {
    return state < p.state && msg < p.msg;
  }
};

using boost::asio::ip::tcp;

void loop(tcp::socket& sock)
{
  std::map<Pair, Pair> m;
  m[Pair(HelloState, HelloAckMsg)] = Pair(ByeState, ByeMsg);

  write(sock, HelloMsg);

  State state = HelloState;
  for (;;)
  {
    Message msg;
    bool eol = read(sock, msg);
    if (eol)
    {
      std::cerr << "Eol" << std::endl;
      break;
    }
    std::cerr << "Got: " << msg << std::endl;
    std::map<Pair, Pair>::iterator i = m.find(Pair(state, msg));
    if (i == m.end())
    {
      std::cerr << "Unknown" << std::endl;
      break;
    }
    write(sock, i->second.msg);
    state = i->second.state;
  }
}

void client(std::string host, unsigned short port)
{
  boost::asio::io_service io_service;
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(tcp::v4(), host, boost::lexical_cast<std::string>(port));
  tcp::resolver::iterator iterator = resolver.resolve(query);
  tcp::socket sock(io_service);
  boost::asio::connect(sock, iterator);
  loop(sock);
}

int main(int argc, char* argv[])
{
  try
  {
    client("localhost", 5001);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
