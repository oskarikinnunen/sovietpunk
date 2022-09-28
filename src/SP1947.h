/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:13:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/28 20:44:58 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_H
# define SP1947_H

#include "../libs/include/SDL2/SDL.h"
#include "SP_OBJ.h"
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "libft.h"
//#include "SP_PNG.h"

# define FPSCOUNTER
# define WINDOW_W 800
# define WINDOW_H 600
# define X 0
# define Y 1

# define DARKNESS 1000.0f
# define GAMESCALE	64
# define RAYSLICE	0.0015f
# define FOV		RAYSLICE * WINDOW_W
# define RAD90		1.57079633f
# define WALLTHING	(0.0017777f / RAYSLICE) * 36000 //800w rayslice 0.0018f = 452
# define TURNSPEED	0.003f
# define MOVESPEED	0.100f
# define MAPSIZE	16

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
	t_fdf					*fdfs;
	int						*ft; //floortable
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
	//Uint32			lmap[MAPSIZE * MAPSIZE];
	t_clock			clock;
	u_int32_t		lastwall;
	u_int32_t		tex_x; //TODO: REMOVE and make a local variable
	u_int32_t		v[2];
}	t_gamecontext;

/* player.c */
void	spawnplayer(t_gamecontext *gc);

/* shade.c */
u_int32_t	shade(u_int32_t color, int wallheight);

/* obj_render.c */
void	renderobj(t_gamecontext *gc);

/* v2.c */
int		v2dist(int *v, int *ov);

/* eventloop.c */
int		eventloop(t_gamecontext *gc);

/* gameloop.c */
void	gameloop(t_gamecontext gc);

/* image.c */
Uint32	samplecolor(t_simpleimg img, int ix, int iy);
void	drawimage(t_sdlcontext *context, int x, int y);
void	drawimagescaled(t_sdlcontext *context, int p[2], int tid, int scale);

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