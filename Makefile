CC = c++
NAME = webserv
FLAGS = -Wall -Wextra -Werror --std=c++98
OBJ_DIR = obj
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC = main.cpp $(SRC_FILES)
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRC)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
