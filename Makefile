NAME	 =	ircserv
FLAGS	 =	-std=c++98 -Wall -Wextra -Werror -g -fsanitize=address
INCLUDE  =	-Iinc/
DIR_OBJS =	objs/
DIR_SRC  = src/

# *******************************	FILES	******************************* #

FILES		=	main.cpp Server.cpp Client.cpp Channel.cpp utils.cpp commands.cpp
FILES_SRC	=	$(addprefix $(DIR_SRC),$(FILES))

# *******************************  OBJECTS	******************************* #

OBJS	=	$(FILES_SRC:%.cpp=$(DIR_OBJS)%.o) #$(addprefix $(DIR_OBJS),$(FILES_SRC:.cpp=.o))
DEPS	=	$(OBJS:%.o=%.d) #$(OBJS:.o=.d)

# *******************************  COLORS	******************************* #

RED			=	\033[0;31m
GREEN		=	\033[0;32m
YELLOW		=	\033[0;33m
BLUE		=	\033[0;34m
PURPLE		=	\033[0;35m
CYAN		=	\033[0;36m
RESET		=	\033[0m
GREEN_BOLD	=	\033[1;32m
BLUE_BOLD	=	\033[1;34m
CYAN_BOLD	=	\033[1;36m

# ********************************  RULES   ******************************* #

all: $(DIR_OBJS) $(NAME)

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	@echo "${BLUE_BOLD}Internet Relay Chat ${GREEN}compiled ✅\n${RESET}"

$(DIR_OBJS)%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	c++ $(FLAGS)  $(INCLUDE) -MMD -c $< -o $@
	@echo "${YELLOW}Compiling ${RESET}$@...${RESET}"

clean:
	@rm -rf $(DIR_OBJS)
	@echo "${RED}Deleting${RESET} all objects 🗑"

fclean: clean
	@rm -f $(NAME)
	@echo "${BLUE_BOLD}Internet Relay Chat ${RED}deleted${RESET}"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re