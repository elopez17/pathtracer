#include <pt.h>

void	save_img(t_rt *rt)
{
	double	Lw = 0.0;
	double	scale = 1.0 / 50.0;
	int		i;
	double	this_lw;

	for (int y = 0; y < rt->w.height; y++)
		for (int x = 0; x < rt->w.width; x++)
		{
			i = x + (y * rt->w.width);
	//		ft_printf("%lf\n%lf\n%lf\n", rt->image[i].red, rt->image[i].green, 
	//				rt->image[i].blue);
			rt->image[i] = cscalar(rt->image[i], scale);
	//		ft_printf("%lf %lf %lf\n", rt->image[i].red, rt->image[i].green, 
	//				rt->image[i].blue);
//			this_lw = log(0.1 + rt->image[i].red +  rt->image[i].green + rt->image[i].blue);
		//	this_lw = log(0.1 + 0.2126 * rt->image[i].red + 0.7152 * rt->image[i].green + 0.0722 * rt->image[i].blue);
//			if (this_lw == this_lw){
//				Lw += this_lw;
	//			ft_printf("OK\n");
//			}
//			else {
	//			ft_printf("NaN\n");
//			}
		}
//	Lw /= ((double)(rt->w.height * rt->w.width));
//	Lw = exp(Lw);
	for (int y = 0; y < rt->w.height; y++)
		for (int x = 0; x < rt->w.width; x++)
		{
			i = x + (y * rt->w.width);
/*			rt->image[i].red = rt->image[i].red / (Lw);// 0.36
			rt->image[i].green = rt->image[i].green / (Lw);
			rt->image[i].blue = rt->image[i].blue / (Lw);

	//		ft_printf("%.3lf\n%.3lf\n%.3lf\n", rt->image[i].red, rt->image[i].green, 
	//				rt->image[i].blue);
			rt->image[i].red = rt->image[i].red / (rt->image[i].red + 1.0);
			rt->image[i].green = rt->image[i].green / (rt->image[i].green + 1.0);
			rt->image[i].blue = rt->image[i].blue / (rt->image[i].blue + 1.0);
	//		ft_printf("%lf\n%lf\n%lf\n", rt->image[i].red, rt->image[i].green, 
	//				rt->image[i].blue);*/
		//	rt->image[i] = cscalar(rt->image[i], 256.0);
			putpixel(rt, x, y, cscalar(rt->image[i], 256.0));
		}
	draw(rt);
}
