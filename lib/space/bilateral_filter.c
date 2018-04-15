/*author: charles cheng 2017-12-25*/
#include "bilateral_filter.h"
/*
  * Discrete
  * The bilateral filter output dst_pixel(i, j) depends on neighbour pixels value and the weight
  * 	g(i, j) = E<k, l>[f(k, l)*w(i, j, k, l)] / E<k, l>[w(i, j, k, l)]
  * and the weight w(i, j, k, l) depends on distance core d(i, j, k, l) and color value core r(i, j, k, l)
  * 	d(i, j, k, l) = exp[ (-(i-k)^2 + (j-l)^2) /2*sig_d^2 ]
  * 	r(i, j, k, l) = exp[ (-(f(i, j) - f(k, l))^2) /2*sig_r^2 ]
  * Default using odd length of array side, minimum 3 and maximum 9, so we can use look-up table to get d(i, j, k, l) and r(i, j, k, l).
  */
#define MAX_MOD_LEN 5
static double *guass_distances_table_init(void)
{
	int abs_len = (MAX_MOD_LEN - 1) / 2;
	int *distances_table = malloc(sizeof(int)*abs_len);
	
	
	
}
static void guass_value_array_init(uint32_t side_length)
{

}

static void bilateral_window_init()
{
	
}

