# Huffman-Coding-Tree

## Description
This program reads the text from a given file and computes the number of occurences of each character in the file.
Each character and its frequency is placed in a new tree node.
Each new internal node in the Huffman Code Tree is given a unique character label of values 128 and greater.

The pre-order traversal of the tree is written to the file "preorder".
The in-order traversal of the tree is written to the file "inorder".
These files will be generated following execution.

A table is constructed containing the encoding for each character as a string.
The encoded version of the text is written to "code.txt"

A true binary version of the encoded text is written to "code.bin"
The first two bytes int ehf ile indicate the number of characters in the text.
If the last character doesn't finish a byte, the last byte is padded with 0s. 

## Implementation
The file "foo.txt" has been supplied to test the program. Any file you want can be added.
The binary heap data structure uses an array and implements insert and extract-min operations that run in O(log n) time.
The Huffman Code Tree is built using a Min-Heap.

## Compilation and Execution
Clone the repository from temrinal, or download the zip and extract the files.
```bash
git clone hhtps://github.com/aeyuk/Huffman-Coding-Tree.git
```

Build the project using `make` and execute using `./encode foo.txt`, replacing "foo.txt" with whatever file you would like to run.
