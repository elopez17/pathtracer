/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elopez <elopez@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 13:32:37 by elopez            #+#    #+#             */
/*   Updated: 2018/02/18 20:30:58 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

int		expose_hook(t_rt **rt)
{
	draw(*rt);
	return (0);
}

int		mousepress(int key, int x, int y, t_rt **rt)
{
	t_xy	pixel;
	t_ray	ray;
	t_ray	intersect;
	int		index;

	if (key != 1)
		return (0);
	pixel = (t_xy){(double)x, (double)y};
	set_ray_xy(*rt, &ray, &pixel);
	if ((index = findintersect(&intersect, ray, *rt)) != -1)
		(*rt)->current = (*rt)->a_obj[index];
	return (0);
}

int		(*g_filter_ptr[]) (char *iimage) =
{
	&greyscale_filter,
	&natural_greyscale_filter,
	&whitescale_filter,
	&whitegreyscale_filter,
	&natural_whitegreyscale_filter,
	&inv_filt,
	&filter_two,
	&filter_six,
	&filter_seven,
	NULL,
};

void	swap_filter(char *image)
{
	static int i = 0;

	if (g_filter_ptr[i])
		(*g_filter_ptr[i++])(image);
	else
		i = 0;
}

int		key_hook(int key, t_rt **rt)
{
	t_obj *tmp;

/*	(key == KTOP1) ? addsphere(rt) : 0;
	(key == KTOP2) ? addplane(rt) : 0;
	(key == KTOP3) ? addcone(rt) : 0;
	(key == KTOP4) ? addcylin(rt) : 0;
	(key == KTOP5) ? addcube(rt) : 0;*/
	(key == KEY5) ? (*rt)->toggle ^= 1 : 0;
	if (key == KEYESC)
	{
		while ((*rt)->obj != NULL)
		{
			tmp = (*rt)->obj->next;
			ft_memdel((void**)&(*rt)->obj);
			(*rt)->obj = tmp;
		}
		ft_memdel((void**)&(*rt)->image);
		rt_memdel(rt);
		exit(0);
	}
	else if ((*rt)->current != NULL)
		move_obj(key, &(*rt)->current, (*rt)->toggle);
	scene(*rt);
	(key == KEYENT) ? swap_filter((*rt)->addr) : 0;
	draw(*rt);
	return (0);
}

int		close_hook(t_rt **rt)
{
	key_hook(KEYESC, rt);
	return (0);
}
