BG = \033[1;32m
NR = \033[0;31m
DF = \033[0m

C = clang
C_FLAGS = -Wall -Werror -Wextra -pthread
LINK_OPTIONS = -pthread
SRC_SUF = .c

FCLEAN=.fclean
CLEAN=.clean
NTBD="Nothing to be done for '$@'"

NAME = philo_three
INCLUDES =
SRCS = ft_atoi\
		ft_isdigit\
		main\
		philos_live\
		utils_1\
		utils_2\
		utils_3

all : $(NAME)

$(NAME): $(addsuffix .o,$(SRCS))
	@$(C) $^ -o $(NAME) $(LINK_OPTIONS)
	@echo "$(BG)Link executable $(NAME)$(DF)"
	@touch $(FCLEAN)
	@touch $(CLEAN)

clean :
ifneq ($(shell find . -maxdepth 1 -name $(CLEAN)),)
	@$(RM) *.d
	@$(RM) $(addsuffix .o,$(SRCS))
	@$(RM) $(CLEAN)
	@@echo "$(NR)Remove object files$(DF)"
else
	@echo $(NTBD)
endif

fclean :
ifneq ($(shell find . -maxdepth 1 -name $(FCLEAN)),)
	@make -s clean
	@echo "$(NR)Remove executable file$(DF)"
	@$(RM) $(FCLEAN)
	@$(RM) $(NAME)
else
	@echo $(NTBD)
endif

re : fclean all

%.o: %$(SRC_SUF)
	@$(C) $(C_FLAGS) -c $< -MD
	@echo "$(BG)Compile $<.o$(DF)"

include $(wildcard *.d)

.PHONY: all clean fclean re
