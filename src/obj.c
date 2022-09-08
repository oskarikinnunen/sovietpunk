	/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:44:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/22 20:59:09 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "SP_OBJ.h"
#include "SP1947.h"
#include "v3lib.h"

int	file_open(char *filename) //TODO: use sp_fileopen
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd <= -1)
		exit(0);
	return (fd);
}

void	read_vertex(int *v3, char *line)
{
	char	**vstrs;
	int		i;

	vstrs = ft_strscrape(ft_strchr(line, 'v')+1, " \t");
	i = 0;
	while (i < 3)
	{
		ft_strreplace(vstrs[i], 'E', '\0');
		ft_strreplace(vstrs[i], '.', '\0');
		v3[i] = ft_atoi(vstrs[i]);
		free(vstrs[i]);
		i++;
	}
	free(vstrs);
	//printf("v content x %i y%i z%i \n", v3[X], v3[Y], v3[Z]);
}

void	read_face(int *v3, char *line)
{
	char	**fstrs;
	int		i;

	
	fstrs = ft_strscrape(line + 1, " \t");
	i = 0;
	while (i < 3)
	{
		v3[i] = ft_atoi(fstrs[i]) - 1;
		free(fstrs[i]);
		i++;
	}
	free(fstrs);
	//printf("f content %i->%i->%i \n", v3[X] + 1, v3[Y] + 1, v3[Z] + 1);
}

void	get_vertices(t_obj *obj, int fd)
{
	char	*line;
	int index = 0;

	obj->verts = ft_memalloc(sizeof(int **) * 10000); // Do with remalloc
	obj->faces = ft_memalloc(sizeof(int **) * 10000);
	while (ft_get_next_line(fd, &line))
	{
		if (*line == 'v' && line[1] != 'n')
		{
			obj->verts[obj->v_count] = ft_memalloc(sizeof(int *) * 3);
			read_vertex(obj->verts[obj->v_count], line);
			obj->v_count++;
		}
		if (*line == 'f' && line[1] != 'n')
		{
			obj->faces[obj->f_count] = ft_memalloc(sizeof(int *) * 3);
			read_face(obj->faces[obj->f_count], line);
			obj->f_count++;
		}
		free(line);
	}
	
}

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
		clr += ((int)(atof(strs[i]) * 255.0f) & 0xFF) << (8 * i);
		i++;
	}
	printf("clr %i %i %i \n", clr & 0xFF, (clr >> 8) & 0xFF, (clr >> 16) & 0xFF);
}

void	get_materials(t_obj *obj, int fd)
{
	char	*line;
	int		i;

	i = -1;
	obj->mtlnames = ft_memalloc(sizeof(char *) * 4); //check null
	obj->mtlcolors = ft_memalloc(sizeof(uint32_t) * 4); //TODO: find out allocation length from file
	while (ft_get_next_line(fd, &line)) 
	{
		if (ft_strncmp(line, "newmtl", 6) == 0)
		{
			i++;
			obj->mtlnames[i] = ft_strnew(20); //TODO do properly
			ft_strcpy(obj->mtlnames[i], line + 7); //TODO: UNSAFEEEE
			printf("mat found %s \n", obj->mtlnames[i]);
		}
		if (ft_strncmp(line, "Kd", 2) == 0)
			obj->mtlcolors[i] = get_color(line + 3);
		free(line);
		//if newmat, alloc
	}
}

void	parse_obj(t_obj *obj)
{
	int		fd;

	ft_bzero(obj, sizeof(t_obj));
	//load mtl
	fd = file_open("crash2.mtl");
	get_materials(obj, fd);
	close(fd);
	//exit(0);
	//load obj
	fd = file_open("crash2.obj");
	get_vertices(obj, fd);
	close(fd);
	
}