/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:10:32 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/26 17:23:56 by okinnune         ###   ########.fr       */
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
	ptr += 4; // should be 4
	i = 0;
	while (ft_strncmp(ptr, "bKGD", 4) != 0 && ft_strncmp(ptr, "tIME", 4) != 0)
	{
		//png->palette.plte[i] = *ptr + (*(ptr + 1) << 8) + (*(ptr + 2) << 16);
		png->palette.plte[i] = (*ptr << 16) + (*(ptr + 1) << 8) + (*(ptr + 2));
		//png->palette.plte[i] = *(ptr + 2);
		ptr += 3;
		i++;
	}
	png->palette.length = i;
	printf("pal len %i \n", png->palette.length);
	return (ptr);
}

void	readdat(t_pngdata *png, Uint8 *ptr)
{
	while (ft_strncmp(ptr, "IDAT", 4) != 0)
		ptr++;
	ptr += 12; // should be 4??
	//printf("string start %s \n", ptr);
	int count = 0;
	png->data = ft_memalloc(sizeof(Uint8) * png->size[X] * png->size[Y]);
	//protec

	//ft_strncmp(ptr++, "IEND", 4) != 0 &&
	while (ft_strncmp(ptr, "tEXt", 4) != 0
		&& count < png->size[X] * png->size[Y])
	{
		png->data[count] = *ptr;
		ptr++;
		count++;
	}
	printf("found %i pixels in image \n", count);
	/*printf("first pixel %i second pixel %i \n", png->data[0], png->data[1]);*/
}

void	pngparse(t_pngdata	*png, char *filename)
{
	int			fd;
	int			len;
	Uint8		*ptr;
	Uint8		buf[32000];
	//t_pngdata	png;

	fd = sp_fileopen(filename, O_RDONLY);
	//fd = sp_fileopen("exdc.png", O_RDONLY);
	len = read(fd, buf, sizeof(Uint8) * 32000);
	//error_exit("png file didn't fit in static buffer");
	ft_bzero(png, sizeof(t_pngdata));
	ptr = buf;
	while (ft_strcmp(++ptr, "IHDR") != 0 && len > 0) //Extract function?
		len--;
	//printf("string: %s length left: %i\n", ptr, len);
	ptr += 4; //Skip IHDR
	png->size[X] = png4byte(ptr);
	png->size[Y] = png4byte(ptr+4);
	ptr += 8;
	png->bitdepth = *(ptr);
	png->colortype = *(ptr + 1);
	printf("compression yes/no %i \n", *(ptr + 4));
	printf("width = %i \n", png->size[X]);
	printf("height = %i \n", png->size[Y]);
	printf("bitdepth = %i \n", png->bitdepth);
	printf("colortype = %i \n", png->colortype);
	//fillpalette(png, &ptr, &buf, len);
	ptr = readpalette(png, ptr);
	readdat(png, ptr);
	close(fd);
}
