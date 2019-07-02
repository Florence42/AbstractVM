# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by marvin            #+#    #+#              #
#    Updated: 2019/05/31 13:21:08 by frivaton         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = avm

SRC = src
OBJ = obj

SCFILES = AbstractVM.cpp AbstractVM_OP.cpp Operand.cpp Log.cpp main.cpp
SHFILES = AbstractVM.hpp IOperand.hpp Operand.hpp Log.hpp MutantStack.hpp

CFILES = $(patsubst %.cpp, $(SRC)/%.cpp, $(SCFILES))
HFILES = $(patsubst %.hpp, $(SRC)/%.hpp, $(SHFILES))
OFILES = $(patsubst %.cpp, $(OBJ)/%.obj, $(SCFILES))
DFILES = $(patsubst %.cpp, $(OBJ)/%.odb, $(SCFILES))

FLAGS = -Wall -Werror -Wextra -std=c++14 -pedantic

#standard version
$(OBJ)/%.obj: $(SRC)/%.cpp $(HFILES)
	clang++ -c $< -o $@ $(FLAGS)

$(NAME): $(OBJ) $(OFILES) $(HFILES)
	clang++ $(OFILES) $(FLAGS) -o $@

#debug version
$(OBJ)/%.odb: $(SRC)/%.cpp $(HFILES)
	clang++ -c $< -o $@ $(FLAGS) -O0 -g

$(NAME)d: $(OBJ) $(DFILES) $(HFILES)
	clang++ $(DFILES) $(FLAGS) -o $@

$(OBJ):
	mkdir -p $(OBJ)

all: $(NAME)

dbg: $(NAME)d

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f $(NAME)d

re:	fclean all

.PHONY: clean fclean
