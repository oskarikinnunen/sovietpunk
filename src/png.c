/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:10:32 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/12 22:05:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP_PNG.h"

Uint32	png4byte(char *ptr)
{
	Uint32	result;
	int		i;

	i = 0;
	result = 0;
	while (i < 4)
	{
		result += (unsigned char)ptr[i]; //bitshift, this isn't correct
		i++;
		//printf("png4byte result %i \n", result);
	}
	
	return (result);
}
 //Figure out palette lenght!
Uint8	*readpalette(t_pngdata *png, Uint8 *ptr)
{
	int	i;
	png->palette.plte = ft_memalloc(sizeof(Uint64) * 1024); //TODO: calculate actual lenght for malloc.
	while (ft_strncmp(ptr, "PLTE", 4) != 0)
		ptr++;
	ptr += 4;
	i = 0;
	while (ft_strncmp(ptr++, "IDAT", 4) != 0 && ft_strncmp(ptr, "tIME", 4) != 0)
	{
		png->palette.plte[i] = *(Uint64 *)ptr;
		printf("color %li %li %li\n", png->palette.plte[i] & 0xFF, (png->palette.plte[i] >> 8) & 0xFF
										, (png->palette.plte[i] >> 16) & 0xFF);
		i++;
		//ptr++;
	}
	printf("palette len %i \n", i);
	return (ptr);
}

void	readdat(t_pngdata *png, Uint8 *ptr)
{
	ptr += 4;
	int count = 0;
	while (ft_strncmp(ptr++, "IEND", 4) != 0)
	{
		count++;
	}
	printf("found %i pixels in image \n", count * 8);
}

void	pngparse(void)
{
	int			fd;
	int			len;
	Uint8		*ptr;
	Uint8		buf[8192];
	t_pngdata	png;

	fd = sp_fileopen("grass.png", O_RDONLY);
	len = read(fd, buf, sizeof(Uint8) * 8192);
	//error_exit("png file didn't fit in static buffer");
	ptr = buf;
	while (ft_strcmp(++ptr, "IHDR") != 0 && len > 0) //Extract function?
		len--;
	printf("string: %s length left: %i\n", ptr, len);
	ptr += 4; //Skip IHDR
	png.size[X] = png4byte(ptr);
	png.size[Y] = png4byte(ptr+4);
	ptr += 8;
	png.bitdepth = *(ptr);
	png.colortype = *(ptr + 1);
	printf("compression yes/no %i \n", *(ptr + 4));
	printf("width = %i \n", png.size[X]);
	printf("height = %i \n", png.size[Y]);
	printf("bitdepth = %i \n", png.bitdepth);
	printf("colortype = %i \n", png.colortype);
	//fillpalette(png, &ptr, &buf, len);
	ptr = readpalette(&png, ptr);
	readdat(&png, ptr);
}
