/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 18:47:13 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/20 16:48:09 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

static inline t_vect	hemisphere(double u1, double u2)
{
//	ft_printf("%lf %lf\n", u1, u2);
	const double r = sqrt(1.0 - u1*u1);
	const double phi = 2.0 * (double)PI * u2;
	return ((t_vect){cos(phi)*r, sin(phi)*r, u1});
}

void	ons(const t_vect n, t_vect *v2, t_vect *v3)
{
	double	invLen;
	double	denom;

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
	*v3 = vcross(n, *v2);
//	*v3 = vcross(*v2, n);
/*	v3->x = (v2->x == 0.0) ? v2->x : fmod(n.x, v2->x);
	v3->y = (v2->y == 0.0) ? v2->y : fmod(n.y, v2->y);
	v3->z = (v2->z == 0.0) ? v2->z : fmod(n.z, v2->z);*/
}

static void	trace(t_ray *intersect, t_rgb *color, t_ray ray, t_rt *rt, int depth)
{
	int		index;
	t_obj	*obj;
	t_rgb	clr2 = (t_rgb){1, 1, 1};
	double	rrFactor = 1.0;
// Russian roulette: starting at depth 5, each recursive step will stop with a probability of 0.1
	
	if (depth >= 5) {
		return ;
		const double rrStopProbability = 0.1;
		if (RND2 <= rrStopProbability)
			return ;
		rrFactor = 1.0 / (1.0 - rrStopProbability);
	}
	if ((index = findintersect(intersect, ray, rt)) == -1)
		return ;
	obj = rt->a_obj[index];
// Travel the ray to the hit point where the closest object lies and compute the surface
	//normal there.
	obj->norm = obj->normal(obj->u, intersect->origin);
	if (vdot(obj->norm, ray.dir) < 0.0)
		obj->norm = invert(obj->norm);
	ray.origin = intersect->origin;
// Add the emission, the L_e(x,w) part of the rendering equation, but scale it with the Russian Roulette
// probability weight.
	*color = cadd(*color, cscalar(obj->emission, rrFactor));

	// Diffuse BRDF - choose an outgoing direction with hemisphere sampling.
	if (obj->diff)
	{
		t_vect rotX, rotY;
		ons(obj->norm, &rotX, &rotY);
		t_vect sampledDir = hemisphere(RND2, RND2);
		t_vect rotatedDir;
		rotatedDir.x = vdot((t_vect){rotX.x, rotY.x, obj->norm.x}, sampledDir);
		rotatedDir.y = vdot((t_vect){rotX.y, rotY.y, obj->norm.y}, sampledDir);
		rotatedDir.z = vdot((t_vect){rotX.z, rotY.z, obj->norm.z}, sampledDir);
		ray.dir = rotatedDir;	// already normalized
	//	ray.dir = normalize(vadd(obj->norm, sampledDir));
		double cost = vdot(ray.dir, obj->norm);
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(cmult(clr2, obj->clr), cost * 0.1 * rrFactor));
	}
	// Specular BRDF - this is a singularity in the rendering equation that follows
	// delta distribution, therefore we handle this case explicitly - one incoming
	// direction -> one outgoing direction, that is, the perfect reflection direction.
	if (obj->spec)
	{
		double cost = vdot(ray.dir, obj->norm);
		ray.dir = normalize(vdiff(ray.dir, vmult(obj->norm, cost * 2)));
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(clr2, rrFactor));
	}

	// Glass/refractive BRDF - we use the vector version of Snell's law and Fresnel's law
	// to compute the outgoing reflection and refraction directions and probability weights.
	if (obj->refract)
	{
		double n = 1.5;
		double R0 = (1.0 - n) / (1.0 + n);
		R0 = R0*R0;
		if (vdot(obj->norm, ray.dir) > 0)
		{	// we're inside the medium
			obj->norm = invert(obj->norm);
			n = 1.0 / n;
		}
		n = 1.0 / n;
		double cost1 = vdot(obj->norm, ray.dir) * -1.0; // cosine of theta_1
		double cost2 = 1.0 - n*n*(1.0 - cost1*cost1); // cosine of theta_2
		double Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation
		if (cost2 > 0 && RND2 > Rprob) { // refraction direction
			ray.dir = normalize(vadd(vmult(ray.dir, n), vmult(obj->norm, n*cost1-sqrt(cost2))));
		} else { // reflection direction
			ray.dir = normalize(vadd(ray.dir, vmult(obj->norm, cost1 * 2)));
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
				i = (int)(pixel.x + (pixel.y * rt->w.width));
				rt->image[i] = cadd(rt->image[i], color);
			}
		}
	}
	save_img(rt);
}
