# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/06 13:41:07 by okinnune          #+#    #+#              #
#    Updated: 2022/10/10 15:28:05 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= wolf3d

INCLUDE= -Ilib/include/SDL2/ -Isrc -Ilibft
LIBFT= libft/libft.a
SDL2= SDL_built/lib/libSDL2.a
SRCFILES= main.c editor.c png.c file_open.c mini_fdf.c obj.c \
		image.c gameloop.c deltatime.c eventloop.c editor_eventloop.c convertpng.c \
		v2.c obj_render.c shade.c spawnplayer.c inputhelp.c v2_2.c draw.c samplemap.c \
		editor_buttons.c drawquadtile.c getwall.c mini_fdf_helpers.c mini_fdf_matrix.c \
		error.c obj_parse.c player_move.c render.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)
CC= gcc
LIBS= $(LIBFT) -ldl -lpthread -lm
override CFLAGS +=  $(INCLUDE) -Werror -Ofast -finline-functions -g#-g# -g -O2
PWD= $(shell pwd)

all: $(SDL2) $(LIBFT) $(OBJ) #src/SP1947.h
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE) $(LIBS)

shade1:
	$(MAKE) clean
	$(MAKE) CFLAGS='-D SHADE1'

shade2:
	$(MAKE) clean
	$(MAKE) CFLAGS='-D SHADETRIPPY'

$(OBJ): src/SP1947.h

$(SDL2):
	cd SDL2-2.0.22/build && ../configure --prefix=$(PWD)/SDL_built/ && make install

clean-sdl:
	rm -rf SDL2-2.0.22/build/*
	rm -rf SDL_built/*
	touch SDL_built/DontRemoveMe
	rm -f $(SDL2)

clean:
	rm -f src/*.o
	rm -f $(LIBFT)

re:	clean all

fclean: clean-sdl clean

re-sdl: clean-sdl $(SDL2)
	
#clean: rm libs subfolders
$(LIBFT):
	make -C libft

.PHONY : all shade1 shade2 clean-sdl clean re fclean re-sdl
