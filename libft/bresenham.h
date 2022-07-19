/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 04:27:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/04/28 04:48:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRESENHAM_H
# define BRESENHAM_H

typedef struct s_bresenham
{
	int	diff[2];
	int	add[2];
	int	local[3];
	int	target[2];
	int	error;
}	t_bresenham;

void	populate_bresenham(t_bresenham *b, int *from, int *to);
int		step_bresenham(t_bresenham *b);
void	step_bresenham_x(t_bresenham *b);
void	step_bresenham_y(t_bresenham *b);

#endif