#include <math.h>
#define PI 3.1415926535897932

// t = Time - 表示动画开始以来经过的时间。通常从0开始，通过游戏循环或update函数来缓慢增加。
// b = Beginning value - 动画的起点，默认从0开始。
// c = Change in value - 从起点到终点的差值。
// d = Duration - 完成动画所需的时间。   

// Linear
double Linear_easeIn(double t, double b, double c, double d)
{
	return c * t / d + b;
}
 
double Linear_easeOut(double t, double b, double c, double d)
{
	return c * t / d + b;
}
 
double Linear_easeInOut(double t, double b, double c, double d)
{
	return c * t / d + b;
}
 
// Quadratic
double Quad_easeIn(double t, double b, double c, double d)
{
	return c * (t /= d) * t + b;
}
 
double Quad_easeOut(double t, double b, double c, double d)
{
	return -c * (t /= d) * (t - 2) + b;
}
 
double Quad_easeInOut(double t, double b, double c, double d)
{
	if ((t /= d / 2) < 1)
		return c / 2 * t * t + b;
	return -c / 2 * ((--t) * (t - 2) - 1) + b;
}
 
// Cubic
double Cubic_easeIn(double t, double b, double c, double d)
{
	return c * (t /= d) * t * t + b;
}
 
double Cubic_easeOut(double t, double b, double c, double d)
{
	return c * ((t = t / d - 1) * t * t + 1) + b;
}
 
double Cubic_easeInOut(double t, double b, double c, double d)
{
	if ((t /= d / 2) < 1)
		return c / 2 * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t + 2) + b;
}
 
// Quartic
double Quart_easeIn(double t, double b, double c, double d)
{
	return c * (t /= d) * t * t * t + b;
}
 
double Quart_easeOut(double t, double b, double c, double d)
{
	return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}
 
double Quart_easeInOut(double t, double b, double c, double d)
{
	if ((t /= d / 2) < 1)
		return c / 2 * t * t * t * t + b;
	return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}
 
// Quintic
double Quint_easeIn(double t, double b, double c, double d)
{
	return c * (t /= d) * t * t * t * t + b;
}
 
double Quint_easeOut(double t, double b, double c, double d)
{
	return c * ((t = t / d - 1)* t * t * t * t + 1) + b;
}
 
double Quint_easeInOut(double t, double b, double c, double d)
{
	if ((t /= d / 2) < 1)
		return c / 2 * t * t * t * t * t + b;
	return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}
 
// Sinusoidal
double Sine_easeIn(double t, double b, double c, double d)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}
 
double Sine_easeOut(double t, double b, double c, double d)
{
	return c * sin(t / d * (PI / 2)) + b;
}
 
double Sine_easeInOut(double t, double b, double c, double d)
{
	return -c / 2 * (cos(PI * t/ d) - 1) + b;
}
 
// Exponential
double Expo_easeIn(double t, double b, double c, double d)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}
 
double Expo_easeOut(double t, double b, double c, double d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}
 
double Expo_easeInOut(double t, double b, double c, double d)
{
	if (t == 0)
		return b;
	if (t == d)
		return b+c;
	if ((t /= d / 2) < 1)
		return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}
 
// Circular
double Circ_easeIn(double t, double b, double c, double d)
{
	return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
}
 
double Circ_easeOut(double t, double b, double c, double d)
{
	return c * sqrt(1 - (t = t / d - 1) * t) + b;
}
 
double Circ_easeInOut(double t, double b, double c, double d)
{
	if ((t /= d / 2) < 1)
		return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	return c / 2 * (sqrt(1 - (t -= 2) * t) + 1) + b;
}
 
// Elastic
double Elastic_easeIn(double t, double b, double c, double d, double a/* = 0.0f*/, double p/* = 0.0f*/)
{
	double s = 0;
	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return b + c;
	if (!p)
		p = d * .3f;
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p/4;
	}
	else
		s = p / (2 * PI) * asin (c / a);
	return -(a * pow(2, 10 * (t -= 1)) * sin((t * d - s) * (2 * PI) / p)) + b;
}
 
double Elastic_easeOut(double t, double b, double c, double d, double a/* = 0.0f*/, double p/* = 0.0f*/)
{
	double s = 0;
	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return (b + c);
	if (!p)
		p = d *.3f;
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p / 4;
	}
	else
		s = p / (2 * PI) * sin((double)c / a);
 
	return (a * pow(2.0f, (int) - 10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b);
}
 
double Elastic_easeInOut(double t, double b, double c, double d, double a/* = 0.0f*/, double p/* = 0.0f*/)
{
	double s = 0;
	if (t == 0)
		return b;
	if ((t /= d / 2) == 2)
		return b + c;
	if (!p)
		p = d * (.3f * 1.5f);
	if (!a || a < fabsf(c))
	{
		a = c;
		s = p/4;
	}
	else
		s = p / (2 * PI) * asin (c / a);
	if (t < 1)
		return -.5f * (a * pow(2, 10 * (t -= 1)) * sin( (t * d - s) * (2 * PI) / p )) + b;
	return a * pow(2, -10 * (t -= 1)) * sin((t * d - s) * (2 * PI) / p ) * .5f + c + b;
}

// Bounce
double Bounce_easeOut(double t, double b, double c, double d)
{
	if ((t /= d) < (1 / 2.75))
	{
		return c * (7.5625f * t * t) + b;
	}
	else if (t < (2 / 2.75))
	{
		return c * (7.5625f * (t -= (1.5f / 2.75f)) * t + .75f) + b;
	}
	else if (t < (2.5 / 2.75))
	{
		return c * (7.5625f * (t -= (2.25f / 2.75f)) * t + .9375f) + b;
	}
	else
	{
		return c * (7.5625f * (t -= (2.625f / 2.75f)) * t + .984375f) + b;
	}
}
 
double Bounce_easeIn(double t, double b, double c, double d)
{
	return c - Bounce_easeOut(d - t, 0, c, d) + b;
}
 
double Bounce_easeInOut(double t, double b, double c, double d)
{
	if (t < d / 2)
		return Bounce_easeIn(t * 2, 0, c, d) * .5f + b;
	else return Bounce_easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}