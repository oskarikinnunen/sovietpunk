# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/06 13:41:07 by okinnune          #+#    #+#              #
#    Updated: 2022/09/29 19:01:34 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= SovietPunk1947

INCLUDE= -Ilib/include/SDL2/ -Isrc -Ilibft
LIBFT= libft/libft.a
SDL2= libs/lib/libSDL2.a
SRCFILES= main.c editor.c png.c error.c file_open.c fdf_drawline.c mini_fdf.c obj.c \
		image.c gameloop.c deltatime.c eventloop.c editor_eventloop.c simpleimg.c \
		v2.c obj_render.c shade.c player.c sortwalls.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)
CC= gcc
CFLAGS +=  $(INCLUDE) -ldl -lpthread -lm -g -O1# -g -O2
PWD= $(shell pwd)

all: $(OBJ) $(SDL2) $(LIBFT) src/SP1947.h
	$(CC) $(OBJ) -o $(NAME) `libs/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBFT) -lm

$(OBJ): src/SP1947.h

$(SDL2):
	cd SDL2-2.0.22/build && ../configure --prefix=$(PWD)/libs/ && make install

clean-sdl:
	rm -rf SDL2-2.0.22/build/*
	rm -f $(SDL2)

clean:
	rm -f src/*.o
	rm -f $(LIBFT)

re-sdl: clean-sdl $(SDL2)
	
#clean: rm libs subfolders
$(LIBFT):
	make -C libft
