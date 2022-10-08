# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/06 13:41:07 by okinnune          #+#    #+#              #
#    Updated: 2022/10/08 09:52:57 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= SovietPunk1947

INCLUDE= -Ilib/include/SDL2/ -Isrc -Ilibft
LIBFT= libft/libft.a
SDL2= SDL_built/lib/libSDL2.a
SRCFILES= main.c editor.c png.c error.c file_open.c fdf_drawline.c mini_fdf.c obj.c \
		image.c gameloop.c deltatime.c eventloop.c editor_eventloop.c simpleimg.c \
		v2.c obj_render.c shade.c player.c inputhelp.c v2_2.c draw.c samplemap.c \
		editor_buttons.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)
CC= gcc
LIBS= $(LIBFT) -ldl -lpthread -lm
override CFLAGS +=  $(INCLUDE) -g -O2# -g -O2
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

re-sdl: clean-sdl $(SDL2)
	
#clean: rm libs subfolders
$(LIBFT):
	make -C libft
