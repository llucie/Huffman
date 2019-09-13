
# Huffman

This depository contains a simple implementation of the Huffman compression algorithm.
This algorithm is lossless, it means that no information is lost during compression and decompression.

Huffman algorithm creates a binary tree based on the frequency of the pixels in the input image.
From this tree, we can determine a map, mapping each pixel to a binary code.
The more frequent a pixels is in the image, the smallest will be its associated code.

The output encoded image will be a sequence of 0 and 1.
Since C++ does not handle bit arrays, the encoded sequence is represented as an array of ints. An int size is 32 bits, it means that we can store a 32-bits sequence in a single int.


## Build and run

    git clone https://github.com/llucie/Huffman
    cd Huffman
    make
    ./Huffman
    

## Clean directory

    make clean


