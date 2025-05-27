# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 18:07:37 by akovalev          #+#    #+#              #
#    Updated: 2025/05/07 18:07:37 by akovalev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := PmergeMe
CFLAGS := -Wall -Wextra -Werror
SRC := main.cpp PmergeMe.cpp Deque.cpp
OBJ := $(SRC:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	@c++ $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	@c++ $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean	
	@rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re

