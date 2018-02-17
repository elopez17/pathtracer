/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 18:47:13 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/17 12:26:17 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>
#define IRFL tmp->io_refl
#define ITRN tmp->io_trans

static int		g_count = -1;

t_vect	hemisphere(double u1, double u2)
{
	const double r = sqrt(1.0 - u1*u1);
	const double phi = 2 * PI * u2;
	return ((t_vect){cos(phi)*r, sin(phi)*r, u1});
}

void	ons(const t_vect n, t_vect *v2, t_vect *v3)
{
	double	invLen;
	double	denom;
	int		vector[3];
	int		vect1[3];
	int		vect2[3];

	if (fabs(n.x) > fabs(n.y)) {
// project to the y = 0 plane and construct a normalized orthogonal vector in this plane
		denom = sqrt(n.x * n.x + n.z * n.z);
		invLen = 1.0 / (denom == 0.0 ? EPS : denom);
		*v2 = (t_vect){-n.z * invLen, 0.0, n.x * invLen};
	}
	else {
// project to the x = 0 plane and construct a normalized orthogonal vector in this plane
		denom = sqrt(n.y * n.y + n.z * n.z);
		invLen = 1.0 / (denom == 0.0 ? EPS : denom);
		*v2 = (t_vect){0.0, n.z * invLen, -n.y * invLen};
	}
	vect1[0] = n.x;
	vect1[1] = n.y;
	vect1[2] = n.z;
	vect2[0] = v2->x;
	vect2[1] = v2->y;
	vect2[2] = v2->z;
	vector[0] = (vect2[0] == 0) ? vect1[0] : vect1[0] % vect2[0];
	vector[1] = (vect2[1] == 0) ? vect1[1] : vect1[1] % vect2[1];
	vector[2] = (vect2[2] == 0) ? vect1[2] : vect1[2] % vect2[2];
	*v3 = (t_vect){vector[0], vector[1], vector[2]};
}

static void	trace(t_ray *intersect, t_rgb *color, t_ray ray, t_rt *rt, int depth)
{
	int		index;
	t_obj	*tmp;
	t_rgb	clr2 = (t_rgb){1, 1, 1};
	double	rrFactor = 1.0;
// Russian roulette: starting at depth 5, each recursive step will stop with a probability of 0.1
	
	if (depth >= 5) {
		const double rrStopProbability = 0.1;
		if (RND2 <= rrStopProbability)
			return ;
		rrFactor = 1.0 / (1.0 - rrStopProbability);
	}
	index = findintersect(intersect, ray, rt);
	if (index == -1)
		return ;
	tmp = rt->obj;
	while (--index >= 0)
		tmp = tmp->next;
// Travel the ray to the hit point where the closest object lies and compute the surface
	//normal there.
//vec hp == intersect->origin
//vec N == tmp->norm = tmp->normal(tmp->u, intersect->origin);
	tmp->norm = tmp->normal(tmp->u, intersect->origin);
	ray.origin = intersect->origin;
// Add the emission, the L_e(x,w) part of the rendering equation, but scale it with the Russian Roulette
// probability weight.
	*color = cadd(*color, cscalar(tmp->emission, rrFactor));

	// Diffuse BRDF - choose an outgoing direction with hemisphere sampling.
	if (tmp->diff)
	{
		t_vect rotX, rotY;
		ons(tmp->norm, &rotX, &rotY);
		t_vect sampledDir = hemisphere(RND2, RND2);
		t_vect rotatedDir;
		rotatedDir.x = vdot((t_vect){rotX.x, rotY.x, tmp->norm.x}, sampledDir);
		rotatedDir.y = vdot((t_vect){rotX.y, rotY.y, tmp->norm.y}, sampledDir);
		rotatedDir.z = vdot((t_vect){rotX.z, rotY.z, tmp->norm.z}, sampledDir);
		ray.dir = rotatedDir;	// already normalized
		double cost = vdot(ray.dir, tmp->norm);
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(cmult(clr2, tmp->clr), cost * 0.1 * rrFactor));
	}
	// Specular BRDF - this is a singularity in the rendering equation that follows
	// delta distribution, therefore we handle this case explicitly - one incoming
	// direction -> one outgoing direction, that is, the perfect reflection direction.
	if (tmp->spec)
	{
		double cost = vdot(ray.dir, tmp->norm);
		ray.dir = normalize(vdiff(ray.dir, vmult(tmp->norm, cost * 2)));
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(clr2, rrFactor));
	}

	// Glass/refractive BRDF - we use the vector version of Snell's law and Fresnel's law
	// to compute the outgoing reflection and refraction directions and probability weights.
	if (tmp->refract)
	{
		double n = 1.5;
		double R0 = (1.0 - n) / (1.0 + n);
		R0 = R0*R0;
		if (vdot(tmp->norm, ray.dir) > 0)
		{	// we're inside the medium
			tmp->norm = invert(tmp->norm);
			n = 1.0 / n;
		}
		n = 1.0 / (n == 0 ? EPS : n);
		double cost1 = vdot(tmp->norm, ray.dir) * -1.0; // cosine of theta_1
		double cost2 = 1.0 - n*n*(1.0 - cost1*cost1); // cosine of theta_2
		double Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation
		if (cost2 > 0 && RND2 > Rprob) { // refraction direction
			ray.dir = normalize(vadd(vmult(ray.dir, n), vmult(tmp->norm, n*cost1-sqrt(cost2))));
		} else { // reflection direction
			ray.dir = normalize(vadd(ray.dir, vmult(tmp->norm, cost1 * 2)));
		}
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(clr2, 1.15 * rrFactor));
	}
}

inline void		set_ray_xy(t_rt *rt, t_ray *ray, t_xy *pixel)
{
	t_xy		dir;

	dir.x = (pixel->x / rt->w.width) + (RND / 700);
	dir.y = ((rt->w.height - pixel->y) / rt->w.height) + (RND / 700);
	ray->origin = rt->cam.pos;
	ray->dir = normalize(vadd(rt->cam.dir, vadd(vmult(rt->cam.right,
					dir.x - 0.5), vmult(rt->cam.down, dir.y - 0.5))));
}

void			scene(t_rt *rt)
{
	t_xy		pixel;
	t_ray		ray;
	t_ray		intersection;
	int			index;
	int			i;
	t_rgb		color;

	srand(time(NULL));
	for (int iter = 0; iter < SPP; iter++)
	{
		pixel.y = -1;
		while (++pixel.y < rt->w.height)
		{
			pixel.x = -1;
			while (++pixel.x < rt->w.width)
			{
				color = (t_rgb){1, 1, 1};
				set_ray_xy(rt, &ray, &pixel);
				trace(&intersection, &color, ray, rt, 0);
		//		ft_printf("%.3lf  %.3lf  %.3lf\n", color.red,
		//				color.blue, color.green);
				i = (int)(pixel.x + (pixel.y * rt->w.width));
				rt->image[i] = cadd(rt->image[i], color);
		//		ft_printf("%.3lf  %.3lf  %.3lf\n", rt->image[i].red,
		//				rt->image[i].blue, rt->image[i].green);
			}
		}
	}
	save_img(rt);
}
