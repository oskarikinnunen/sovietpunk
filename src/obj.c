/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 20:44:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 15:14:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "SP1947.h"
#include "v3lib.h"

#define MC 10 //MATERIAL COUNt

void	read_vertex(int *v3, char *line)
{
	char	**vstrs;
	int		i;

	vstrs = ft_strscrape(ft_strchr(line, 'v') + 1, " \t");
	i = 0;
	if (vstrs == NULL)
		return ;
	while (i < 3)
	{
		v3[i] = ft_atoi(vstrs[i]);
		free(vstrs[i]);
		i++;
	}
	free(vstrs);
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
}

//Returns just the index that can be used to access the color from materials
uint8_t	materialcolor(char *name, t_obj o)
{
	uint8_t	i;

	i = 0;
	while (i < o.m_count)
	{
		if (ft_strcmp(name, o.mtlnames[i]) == 0)
			break ;
		i++;
	}
	if (i == MC)
		error_exit("exceed materialcount");
	return (i + 1);
}

void	parse_anim(t_obj *obj, char *name, int framecount)
{
	int		i;
	int		fd;
	char	oname[100];
	char	*i_str;

	i = 0;
	ft_bzero(oname, sizeof(char [100]));
	ft_strcat(ft_strcpy(oname, name), "_0000");
	while (i < framecount)
	{
		ft_strcpy(name, oname);
		if (i < 10)
			ft_strcat(name, "0");
		i_str = ft_itoa(i);
		ft_strcat(ft_strcat(name, i_str), ".obj");
		free(i_str);
		fd = sp_fileopen(name, O_RDONLY);
		if (i != 0)
			cpy_materials(&obj[i], &obj[0]);
		get_vertices(&(obj[i]), fd);
		close(fd);
		i++;
	}
}

void	parse_obj(t_obj *obj)
{
	int		fd;
	char	name[100];

	ft_bzero(obj, sizeof(t_obj [ANIMFRAMES]));
	fd = sp_fileopen("animtests/cyborg/cy_000000.mtl", O_RDONLY);
	get_materials(obj, fd);
	close(fd);
	ft_bzero(name, sizeof(char [100]));
	ft_strcat(name, "animtests/cyborg/cy");
	parse_anim(obj, name, ANIMFRAMES);
}
