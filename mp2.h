#if !defined(MP2_H)
#define MP2_h

#include <stdint.h>

// Compute a Gaussian filter with radius ceil (3*sigma) and standard 
// deviation sigma.
extern void gaussianFilter (double* filter, double sigma);

// Convolve an image (RGB) with a filter.  Copy the alpha channel.
extern void convolveImage 
    (int width, int height, const uint8_t* inRed, const uint8_t* inGreen, 
     const uint8_t* inBlue, const uint8_t* inAlpha, int radius, 
     const double* filter, uint8_t* outRed, uint8_t* outGreen,
     uint8_t* outBlue, uint8_t* outAlpha);

// Convert an image to greyscale, using the 3-element array gMonoMult 
// as multipliers for the RGB channels.  Copy the alpha channel.
extern void greyscale 
    (int width, int height, const uint8_t* inRed, const uint8_t* inGreen, 
     const uint8_t* inBlue, const uint8_t* inAlpha, const double* gMonoMult,
     uint8_t* outRed, uint8_t* outGreen, uint8_t* outBlue, uint8_t* outAlpha);

// Invert the RGB channels in an image.  Copy the alpha channel.
extern void invertColors 
    (int width, int height, const uint8_t* inRed, const uint8_t* inGreen,
     const uint8_t* inBlue, const uint8_t* inAlpha, uint8_t* outRed, 
     uint8_t* outGreen, uint8_t* outBlue, uint8_t* outAlpha);

#endif // MP2_h
