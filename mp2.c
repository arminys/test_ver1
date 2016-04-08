#include <math.h>

#include "mp2.h"
#define pi = 3.1416


/*
 * gaussianFilter -- compute a Gaussian filter for a specific sigma;
 *                   note that you must calculate the appropriate radius
 *                   based on sigma; the filter should be a square
 *                   with height and width (2 * radius + 1)
 * INPUTS: sigma -- the desired value of sigma
 * OUTPUTS: filter -- write the filter to this array; enough space will
 *                    have been allocated by the caller (assuming that
 *                    you calculate radius correctly)
 * RETURN VALUE: none
 */
void gaussianFilter(double* filter, double sigma)
{
	double r, s = 2.0 * sigma * sigma; 
	double x, y;
	double size;
	size = ceil(3.0*sigma);
	for (x = -size; size >= x; x++)
		for (y = -size; size >= y; y++)
		{
			r = sqrt(x*x + y*y);
			filter[(int)((x+size) + (y+size)*(2*size + 1))] = (exp(-(r*r)/s))/(M_PI * s);
		}
	return;
}

/*
 * convolveImage -- performs a convolution between a filter and image;
 *                  the alpha channel should simply be copied, not
 *                  convolved with the filter
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array) 
 *         inAlpha -- pointer to the input alpha channel (1-D array)
 *         radius -- radius of the convolution filter
 *         filter -- pointer to the convolution filter (1-D array)
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 *          outAlpha -- pointer to the output alpha channel (1-D array)
 * RETURN VALUE: none
 */
void sortarr(int* number, int n){
	int i,j,a;
	    for (i = 0; i < n; ++i)
    {
        for (j = i + 1; j < n; ++j)
        {
            if (number[i] > number[j])
            {
                a =  number[i];
                number[i] = number[j];
                number[j] = a;
            }
        }
    }
}

void convolveImage (int width, int height,
                    const uint8_t* inRed, const uint8_t* inGreen, 
		    const uint8_t* inBlue, const uint8_t* inAlpha, 
                    int radius, const double* filter,
		    uint8_t* outRed, uint8_t* outGreen,
                    uint8_t* outBlue, uint8_t* outAlpha)
{

	int i = 0;
	int j = 0;
	int radius1=1;
	int x, y, xf, yf;
	int arrayR[9] = {0,0,0,0,0,0,0,0,0};
	int arrayG[9] = {0,0,0,0,0,0,0,0,0};
	int arrayB[9] = {0,0,0,0,0,0,0,0,0};
	//copy alpha
	for(y = 0; y < height; y++){
	for(x = 0; x < width; x++){
		outAlpha[x + y*width] = inAlpha[x + y*width];
		//outBlue[x + y*width] = inBlue[x + y*width];
		//outGreen[x + y*width] = inGreen[x + y*width];
	}}
	//do the convolution
	for (j=0;j<9;j++){
		arrayR[j] =0;
		arrayG[j] =0;
		arrayB[j] =0;
	}
	for(y = 1; y < height; y++){
	for(x = 1; x < width; x++){
		for(yf = -radius1; radius1 >= yf; yf++){
		for(xf = -radius1; radius1 >= xf; xf++){				
			if(!((x + xf) < 0 || (y + yf) < 0 || (x + xf) >= width || (y + yf) >= height)){				
				arrayR[i] = inRed[ (x + xf) + (y + yf)*(width)];
				arrayG[i] = inGreen[ (x + xf) + (y + yf)*(width)];
				arrayB[i] = inBlue[ (x + xf) + (y + yf)*(width)];
				i++;
		}				
		}}
		sortarr(arrayR,9);
		sortarr(arrayG,9);
		sortarr(arrayB,9);
		outRed[x + y*width] = (int)arrayR[4];
		outGreen[x + y*width] = (int)arrayG[4];
		outBlue[x + y*width] = (int)arrayB[4];
			
		//reset
		i = 0;
	}}
}




/*
 * greyscale -- convert an image to greyscale by calculating the
 *              inner product of each pixel with an RGB filter;
 *              copy the alpha channel (do not modify it)
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         inAlpha -- pointer to the input alpha channel (1-D array)
 *         gMonoMult -- pointer to array with constants to be multiplied
 *                      with RGB channels to convert image to grayscale 
 *                      (3-element array)
 * OUTPUTS: outRed -- pointer to the output red channel (1-D array)
 *          outGreen -- pointer to the output green channel (1-D array)
 *          outBlue -- pointer to the output blue channel (1-D array)
 *          outAlpha -- pointer to the output alpha channel (1-D array)
 * RETURN VALUE: none
 */
void greyscale (int width, int height, 
	        const uint8_t* inRed, const uint8_t* inGreen,
		const uint8_t* inBlue, const uint8_t* inAlpha,
	        const double* gMonoMult,
		uint8_t* outRed, uint8_t* outGreen,
	        uint8_t* outBlue, uint8_t* outAlpha)
{
	int x,y;
	double set_px = 0;
	for(y = 0; y < height; y++){
	for(x = 0; x < width; x++){
		set_px = inRed[x+y*width]*gMonoMult[0] + inGreen[x+y*width]*gMonoMult[1] + inBlue[x+y*width]*gMonoMult[2];
		if(set_px > 255)
			set_px = 255;
		if(set_px < 0)
			set_px = 0;
		outRed[x+y*width] = set_px;
		outGreen[x+y*width] = set_px;
		outBlue[x+y*width] = set_px;
		outAlpha[x+y*width] = inAlpha[x+y*width];
	}}	
}


/*
 * invertColors -- inverts the colors of the image (replaces R, G, and B
 *                 channels by (255 - (pixel value)) ); the alpha channel
 *                 should not be changed
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         inAlpha -- pointer to the input alpha channel (1-D array)
 * OUTPUTS: outRed - pointer to the output red channel (1-D array)
 *          outGreen - pointer to the output green channel (1-D array)
 *          outBlue - pointer to the output blue channel (1-D array)
 *          outAlpha - pointer to the output alpha channel (1-D array)
 * RETURN VALUE: none
 */
void invertColors (int width, int height,
		   const uint8_t* inRed, const uint8_t* inGreen,
		   const uint8_t* inBlue, const uint8_t* inAlpha,
		   uint8_t* outRed, uint8_t* outGreen,
                   uint8_t* outBlue, uint8_t* outAlpha)
{	
	int x,y;
	for(y = 0; y < height; y++){
	for(x = 0; x < width; x++){
		outRed[x+y*width] = 255 - inRed[x+y*width];
		outGreen[x+y*width] = 255 - inGreen[x+y*width];
		outBlue[x+y*width] = 255 - inBlue[x+y*width];
		outAlpha[x+y*width] = inAlpha[x+y*width];
	}}
}

