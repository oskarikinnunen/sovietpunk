/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP_OBJ.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:39:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/11 05:59:32 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP_OBJ_H
# define SP_OBJ_H

#include "libft.h"
#include "inttypes.h"

typedef struct s_obj
{
	char		**mtlnames;
	uint32_t	*mtlcolors;
	uint8_t		*colors; //Points to colors in mtlcolors
	int		**verts;
	int		**faces;
	int		v_count;
	int		f_count;
}	t_obj;

/*typedef struct s_img_info
{
	int		size[2];
	void	*ptr;
	char	*data_addr;
	int		bpp;
	int		sz_line;
	int		endian;
}	t_img_info;*/

typedef struct s_fdf
{
	t_obj		*obj;
	struct s_simpleimg	*img;
	int			*depth;
	float		**verts;
	float		matrices[2][3][3];
	u_int32_t	screenspace[2];
	u_int32_t	scale;
	u_int32_t	crd[2];
	float		view[2];
}	t_fdf;

void	parse_obj(t_obj *obj);
int		fdf_init(t_fdf *fdf, struct s_simpleimg *img, t_obj *object);
void	fdf_update(t_fdf *fdf);

#endif