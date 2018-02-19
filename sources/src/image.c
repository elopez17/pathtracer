#include <pt.h>

void	save_img(t_rt *rt)
{
	double	Lw = 0.0;
	double	scale = 1.0 / (double)SPP;
	int		i;
	double	this_lw;

	for (int y = 0; y < rt->w.height; y++)
		for (int x = 0; x < rt->w.width; x++)
		{
			i = x + (y * rt->w.width);
			rt->image[i] = cscalar(rt->image[i], scale);
//this_lw = log(0.1 + 0.2126 * rt->image[i].red + 0.7152 * rt->image[i].green + 0.0722 * rt->image[i].blue);
			this_lw = log(0.1 + 0.2126 * rt->image[i].red + 0.7152 * rt->image[i].green + 0.0722 * rt->image[i].blue);
			if (this_lw == this_lw)
				Lw += this_lw;
		}
	Lw /= (double)(rt->w.height * rt->w.width);
	Lw = exp(Lw);
	for (int y = 0; y < rt->w.height; y++)
		for (int x = 0; x < rt->w.width; x++)
		{
			i = x + (y * rt->w.width);
			rt->image[i].red = rt->image[i].red * 0.36 / Lw;
			rt->image[i].green = rt->image[i].green * 0.36 / Lw;
			rt->image[i].blue = rt->image[i].blue * 0.36 / Lw;

			rt->image[i].red = rt->image[i].red / (rt->image[i].red + 1.0);
			rt->image[i].green = rt->image[i].green / (rt->image[i].green + 1.0);
			rt->image[i].blue = rt->image[i].blue / (rt->image[i].blue + 1.0);
			rt->image[i] = cscalar(rt->image[i], 250.0);
			putpixel(rt, x, y, rt->image[i]);
		}
}
