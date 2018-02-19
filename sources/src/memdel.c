/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memdel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evanheum <evanheum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/24 11:06:05 by evanheum          #+#    #+#             */
/*   Updated: 2018/02/18 19:50:32 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

void	rt_memdel(t_rt **rt)
{
	mlx_destroy_image((*rt)->mlx, (*rt)->img);
	mlx_destroy_window((*rt)->mlx, (*rt)->win);
	free((*rt)->a_obj);
//	mlx_destroy_window((*rt)->mlx, (*rt)->win2);
	ft_memdel((void**)&(*rt)->light);
	ft_memdel((void**)rt);
}
