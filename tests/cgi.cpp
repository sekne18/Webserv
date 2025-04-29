#include "ConcreteRequestHandlers.hpp"
#include "ConcreteResponses.hpp"
#include "HandlerContext.hpp"
#include "RequestParser.hpp"
#include "StreamLogger.hpp"

int main(int argc, char **argv, char **envp)
{
    ILogger *logger = StreamLogger::getInstance();
    logger->setLogLevel(ILogger::LOG_DEBUG);
    std::string path = argv[1];
    CgiHandler cgiHandler(".cgi");
    IRequestParser *request = new RequestParser(*logger);
    IHandlerContext *ctx = new HandlerContext();
    IResponse *response = cgiHandler.handle(*request, ctx);

    logger->logDebug("Request: " + request->getMethod() + " " + request->getTarget());
    if (response)
    {
        logger->logDebug("Response: " + toString(response->getStatus()));
        std::cout << response->serialize() << std::endl;
    }
    else
    {
        logger->logDebug("Response: No response");
    }

    delete request;
    delete ctx;
    delete response;
    StreamLogger::destroyInstance();
    return 0;
}