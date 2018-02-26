/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eLopez <elopez@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 18:47:13 by eLopez            #+#    #+#             */
/*   Updated: 2018/02/25 20:41:21 by eLopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pt.h>

static inline t_vect	hemisphere(float u1, float u2)
{
//	ft_printf("%lf %lf\n", u1, u2);
	const float r = sqrt(1.0 - u1*u1);
	const float phi = 2.0 * (float)PI * u2;
	return ((t_vect){cos(phi)*r, sin(phi)*r, u1});
}

void	ons(const t_vect n, t_vect *v2, t_vect *v3)
{
	float	invLen;
	float	denom;

	if (fabsf(n.x) > fabsf(n.y)) {
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
}

static float	sample_light(t_rt *rt, t_ray ray, const t_vect normal)
{
	t_ray	shadow;
	t_vect	light_dist;
	float	dist_mag;
	float	intersects[rt->nodes];
	t_obj	*obj;
	int		in_shadow = 0;
	t_union	u;

	u.sphere.pos = rt->light[0];
	u.sphere.radius = .5;
	u.sphere.clr = (t_rgb){1, 1, 1};
	shadow.origin = ray.origin;
	t_vect tmp = (t_vect){rt->light[0].x + (RND2 - 0.5),
		rt->light[0].y + (RND2 - 0.5), rt->light[0].z + (RND2 - 0.5)};
	shadow.dir = normalize(vdiff(tmp, shadow.origin));
	if ((dist_mag = findintersphere(shadow, u)) == -1)
		return (0);
	for (int i = 0; i < rt->nodes; i++)
	{
		obj = rt->a_obj[i];
		intersects[i] = obj->inter(shadow, obj->u);
		if (intersects[i] >= EPS && intersects[i] < dist_mag)
		{
			in_shadow = 1;
			break ;
		}
	}
	if (in_shadow)
		return (0);
	float	cost = vdot(normal, normalize(vdiff(rt->light[0], shadow.origin)));
	return (cost < 0.0 ? 0 : cost);
}

static void	trace(t_ray *intersect, t_rgb *color, t_ray ray, t_rt *rt, int depth)
{
	int		index;
	t_obj	*obj;
	t_rgb	clr2 = (t_rgb){0, 0, 0};
	float	rrFactor = 1.0;
	float	cos_L;
// Russian roulette: starting at depth 5, each recursive step will stop with a probability of 0.1
	
	if (depth >= 5) {
		return ;
		const float rrStopProbability = 0.1;
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
	if (vdot(obj->norm, ray.dir) > 0.0)
		obj->norm = invert(obj->norm);
	ray.origin = intersect->origin;
// Add the emission, the L_e(x,w) part of the rendering equation, but scale it with the Russian Roulette
// probility weight.
	cos_L = sample_light(rt, ray, obj->norm);
	*color = cadd(*color, obj->emission);
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
		float cost = vdot(ray.dir, obj->norm);
		trace(intersect, &clr2, ray, rt, depth + 1);
	//	*color = cadd(*color, cscalar(cadd(obj->clr, clr2), (double)cost));
	//	*color = cadd(*color, cadd(clr2, cscalar(obj->clr, (double)cost)));
		*color = cadd(*color, cadd(cscalar(obj->clr, (double)cos_L), cscalar(clr2, (double)cost)));
	//	*color = cadd(*color, cscalar(clr2, (double)cost));
	//	*color = cadd(*color, cadd(obj->clr, cscalar(clr2, (double)cost)));
//		*color = cadd(*color, cmult(clr2, cscalar(obj->clr, (double)cost)));
//		*color = cadd(cscalar(cadd(clr2, cscalar(obj->clr, (double)cost)), 0.5), obj->emission);
//		*color = cadd(cmult(clr2, cscalar(obj->clr, (double)cost)), obj->emission);
	//	*color = cmult(clr2, cscalar(obj->clr, (double)cost));
//		*color = cscalar(cmult(clr2, obj->clr), (double)cost);
//		*color = cscalar(cadd(*color, cmult(clr2, obj->clr)), (double)cost);
//		*color = cadd(*color, cscalar(cmult(clr2, obj->clr), (double)cost));
	}
	// Specular BRDF - this is a singularity in the rendering equation that follows
	// delta distribution, therefore we handle this case explicitly - one incoming
	// direction -> one outgoing direction, that is, the perfect reflection direction.
	if (obj->spec)
	{
		float cost = vdot(ray.dir, obj->norm);
		ray.dir = normalize(vdiff(ray.dir, vmult(obj->norm, cost * 2)));
		trace(intersect, &clr2, ray, rt, depth + 1);
		*color = cadd(*color, cscalar(clr2, rrFactor));
	}

	// Glass/refractive BRDF - we use the vector version of Snell's law and Fresnel's law
	// to compute the outgoing reflection and refraction directions and probability weights.
	if (obj->refract)
	{
		float n = 1.5;
		float R0 = (1.0 - n) / (1.0 + n);
		R0 = R0*R0;
		if (vdot(obj->norm, ray.dir) > 0)
		{	// we're inside the medium
			obj->norm = invert(obj->norm);
			n = 1.0 / n;
		}
		n = 1.0 / n;
		float cost1 = vdot(obj->norm, ray.dir) * -1.0; // cosine of theta_1
		float cost2 = 1.0 - n*n*(1.0 - cost1*cost1); // cosine of theta_2
		float Rprob = R0 + (1.0 - R0) * pow(1.0 - cost1, 5.0); // Schlick-approximation
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

	clock_t	begin = clock();
	srand(time(NULL));
	for (int iter = 0; iter < SPP; iter++)
	{
		pixel.y = -1;
		while (++pixel.y < rt->w.height)
		{
			pixel.x = -1;
			while (++pixel.x < rt->w.width)
			{
				color = (t_rgb){0, 0, 0};
				set_ray_xy(rt, &ray, &pixel);
				trace(&intersection, &color, ray, rt, 0);
				i = (int)(pixel.x + (pixel.y * rt->w.width));
				rt->image[i] = cadd(rt->image[i], color);
			}
		}
		if (((iter + 1) % 50) == 0)
			ft_printf("%d\n", iter + 1);
	}
	save_img(rt);
	clock_t	end = clock();
	float seconds = (float)(end - begin) / (float)CLOCKS_PER_SEC;
	ft_printf("%{RD} %.0lf%{nc} seconds\n%{RD} %.2lf%{nc} minutes\n", 
			seconds, (seconds/60.0));
}
