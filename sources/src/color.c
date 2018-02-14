/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdalha <oabdalha@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 13:32:37 by elopez            #+#    #+#             */
/*   Updated: 2018/02/13 21:54:43 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

inline t_rgb	cscalar(t_rgb color, double scalar)
{
	return ((t_rgb){color.red * scalar,
			color.green * scalar,
			color.blue * scalar});
}

inline t_rgb	cadd(t_rgb clr1, t_rgb clr2)
{
	return ((t_rgb){clr1.red + clr2.red,
			clr1.green + clr2.green,
			clr1.blue + clr2.blue});
}

inline t_rgb	cmult(t_rgb clr1, t_rgb clr2)
{
	return ((t_rgb){clr1.red * clr2.red,
			clr1.green * clr2.green,
			clr1.blue * clr2.blue});
}

inline t_rgb	cavg(t_rgb clr1, t_rgb clr2)
{
	return ((t_rgb){(clr1.red + clr2.red) / 2,
			(clr1.green + clr2.green) / 2,
			(clr1.blue + clr2.blue) / 2});
}
