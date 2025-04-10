#ifndef INETWORKMANAGER_HPP
#define INETWORKMANAGER_HPP

class INetworkManager
{
public:
	virtual ~INetworkManager() {}

	virtual void initializeServer(int port, const std::string &address) = 0;
	virtual virtual void start() = 0;
	virtual void stop() = 0;

protected:
private:
	// Prevent copying
	INetworkManager(const INetworkManager &);
	INetworkManager &operator=(const INetworkManager &);
}

#endif // INETWORKMANAGER_HPP