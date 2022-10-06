/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:13:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 14:47:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_H
# define SP1947_H

#include "../SDL_built/include/SDL2/SDL.h"
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

# define CLR_PRPL 14231500
# define CLR_TURQ 5505010
# define CLR_GRAY 4868682

# define DARKNESS	500.0f //On my elitebook 8460p 1000.0f is okay
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

typedef struct s_obj
{
	char		**mtlnames;
	uint32_t	*mtlcolors;
	uint8_t		*colors; //Points to colors in mtlcolors
	uint32_t	m_count;
	int32_t		**verts;
	uint32_t	**faces;
	uint32_t	v_count;
	uint32_t	f_count;
}	t_obj;

typedef struct s_fdf
{
	t_simpleimg			img; // TODO: make local
	struct s_clock		*clock;
	t_obj				*obj;
	int					curframe;
	float				*depth;
	float				**verts;
	float				matrices[2][3][3];
	float				view[2];
	u_int32_t			frames;
	int32_t				screenspace[2];
	u_int32_t			scale;
	int32_t				crd[2];
}	t_fdf;

typedef struct s_sdlcontext
{
	SDL_Window				*window;
	SDL_Surface				*surface;
	t_simpleimg				*images;
	t_fdf					fdfs[1];
	int						*ft; //floortable
}	t_sdlcontext;

typedef struct s_clock
{
	Uint32	prev_time;
	Uint32	delta;
} t_clock;

typedef struct s_gamecontext
{
	t_sdlcontext	sdl;
	t_player		player;
	Uint32			map[MAPSIZE * MAPSIZE];
	t_clock			clock;
	u_int32_t		tex_x; //TODO: REMOVE and make a local variable
	u_int32_t		v[2];
}	t_gamecontext;



/* player.c */
void	spawnplayer(t_gamecontext *gc);

/* shade.c */
u_int32_t	shade(u_int32_t color, int wallheight);

/* obj_render.c */
void	drawfdf(t_sdlcontext *context, t_fdf fdf, int *walls);
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
void	alloc_image(t_simpleimg *img, int width, int height);

/* fdf.c / obj.c */
void	parse_obj(t_obj *obj);
int		fdf_init(t_fdf *fdf, t_obj *object);
void	fdf_update(t_fdf *fdf);


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