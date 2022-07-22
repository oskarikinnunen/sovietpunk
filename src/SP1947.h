/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:13:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/20 05:45:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_H
# define SP1947_H

#include "../libs/include/SDL2/SDL.h"
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"
//#include "SP_PNG.h"

# define WINDOW_W 1280
# define WINDOW_H 720
# define X 0
# define Y 1

typedef struct s_SDL_Context
{
	SDL_Window		*window;
	SDL_Surface		*surface;
	SDL_Renderer	*renderer;
	struct	s_pngdata		*textures;
}	t_SDL_Context;

/* MAP.C */
void	mapcreator(char *mapname, t_SDL_Context context);

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