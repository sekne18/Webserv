#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "INetworkManager.hpp"
#include "IRouter.hpp"

class IServer
{
public:
	virtual ~IServer();
	virtual void start() = 0;
	virtual void stop() = 0;

protected:
	INetworkManager *_networkManager;
	IRouter *_router;

private:
	// Prevent copying
	IServer(const IServer &);
	IServer &operator=(const IServer &);
}

#endif // ISERVER_HPP