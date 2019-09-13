/*! \file MinHeapNode.h
    \brief TODO
    
    Details: TODO
*/

#ifndef COMPRESSION_NODE_H
#define COMPRESSION_NODE_H

#include <cstddef>

namespace comp {

// A Huffman tree node
struct Node {
   unsigned short data;       // One of the input pixels
   bool hasData;
   int freq;                  // Frequency of the pixel
   Node *left, *right; // Left and right child

   Node(unsigned short data, bool hasData, int freq) {
      left = right = NULL;
      this->data = data;
      this->hasData = hasData;
      this->freq = freq;
   }

   ~Node() {
      delete left;
      left = NULL;
      delete right;
      right = NULL;
   }
};

// utility function for the priority queue
struct compare {
   bool operator()(Node* l, Node* r) {
      return (l->freq > r->freq);
   }
};
}
#endif // COMPRESSION_NODE_H
