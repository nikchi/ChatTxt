#include "Server.h"

Server::Server(io_service& service, const tcp::endpoint& endpoint)
  : mRunning(false),
    mService(service),
    mAcceptor(service, endpoint),
    mSessionMgr()
{
}

Server::~Server()
{
}

void Server::start()
{
  // start listening
  mRunning = true;
  listen();
}

void Server::stop()
{
  // stop listening
  mRunning = false;
  mSessionMgr.kill();
}

void Server::listen()
{
  // if we're not still running stop listening
  if (!mRunning)
    return;

  printf("Waiting for new connection...\n");
  SessionPtr session(new Session(mService, mSessionMgr));
  mAcceptor.async_accept(session->socket(),
			 boost::bind(&Server::accept,
				     this,
				     session,
				     boost::asio::placeholders::error));
}

void Server::accept(SessionPtr session, const error_code& error)
{
  if (mRunning)
    {
      if (!error)
	{
	  session->start();
	  printf("Connection established\n");
	}

      listen();
    }
}
