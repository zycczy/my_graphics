/*author: charles cheng 2018-04-10*/
#include "canny.h"
/* The canny egde detection:
  *	1. Guass filtrer
  *	2. Sobel calculate the grad of x, y direction and the amplitude
  *	3. non-maxima suppression
  *	4. double threshold and connect the edge
  */
int canny(HBMP_i_t *hbmp_src)
{
	
	spatial_filter(hbmp_src, TEMPLATE_SMOOTH_GAUSS);
	
	return EPDK_OK;
}

