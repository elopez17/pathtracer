/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdalha <oabdalha@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 13:32:37 by elopez            #+#    #+#             */
/*   Updated: 2018/02/13 21:59:06 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

inline t_vect	vmult(const t_vect v, const float scalar)
{
	return ((t_vect){v.x * scalar, v.y * scalar, v.z * scalar});
}

inline t_vect	vdiv(const t_vect v, float denominator)
{
	if (denominator == 0)
		denominator = 1;
	return ((t_vect){v.x / denominator, v.y / denominator, v.z / denominator});
}

inline t_vect	vdiff(const t_vect v1, const t_vect v2)
{
	return ((t_vect){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
}

inline float	vsqr(const t_vect v)
{
	return (vdot(v, v));
}

inline float	vlen(const t_vect v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}
