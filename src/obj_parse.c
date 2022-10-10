/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:30:09 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 15:13:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

static void	verticeprotect(t_obj *obj, int v_or_f)
{
	if (obj->verts == NULL
		|| obj->faces == NULL || obj->colors == NULL)
		error_exit("alloc fail");
	if (v_or_f != -1)
	{
		if (v_or_f == 1 && obj->verts[obj->v_count] == NULL)
			error_exit("alloc fail");
		if (v_or_f == 0 && obj->faces[obj->f_count] == NULL)
			error_exit("alloc fail");
	}
}

void	get_vertices(t_obj *obj, int fd)
{
	char	*line;

	obj->verts = ft_memalloc(sizeof(int **) * 10000);
	obj->faces = ft_memalloc(sizeof(int **) * 10000);
	obj->colors = ft_memalloc(sizeof(uint8_t *) * 10000);
	verticeprotect(obj, -1);
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strncmp("usemtl", line, 6) == 0)
			obj->colors[obj->f_count] = materialcolor(line + 7, *obj);
		if (*line == 'v' && line[1] != 'n' && line[1] != 't')
		{
			obj->verts[obj->v_count] = ft_memalloc(sizeof(int *) * 3);
			verticeprotect(obj, 1);
			read_vertex(obj->verts[obj->v_count++], line);
		}
		if (*line == 'f' && line[1] != 'n')
		{
			obj->faces[obj->f_count] = ft_memalloc(sizeof(int *) * 3);
			verticeprotect(obj, 0);
			read_face(obj->faces[obj->f_count++], line);
		}
		free(line);
	}
}

//color is reversed cuz I'm too lazy to change the sdl format
uint32_t	get_color(char *line)
{
	char	**strs;
	int		i;
	int		clr;

	strs = ft_strsplit(line, ' ');
	i = 0;
	clr = 0;
	while (i < 3)
	{
		clr += (atoi(strs[i]) & 0xFF) << (8 * (2 - i));
		free(strs[i]);
		i++;
	}
	free(strs);
	return (clr);
}

//TODO: find out allocation lengths from file
void	get_materials(t_obj *obj, int fd)
{
	char	*line;
	int		i;

	i = -1;
	obj->mtlnames = ft_memalloc(sizeof(char *) * MC);
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t) * MC);
	if (obj->mtlcolors == NULL || obj->mtlnames == NULL)
		return ;
	while (ft_get_next_line(fd, &line))
	{
		if (ft_strncmp(line, "newmtl", 6) == 0)
		{
			i++;
			obj->mtlnames[i] = ft_strnew(20);
			ft_strcpy(obj->mtlnames[i], line + 7);
		}
		if (ft_strncmp(line, "Kd", 2) == 0)
			obj->mtlcolors[i] = get_color(line + 3);
		free(line);
	}
	obj->m_count = i;
}

void	cpy_materials(t_obj *dst, t_obj *src)
{
	int	i;

	dst->mtlcolors = ft_memalloc(sizeof(uint32_t) * MC);
	dst->mtlnames = ft_memalloc(sizeof (char *) * MC);
	if (dst->mtlcolors == NULL || dst->mtlnames == NULL)
		error_exit("alloc fail");
	ft_memcpy(dst->mtlcolors, src->mtlcolors, src->m_count * sizeof(uint32_t));
	i = 0;
	while (i < src->m_count)
	{
		dst->mtlnames[i] = ft_strnew(20);
		if (dst->mtlnames[i] == NULL)
			error_exit("alloc fail");
		ft_strcpy(dst->mtlnames[i], src->mtlnames[i]);
		i++;
	}
	dst->m_count = src->m_count;
}
