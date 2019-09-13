/*! \file Compression.cpp
    \brief TODO
    
    Details: TODO
*/

#include "compression/Compression.h"
#include <bits/stdc++.h>
#include <time.h>

using namespace comp;

const unsigned int SIZE_UINT = sizeof(unsigned) * CHAR_BIT;

//==========================================================================

int* Compression::encodePixels(const unsigned short *pixels, unsigned int size, unsigned int &encodedPixelSize)
{
   std::cout << "Encoding pixels" << std::endl;
   assert(pixels != NULL);
   
   clock_t t;
   t = clock();

   std::string encodedString;
   if(_codes.empty()) {
      std::cout << "Compute frequency and huffman codes" << std::endl;
      computeFrequencyMap(pixels, size);
      HuffmanCodes();
   } else {
      std::cout << "Huffman codes already exist, no need to recompute them." << std::endl;
   }

   for (unsigned int i = 0; i < size; i++) {
      encodedString += _codes[pixels[i]];
   }

   encodedPixelSize = encodedString.size();
   unsigned int sizeBitArray = (encodedPixelSize / SIZE_UINT) + ((encodedPixelSize % SIZE_UINT != 0) ? 1 : 0);

   int *encodedPixels = new int[sizeBitArray];
   for (unsigned int i = 0; i < sizeBitArray; i++) {
      encodedPixels[i] = 0;
   }

   for (unsigned int i = 0; i < encodedPixelSize; i++) {
      if (encodedString[i] == '1') {
         encodedPixels[(i/SIZE_UINT)] |= (1 << (i%SIZE_UINT));
      }
   }
   encodedString = "";

   t = clock() - t;
   std::cout << "COMPRESSION - It took "<< ((float)t)/CLOCKS_PER_SEC <<" second(s) to encode image of size " << size << " encoded sizeBitArray: " << sizeBitArray << std::endl;

   return encodedPixels;
}

//==========================================================================

void Compression::decodePixels(int* encodedPixels, unsigned int encodedPixelsSize, unsigned short *outputPixels, unsigned int outputPixelsSize) const
{
   std::cout << "Decoding pixels" << std::endl;
   assert(encodedPixels != NULL);

   clock_t t;
   t = clock();

   outputPixels = new unsigned short[outputPixelsSize];

   unsigned int index = 0;
   Node* curr = _minHeap.top();

   for (unsigned int i = 0; i < encodedPixelsSize; i++) {
      if (encodedPixels[(i/SIZE_UINT)] & (1 << (i%SIZE_UINT))) {
         curr = curr->right;
      } else {
         curr = curr->left;
      }

      // reached leaf node
      if (curr->left == NULL && curr->right == NULL) {
         outputPixels[index] = curr->data;
         index++;
         curr = _minHeap.top();
      }
   }

   t = clock() - t;
   std::cout << "COMPRESSION - It took " << ((float)t)/CLOCKS_PER_SEC << " second(s) to decode the image" << std::endl;
}

//==========================================================================

void Compression::storeCodes(Node* root, const std::string &str)
{
   if (root == NULL) {
      return;
   }
   if (root->hasData) {
      _codes[root->data] = str;
   }
   storeCodes(root->left, str + "0");
   storeCodes(root->right, str + "1");
}

//==========================================================================

void Compression::HuffmanCodes()
{
   Node *left, *right, *top;

   for (std::unordered_map<unsigned short, int>::iterator v = _freq.begin(); v != _freq.end(); v++) {
      _minHeap.push(new Node(v->first, true, v->second));
   }

   // Frequency map is no longer needed, clear it
   _freq.clear();

   while (_minHeap.size() != 1) {
      left = _minHeap.top();
      _minHeap.pop();
      right = _minHeap.top();
      _minHeap.pop();
      top = new Node(0, false, left->freq + right->freq);
      top->left = left;
      top->right = right;
      _minHeap.push(top);
   }
   storeCodes(_minHeap.top(), "");
}

//==========================================================================

void Compression::computeFrequencyMap(const unsigned short *pixels, unsigned int size)
{
   // Do not try to parallelize the following for loop since a pixel can be present multiple times,
   // incrementing it parallely would cause a segmentation fault.
   for (unsigned int i = 0; i < size; i++) {
      _freq[pixels[i]]++;
   }
}
