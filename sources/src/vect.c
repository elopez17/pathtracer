/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdalha <oabdalha@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 13:32:37 by elopez            #+#    #+#             */
/*   Updated: 2018/02/13 21:58:55 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

inline t_vect	normalize(const t_vect v)
{
	float	magnitude;

	magnitude = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	if (magnitude == 0)
		magnitude = 1;
	return ((t_vect){v.x / magnitude, v.y / magnitude, v.z / magnitude});
}

inline t_vect	invert(const t_vect v)
{
	return ((t_vect){-v.x, -v.y, -v.z});
}

inline float	vdot(const t_vect v1, const t_vect v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline t_vect	vcross(const t_vect v1, const t_vect v2)
{
	return ((t_vect){v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x});
}

inline t_vect	vadd(const t_vect v1, const t_vect v2)
{
	return ((t_vect){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
}
