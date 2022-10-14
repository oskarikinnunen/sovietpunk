/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3lib.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 17:57:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 12:10:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V3LIB_H
# define V3LIB_H

# include "libft.h"

# define X 0
# define Y 1
# define Z 2

void	v3_mul_matrix(float matrix[3][3], float *v3);
void	v3_add_xyz(float *v3, float *add);

#endif