CPP = c++
CPP_FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp src/Server.cpp src/Response.cpp \
      src/Config.cpp src/NetworkManager.cpp src/Client.cpp \
      src/CGI.cpp src/Request.cpp src/Utils.cpp

OBJ_DIR = obj
OBJS = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

NAME = webserv

# Test files
TEST_REQUEST_SRC = tests/test_request.cpp src/Request.cpp
TEST_SERVER_SRC = tests/test_server.cpp src/Server.cpp src/Request.cpp \
                  src/Response.cpp src/Config.cpp src/Utils.cpp \
                  src/Route.cpp src/Error.cpp src/Client.cpp src/CGI.cpp

# Test executables
TEST_REQUEST_NAME = test_request
TEST_SERVER_NAME = test_server

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Build and run request tests
test_request: $(TEST_REQUEST_SRC)
	$(CPP) $(CPP_FLAGS) -o $(TEST_REQUEST_NAME) $(TEST_REQUEST_SRC)
	./$(TEST_REQUEST_NAME)

# Build and run server tests
test_server: $(TEST_SERVER_SRC)
	$(CPP) $(CPP_FLAGS) -o $(TEST_SERVER_NAME) $(TEST_SERVER_SRC)
	./$(TEST_SERVER_NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(TEST_REQUEST_NAME) $(TEST_SERVER_NAME)

re: fclean all

.PHONY: all clean fclean re test_request test_server

