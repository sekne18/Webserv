#include "SessionIdGenerator.hpp"
#include <iostream>

int main()
{
	SessionIdGenerator generator;
	std::set<std::string> existingIds;
	for (size_t i = 0; i < 10; ++i)
	{
		std::string sessionId = generator.generateUniqueSessionId(existingIds);
		existingIds.insert(sessionId);
		std::cout << "Generated session ID: " << sessionId << std::endl;
	}

	return 0;
}