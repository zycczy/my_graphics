/*author: charles cheng 2016-07-21*/
#include "bresenham.h"

void line_Bresenham(int x0, int y0, int xEnd, int yEnd)
{
	int p0;
	int dx, dy;
	int x = x0;
	int y = y0;
	int p;
	dx = abs(xEnd - x0);
	dy = abs(yEnd - y0);
	p0 = 2*dy - dx;
	p =  p0;
	while(x < xEnd){
		if(p < 0){
			x++;
			set_pixel(x, y);
			p = p+2*dy;
		}else{
			x++;
			y++;
			set_pixel(x, y);
			p = p+2*(dy-dx);
		}
	}
}

