#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

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
  m[Pair(InitState,  HelloMsg)] = Pair(HelloState, HelloAckMsg);
  m[Pair(HelloState, ByeMsg)]   = Pair(ByeState,   ByeAckMsg);

  State state = InitState;
  for (;;)
  {
    Message msg;
    bool eol = read(sock, msg);
    if (eol)
    {
      std::cerr << "Eol" << std::endl;
      break;
    }
    std::cerr << "Server got: " << msg << std::endl;
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

void server(unsigned short port)
{
  boost::asio::io_service io_service;
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));

  for (;;)
  {
    tcp::socket sock(io_service);
    a.accept(sock);
    loop(sock);
  }
}

int main(int argc, char* argv[])
{
  try
  {
    server(5001);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
