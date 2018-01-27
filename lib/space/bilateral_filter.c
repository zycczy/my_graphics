/*author: charles cheng 2017-12-25*/
#include "bilateral_filter.h"
/*
The bilateral filter output dst_pixel(i, j) depends on neighbour pixels value and the weight
	g(i, j) = E<k, l>[f(k, l)*w(i, j, k, l)] / E<k, l>[w(i, j, k, l)]
and the weight w(i, j, k, l) depends on distance core d(i, j, k, l) and value core r(i, j, k, l)
	d(i, j, k, l)=exp()
Default using odd length of array side, minimum 3 and maximum 9

*/

static double *guass_distances_array_init(uint32_t side_length)
{
	int center_location;	
	double *distance_array;
	if(side_length<3){
		side_length = 3;
	}else if(side_length>9){
		side_length = 9;
	}
	if(side_length%2 == 0){
		side_length -= 1; 
	}
	distance_array = malloc(side_length*side_length*sizeof(double));
	center_location = (side_length-1)/2;
	
	
}
static void guass_value_array_init(uint32_t side_length)
{

}

static void bilateral_window_init()
{
	
}

