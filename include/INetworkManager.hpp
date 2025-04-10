#ifndef INETWORKMANAGER_HPP
#define INETWORKMANAGER_HPP

#include <string>

class INetworkManager
{
public:
	virtual ~INetworkManager() {}

	virtual void initializeServer(int port, const std::string &address) = 0;
	virtual std::string getNextRequest() = 0;
	virtual void sendResponse(const std::string &response) = 0;

protected:
private:
	// Prevent copying
	INetworkManager(const INetworkManager &);
	INetworkManager &operator=(const INetworkManager &);
}

#endif // INETWORKMANAGER_HPP