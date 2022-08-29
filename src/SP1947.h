/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:13:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/29 21:37:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_H
# define SP1947_H

#include "../libs/include/SDL2/SDL.h"
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "libft.h"
//#include "SP_PNG.h"

# define WINDOW_W 720
# define WINDOW_H 720
# define X 0
# define Y 1

# define TURNSPEED	0.002f
# define MOVESPEED	0.1f
# define MAPSIZE	16
//Yeah yeah it's unauthentic to have the player coordinates as floating points,
// my brain hurt doing the integer so math so f it

typedef struct s_player
{
	float	dest[2];
	float	rot;
	float	pos[2];
	int32_t	sp_pos[2];
	float	angle;
	float	move;
}	t_player;

typedef struct s_simpleimg
{
	Uint32	size[2];
	Uint32	*data;
	Uint32	length;
}	t_simpleimg;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	SDL_Renderer			*renderer;
	//struct	s_pngdata		*textures; //convert pngdarta into simpleimages
	t_simpleimg				*images;
	//struct	s_fdf			*objects;
}	t_sdlcontext;

typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
} t_clock;

typedef struct s_gamecontext
{
	t_sdlcontext	*sdlcontext;
	t_player		player;
	Uint32			map[MAPSIZE * MAPSIZE];
	t_clock			clock;
	u_int32_t		lastwall;
	u_int32_t		tex_x; //TODO: REMOVE and make a local variable
}	t_gamecontext;

/* v2.c */
int		v2dist(int *v, int *ov);

/* eventloop.c */
int		eventloop(t_gamecontext *gc);

/* gameloop.c */
void	gameloop(t_gamecontext *gc);

/* image.c */
void	drawimage(t_sdlcontext context, int x, int y);
void	drawimagescaled(t_sdlcontext context, int p[2], int tid, int scale);

/* deltatime.c */
void	update_deltatime(t_clock *c);

/* MAP.C */
void	mapcreator(char *mapname, t_sdlcontext context);

/* simpleimage.c */
void	loadpngs(t_sdlcontext	*sdl);

/* FILE_OPEN.c */

/* \brief Protected version of the standard 'open' function
	@param filename name of the file.
	@param flags flags used to open the file. */
int		sp_fileopen(char *filename, int flags);

/* ERROR.C */
/* \brief Exits the program after printing out the requested string
	@param str string that's printed out to stderror (2) */
void	error_exit(char *str);

#endif