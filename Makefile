CC := c++
CFLAGS := -g -std=c++98 -Wall -Wextra -Werror

SRC_DIR := src
INC_DIRS := include include/concrete include/interfaces
OBJ_DIR := obj

LINK_FLAGS :=
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

FILES := main.cpp Lexer.cpp Utils.cpp Parser.cpp ServerConfig.cpp Validators.cpp Server.cpp \
			RequestParser.cpp Dispatcher.cpp NetworkManager.cpp StreamLogger.cpp structs.cpp assert.cpp \
			Connection.cpp ConcreteResponses.cpp ILogger.cpp MessageException.cpp 
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
OBJS := $(addprefix $(OBJ_DIR)/, $(FILES:.cpp=.o))
NAME := webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INC_FLAGS) -o $@ $(OBJS) $(LINK_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
