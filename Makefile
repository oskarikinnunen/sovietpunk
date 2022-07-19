# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/06 13:41:07 by okinnune          #+#    #+#              #
#    Updated: 2022/07/08 12:55:16 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= SovietPunk1947

INCLUDE= -Ilib/include/SDL2/ -Isrc -Ilibft
LIBFT= libft/libft.a
SDL2= libs/lib/libSDL2.a
SRCFILES= main.c map.c png.c error.c file_open.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)
CC= gcc
CFLAGS += $(INCLUDE) -ldl -lpthread
PWD= $(shell pwd)

all: $(OBJ) $(SDL2) $(LIBFT)
	$(CC) $(OBJ) -o $(NAME) `libs/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBFT)

$(SDL2):
	cd SDL2-2.0.22/build && ../configure --prefix=$(PWD)/libs/ && make install

$(LIBFT):
	make -C libft
