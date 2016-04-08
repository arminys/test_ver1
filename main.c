#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "imageData.h"
#include "mp2.h"

const double gMonoMult[3] = { .299, .587, .114 };

int
main (int argc, const char* const argv[])
{
    char garbage[2];
    int command;
    double sigma;

    if (4 > argc || 5 < argc) {
        fprintf (stderr, 
	         "syntax: %s <input file> <output file> <command #> ...\n", 
	         argv[0]);
        return 2;
    }

    if (1 != sscanf (argv[3], "%d%1s", &command, garbage) || 
        1 > command || 3 < command) {
	fprintf (stderr, "Command must be from 1 to 3.\n");
	fprintf (stderr, "  1 -- gaussian filter   <sigma>\n");
	fprintf (stderr, "  2 -- greyscale\n");
	fprintf (stderr, "  3 -- invert colors\n");
	return 2;
    }

    if (1 == command && 
        (5 != argc ||
         1 != sscanf (argv[4], "%lf%1s", &sigma, garbage) || 
	 0 >= sigma || 100 <= sigma)) {
    	fprintf (stderr, "Sigma must be greater than 0 and less than 100.\n");
	return 2;
    }

    Image* inputImage = decode (argv[1]);
    printf ("Width: %d, height: %d\n", inputImage->width, inputImage->height);
    Image* outputImage = generateOutput (inputImage);

    int height = inputImage->height;
    int width  = inputImage->width;
    uint8_t* inRed    = inputImage->redChannel;
    uint8_t* inBlue   = inputImage->blueChannel;
    uint8_t* inGreen  = inputImage->greenChannel;
    uint8_t* inAlpha  = inputImage->alphaChannel;
    uint8_t* outRed   = outputImage->redChannel;
    uint8_t* outBlue  = outputImage->blueChannel;
    uint8_t* outGreen = outputImage->greenChannel;
    uint8_t* outAlpha = outputImage->alphaChannel;

    switch (command) {
	case 1: {
	    int radius = ceil (3 * sigma);
	    int fSize = 1 + 2 * radius;
	    double* gauss = malloc (sizeof (gauss[0]) * fSize * fSize);
	    gaussianFilter (gauss, sigma);
	    convolveImage (width, height, inRed, inGreen, inBlue, inAlpha, 
			   radius, gauss, outRed, outGreen, outBlue, outAlpha);
	    encode (argv[2], outputImage);
	    break;
	}
	case 2:
	    greyscale (width, height, inRed, inGreen, inBlue, inAlpha, 
	               gMonoMult, outRed, outGreen, outBlue, outAlpha);
	    encode (argv[2], outputImage);
	    break;
	case 3:
	    invertColors (width, height, inRed, inGreen, inBlue, inAlpha, 
	    		  outRed, outGreen, outBlue, outAlpha);
	    encode (argv[2], outputImage);
	    break;
    }

    freeImage (inputImage);
    freeImage (outputImage);

    return 0;
}
