/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perfgraph.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 15:20:11 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 16:15:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	drawperfgraph(t_perfgraph *graph, uint32_t delta)
{
	int			i;
	t_simpleimg	img;

	img = graph->image;
	ft_bzero(img.data, img.length * sizeof(uint32_t));
	i = 0;
	while (i < PERFGRAPH_SAMPLES - 1)
	{
		graph->deltas[i] = graph->deltas[i + 1];
		graph->deltas[i]
			= ft_clamp(graph->deltas[i], 0, graph->image.size[Y] - 1);
		img.data[i + (graph->deltas[i] * img.size[X])] = CLR_TURQ;
		i++;
	}
	graph->deltas[PERFGRAPH_SAMPLES - 1] = delta;
}
