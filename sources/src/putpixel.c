/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putpixel.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 18:33:42 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/13 22:02:16 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

inline void	putpixel(t_rt *rt, int x, int y, t_rgb color)
{
	int i;

	i = (rt->bpp / 8) * x + rt->len * y;
	if (color.red < 0)
		color.red = 0;
	if (color.blue < 0)
		color.blue = 0;
	if (color.green < 0)
		color.green = 0;
	rt->addr[i++] = (int)((color.blue > 255) ? 255 : color.blue);
	rt->addr[i++] = (int)((color.green > 255) ? 255 : color.green);
	rt->addr[i++] = (int)((color.red > 255) ? 255 : color.red);
}
