/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/13 01:37:58 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/13 21:57:21 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

static void	mod_sphere(int key, t_obj **obj)
{
	key == KEYUP ? (*obj)->u.sphere.radius += 0.3 : 0;
	key == KEYDOWN ? (*obj)->u.sphere.radius -= 0.3 : 0;
	key == KEY7 ? (*obj)->u.sphere.pos.x -= 2 : 0;
	if (key == KEY9)
		(*obj)->u.sphere.pos.x += 2;
	else if (key == KEY4)
		(*obj)->u.sphere.pos.y += 2;
	else if (key == KEY6)
		(*obj)->u.sphere.pos.y -= 2;
	else if (key == KEY1)
		(*obj)->u.sphere.pos.z -= 2;
	else if (key == KEY3)
		(*obj)->u.sphere.pos.z += 2;
	else if (key == KEYR)
		(*obj)->clr.red += 20;
	else if (key == KEYE)
		(*obj)->clr.red -= 20;
	else if (key == KEYG)
		(*obj)->clr.green += 20;
	else if (key == KEYF)
		(*obj)->clr.green -= 20;
	else if (key == KEYB)
		(*obj)->clr.blue += 20;
	else if (key == KEYV)
		(*obj)->clr.blue -= 20;
}

static void	mod_plane(int key, t_obj **obj)
{
	if (key == KEY8)
		++(*obj)->u.plane.dist;
	else if (key == KEY2)
		--(*obj)->u.plane.dist;
	else if (key == KEYR)
		(*obj)->clr.red += 20;
	else if (key == KEYE)
		(*obj)->clr.red -= 20;
	else if (key == KEYG)
		(*obj)->clr.green += 20;
	else if (key == KEYF)
		(*obj)->clr.green -= 20;
	else if (key == KEYB)
		(*obj)->clr.blue += 20;
	else if (key == KEYV)
		(*obj)->clr.blue -= 20;
}

static void	changestate(t_obj **obj, int key)
{
	if (key == KEYLEFT && ((*obj)->reflect = 0) == 0)
	{
		(*obj)->refract = 0;
		(*obj)->transparent = 0;
		return ;
	}
	if (!(*obj)->reflect && !(*obj)->refract && !(*obj)->transparent)
		(*obj)->reflect = 1;
	else if ((*obj)->reflect && ((*obj)->reflect = 0) == 0)
		(*obj)->transparent = 1;
	else if ((*obj)->transparent && ((*obj)->transparent = 0) == 0)
		(*obj)->refract = 1;
	else if ((*obj)->refract && ((*obj)->refract = 0) == 0)
		(*obj)->reflect = 1;
}

static void	changeindex(int key, t_obj **obj)
{
	(key == KEY0 && (*obj)->transparent && (*obj)->io_trans < 0.9) ?
		(*obj)->io_trans += 0.1 : 0;
	(key == KEYDOT && (*obj)->transparent && (*obj)->io_trans > 0.1) ?
		(*obj)->io_trans -= 0.1 : 0;
	(key == KEY0 && (*obj)->reflect && (*obj)->io_refl < 1.0) ?
		(*obj)->io_refl += 0.1 : 0;
	(key == KEYDOT && (*obj)->reflect && (*obj)->io_refl > 0.0) ?
		(*obj)->io_refl -= 0.1 : 0;
	(key == KEY0 && (*obj)->refract && (*obj)->ior >= 1.1) ?
		(*obj)->ior -= 0.1 : 0;
	(key == KEYDOT && (*obj)->refract && (*obj)->ior <= 1.9) ?
		(*obj)->ior += 0.1 : 0;
}

void		move_obj(int key, t_obj **obj, int toggle)
{
	(key == KEY0 || key == KEYDOT) ? changeindex(key, obj) : 0;
	(key == KEYRIGHT || key == KEYLEFT) ? changestate(obj, key) : 0;
	if (key == KEYPLUS || key == KEYMIN)
		(*obj)->amb += (key == KEYPLUS) ? 0.03 : -0.03;
	if (key == KEYS && (*obj)->spec < 1.0f)
		(*obj)->spec += 0.3;
	else if (key == KEYX && (*obj)->spec > 0.0f)
		(*obj)->spec -= 0.3;
	else if (key == KEYD && (*obj)->diff < 1.0f)
		(*obj)->diff += 0.3;
	else if (key == KEYC && (*obj)->diff > 0.0f)
		(*obj)->diff -= 0.3;
	else if (key == KEYN && (*obj)->m > 2)
		(*obj)->m -= 3;
	else if (key == KEYM && ((*obj)->m < 20))
		(*obj)->m += 3;
	else if ((*obj)->type == 1)
		mod_sphere(key, obj);
	else if ((*obj)->type == 2)
		mod_plane(key, obj);
	else if ((*obj)->type == 3)
		mod_cone(key, obj, toggle);
	else if ((*obj)->type == 4)
		mod_cylind(key, obj, toggle);
	((*obj)->type == 5) ? mod_cube(key, obj, toggle) : 0;
}
