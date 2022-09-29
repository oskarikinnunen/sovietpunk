/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sortwalls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 18:54:40 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/29 19:29:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

#include "assert.h"

static void printwalls(int walls[WINDOW_W]){
	for (int i = 0; i < WINDOW_W; i++)
	{
		printf("w %i \n", walls[i] & 0xFFFF);
		if (i > 0 && walls[i - 1] & 0xFFFF > walls[i] & 0xFFFF)
			printf("ASSERT FAIL\n");
		
	}
	//exit(0);
}


void	sortwalls(int *walls)
{
	int	i;
	int	j;
	int	key;

	i = 1;
	while (i < WINDOW_W)
	{
		key = walls[i];
		j = i - 1;
		while (j >= 0 && (walls[j] & 0xFFFF) < (key & 0xFFFF))
		{
			walls[j + 1] = walls[j]; //swap
			j = j - 1;
		}
		walls[j + 1] = key;
		i++;
	}
	//printwalls(walls);
}