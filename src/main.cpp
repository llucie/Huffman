/*! \file main.cpp
    \brief This file contains the main function that uses the Compression 
           engine to compress a generated image.
*/

#include <iostream>
#include "compression/Compression.h"
#include <assert.h>

int main() {
    
    unsigned int size = 1914*2294;
    unsigned short *pixels = new unsigned short[size];

    int value = 0;
    for(unsigned int i = 0; i < size; i++) {
        pixels[i] = value;
        value++;
        if(value == 255) {
            value = 0;
        }
    }

    comp::Compression compressor;

    // Encode image
    unsigned int encodedPixelSize = 0;
    int *encodedPixels = compressor.encodePixels(pixels, size, encodedPixelSize);
    assert(encodedPixels != NULL);

    // Decode image
    unsigned short *outputPixels = NULL;
    compressor.decodePixels(encodedPixels, encodedPixelSize, outputPixels, size);

    delete[] pixels;
    delete[] outputPixels;

    return 0;
}
