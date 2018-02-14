/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 17:36:04 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/13 21:56:47 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>
#define TRANS tmp->transparent
#define TYPE obj->type

t_rgb	lighting(t_obj *obj, t_ray *intersect, t_vect light, double shadow)
{
	t_vect	light_dir;
	t_vect	h;
	double	cos_a;
	double	transp;

	light_dir = normalize(vdiff(light, intersect->origin));
	h = vdiv(vdiff(light_dir, intersect->dir),
			vlen(vdiff(light_dir, intersect->dir)));
	cos_a = fabs(vdot(light_dir, obj->norm));
	transp = 1;
	if (shadow < 1.0 && shadow > 0.0)
		if ((transp = (1.0 - shadow)) > 1.0)
			transp = 1;
	if (shadow == 0)
	{
		return (cadd(cscalar(obj->clr, (obj->amb * transp) + obj->diff
	* cos_a), cscalar((t_rgb){100, 100, 100}, obj->spec * pow(vdot(obj->norm
	, h), obj->m))));
	}
	if (shadow <= 0.9)
	{
		return (cscalar(obj->clr, (obj->amb * transp) > 1 ? 1 :
												(obj->amb * transp)));
	}
	return (cscalar(obj->clr, obj->amb));
}

t_rgb	addlight(t_rt *rt, t_ray *inter, t_obj *obj, t_vect light)
{
	t_obj	*tmp;
	t_ray	shadow;
	t_dist	d;
	double	*intersects;

	obj->norm = obj->normal(obj->u, inter->origin);
	shadow.origin = vadd(inter->origin, vmult(obj->norm, TYPE == 2 ? 0 : 1e-4));
	d.dist = vdiff(light, shadow.origin);
	shadow.dir = normalize(d.dist);
	d.dist_mag = sqrt(pow(d.dist.x, 2) + pow(d.dist.y, 2) + pow(d.dist.z, 2));
	intersects = (double*)ft_memalloc(sizeof(double) * rt->nodes);
	d.i = -1;
	tmp = rt->obj;
	while (++d.i < rt->nodes)
	{
		(d.i != 0) ? tmp = tmp->next : 0;
		intersects[d.i] = tmp->inter(shadow, tmp->u);
		if (intersects[d.i] >= EPS && intersects[d.i] <= d.dist_mag - 1e-4)
		{
			ft_memdel((void**)&intersects);
			return (lighting(obj, inter, light, TRANS ? tmp->io_trans : 1));
		}
	}
	ft_memdel((void**)&intersects);
	return (lighting(obj, inter, light, 0));
}
