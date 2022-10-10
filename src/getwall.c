/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getwall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 13:59:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 13:39:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

uint32_t	getindexedwall(uint32_t wall, int i)
{
	uint32_t	result;

	result = (wall & (0xFF << (8 * i))) >> 8 * i;
	result = ft_clamp(result, 0, PNG_COUNT - 1);
	return (result);
}
