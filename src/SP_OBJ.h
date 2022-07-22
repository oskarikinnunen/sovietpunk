/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP_OBJ.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 19:39:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/22 20:45:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP_OBJ_H
# define SP_OBJ_H

#include "libft.h"

typedef struct s_obj
{
	int	**verts;
	int	**faces;
	int	v_count;
	int	f_count;
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
	float		view[2];
}	t_fdf;

void	parse_obj(t_obj *obj);
int		fdf_init(t_fdf *fdf, struct s_simpleimg *img, t_obj *object);
void	fdf_update(t_fdf *fdf);

#endif