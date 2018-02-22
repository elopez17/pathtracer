/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/13 01:37:58 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/15 13:21:42 by eLopez           ###   ########.fr       */
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
	if (key == KEYLEFT && ((*obj)->diff = 1))
	{
		(*obj)->refract = 0;
		(*obj)->spec = 0;
		return ;
	}
	else if ((*obj)->diff && ((*obj)->diff = 0) == 0)
		(*obj)->refract = 1;
	else if ((*obj)->refract && ((*obj)->refract = 0) == 0)
		(*obj)->spec = 1;
}

void		move_obj(int key, t_obj **obj, int toggle)
{
	(key == KEYRIGHT || key == KEYLEFT) ? changestate(obj, key) : 0;
	if ((*obj)->type == 1)
		mod_sphere(key, obj);
	else if ((*obj)->type == 2)
		mod_plane(key, obj);
	else if ((*obj)->type == 3)
		mod_cone(key, obj, toggle);
	else if ((*obj)->type == 4)
		mod_cylind(key, obj, toggle);
	((*obj)->type == 5) ? mod_cube(key, obj, toggle) : 0;
}
