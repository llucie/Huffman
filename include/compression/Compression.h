/**
 * @file Compression.h
 * @author Lucie Boursier
 * @date 09-September-2019
 * @brief This class is used to encode and decode an image.
 *        It implements the Huffman algorithm, which is an algo based on
 *        The frequency of the pixels in the image.
 *        Input pixels are encoded in unsigned shorts (16 bits), and the output format
 *        is an array of int values.
 *        This array of ints can be passed to the decode methode to retrieve the initial pixels.
 *        The Huffman compression is lossless, it means that no information is lost during the
 *        compression and decompression.
 *
 *        Time to encode and decode the image is logged in the standard output.
 */

#ifndef COMPRESSION_COMPRESSOR_H
#define COMPRESSION_COMPRESSOR_H

#include "compression/Node.h"
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>

namespace comp {
/**
 * @brief Compression class, that will build the frequency map, the Huffman tree and 
 *        provides the encoding and decoding methods.
 *
 * This class stores the huffman tree and a HashMap mapping each pixel to its Huffman code.
 * It is possible to instanciate a Compressor object for each image to encode, but the Huffman tree can 
 * be build only once and used for all images.
 * The first image encoding time will be longer since the algorithm shall build the tree first, but the
 * other images will be compressed faster.
 * 
 * An improvement to this class would be to template the type of input pixels, to avoid
 * forcing the unsigned short type.
 */
class Compression {
public:
   /**
    * @brief This method encodes the pixels into an array of ints.
    *
    * This function uses the Huffman algorithm to encode the pixels. If the 
    * Huffman tree does not exist, it builds the frequency map and the tree.
    * The codes for each pixels are computed from this tree.
    * Note that the output array contains int values, which correspond to bits encoded
    * in a 32 bits structure.
    * Indeed an int is encoded in 32 bits, e.g. int 1 is represented as 00000000000000000000000000000001.
    * The output of the huffman algorithm is a sequence of 0 and 1. 
    * Since C++ does not handle simple arrays of bits, we use ints to represent the bits.
    * The size of the ouput array is (encodedPixelSize / SIZE_UINT + (encodedPixelSize % SIZE_UINT > 0 ? 1 : 0)
    * It is the minimum size to store the entire sequence of encoded pixels.
    * 
    * @param pixels Non NULL array containing the input pixels to encode
    * @param size Number of pixels contained in the pixels array
    * @param encodedPixelSize Size of the output sequence of 0 and 1.
    * @return Array of ints, of size of encodedPixelSize / SIZE_UINT
    * @warning If input pixels is a NULL array, an assertion is raised
    */
   int* encodePixels(const unsigned short *pixels, unsigned int size, unsigned int &encodedPixelSize);

   /**
    * @brief This method decodes the pixels from the input int array.
    *
    * This method iterate through the input encodedPixels array, and for each value of bit goes through
    * the Huffman tree to determine the pixel associated with the code.
    * Once the pixel value is found in the tree, it is appended to the outputPixels array.
    * 
    * Note that the outputPixels array will be dynamically allocated inside this function.
    * 
    * @param encodedPixels Input int array containing the huffman encoded sequence.
    * @param encodedPixelsSize The number of 0 and 1 in the input encodedPixels array
    * @param outputPixels array containing the pixels decoded.
    * @param outputPixelsSize size of the output pixels array, used to dynamically allocate the pixels with the right size
    * @warning If input pixels is a NULL array, an assertion is raised
    */
   void decodePixels(int* encodedPixels, unsigned int encodedPixelsSize, unsigned short *outputPixels, unsigned int outputPixelsSize) const;

private:
   /**
    * @brief Recursive function to store pixels along with their huffman value in an unordered_map
    *
    * The pixels are associated to a string code, containing a sequence of 0 and 1.
    * This mapping is stored in an unordered_map rather that in a map, for more efficiency.
    * 
    * @param root Root node of the Huffman tree
    * @param str temporary variable used in this recursive method to store the code value.
    */
   void storeCodes(Node* root, const std::string &str);

   /**
    * @brief This function build the Huffman tree from the frequency map.
    *
    * The Huffman tree is composed of Nodes, containing children and the sum of their frequency, and Leafs
    * containing the pixel value and its frequency.
    * 
    * The first step of the algorithm is to create an ordered queue from the frequency map. This queue will contain
    * all the Leafs, sorted by the frequency.
    * Then the algo builds the tree: it takes the 2 nodes with the smallest frequency, creates a Node with the sum
    * of their frequency and add the leafs as children.
    * The next step is to build the codes map, which maps the pixel to the code. The algo will go through the entire tree
    * and stores the code when finding a Leaf.
    */
   void HuffmanCodes();

   /**
    * @brief This function computes the frequency of each pixel contained in the input pixels array.
    *
    * It goes through the input pixels array and increment the number of occurrence of the pixels in
    * a map.
    * 
    * @param pixels Array of pixels
    * @param size Size of the input array
    */
   void computeFrequencyMap(const unsigned short *pixels, unsigned int size);

   /**
    * @brief Map used to map each character its huffman value
    */
   std::unordered_map<unsigned short, std::string> _codes;

   /**
    * @brief Map used to store the frequency of character of the input data
    */
   std::unordered_map<unsigned short, int> _freq;
  
   /**
    * @brief STL priority queue to store heap tree, with respect to their heap root node value
    */
   std::priority_queue<Node*, std::vector<Node*>, compare> _minHeap;
};
}
#endif // COMPRESSION_COMPRESSOR_H
