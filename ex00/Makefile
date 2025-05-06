# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akovalev <akovalev@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/29 12:33:14 by akovalev          #+#    #+#              #
#    Updated: 2025/04/30 16:44:22 by akovalev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := btc
CFLAGS := -Wall -Wextra -Werror
SRC := main.cpp BitcoinExchange.cpp
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

