/*CATE YUK 
Reads a plain text file, computes a Huffman code tree for that text file,
writes out the encoded version of the text
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct node {
    unsigned char ch;
    int count = 0;
    node* left = NULL;
    node* right = NULL;
} ;


struct code {
    string path;
    unsigned char x;
} ;


class Tree {

    private:
        int spot = 0;
    public:
        node* root;

        int origCount;

        int size;

        node* MinHeap[128];

        //Constructor
        Tree() {
            root = NULL;
            size = 0;
            spot = 0;
            origCount = 0;
        }

        //Destructor
        ~Tree() {
            node* n = root;
            if (n->left) delete n->left;
            if (n->right) delete n->right;
            delete n;
        }

        //Creates new node using data
        void CreateNode(unsigned char tempCh, int tempCount) {
            node* temp = new node;
            temp->ch = tempCh;
            temp->count = tempCount;
            temp->left = NULL;
            temp->right = NULL;
            origCount++;
            MinHeapInsert(temp);
        }

        //Inserts node as a leaf of the min-heap
        void MinHeapInsert(node* n) {
            size++;
            int index = size - 1;
            MinHeap[index] = n;
            swapUp(index);
            root = MinHeap[0];
        }


        void swapUp(int index) {
            int parent = (index-1)/2;
            while (index != 0 && MinHeap[parent]->count > MinHeap[index]->count) {
                swap(&MinHeap[index], &MinHeap[parent]);
                index = parent;
            }
        }


        //Swaps the node from the extract-min into the correct spot
        void minHeapify(int index) {
            int min = index;
            int left = (min * 2) + 1;
            int right = (min * 2) + 2;

            if (left < size && MinHeap[left]->count < MinHeap[min]->count)
                min = left;
            if (right < size && MinHeap[right]->count < MinHeap[min]->count)
                min = right;
            if (min != index) { //If root is not the smallest value
                swap(&MinHeap[min], &MinHeap[index]);
                minHeapify(min);
            }
        }


        //General swap function            
        void swap(node** one, node** two) {

            node* temp = *one;
            *one = *two;
            *two = temp;
        }


        //Extracts root from the min-heap
        node* ExtractMin() {
            node* t = MinHeap[0]; //Store the current root
            MinHeap[0] = MinHeap[size-1]; //Make last leaf the new root
            size--; //"Delete" the old root
            minHeapify(0); //Fix violations 
            return t;
        }


        void HuffManTree() {
            int i = 0; //Increments unique character label
            while (size != 1) {
                //Gets two least frequent characters from the min-heap
                node* L = ExtractMin();
                node* R = ExtractMin();

                //Creates combined parent node
                node* hRoot = new node();
                hRoot->ch = (char)(128+i);
                hRoot->count = L->count + R->count;
                hRoot->left = L;
                hRoot->right = R;

                //Adds combined node into min-heap array
                MinHeapInsert(hRoot);
                i++;

            }
            root = MinHeap[0]; //Last remaining node is root

        }


        //Recursively writes the preorder traversal
        void PreorderTraversal(ostream &preorder, node* n) {
            if (n == NULL)
                return;
            preorder.put(n->ch);
            PreorderTraversal(preorder, n->left);
            PreorderTraversal(preorder, n->right);
        }

        void InorderTraversal(ostream &inorder, node* n) {
            if (n == NULL)
                return;
            InorderTraversal(inorder, n->left);
            inorder.put(n->ch);
            InorderTraversal(inorder, n->right);
        }

        void what(node* n) {
            n = n->right;
            n = n->right;
            cout << "Check  " << n->ch << ": " << n->count << endl;
        }



        //Struct = 'code': unsigned char x and string path
        void EncodeTree(node *n, string& str, code* codeArr) {
            if (n->left == NULL && n->right == NULL) { //leaf
                code temp;
                temp.x = n->ch; //Store char
                temp.path = str; //Store code
                codeArr[spot] = temp; //Add node to array
                str = ""; //Clear string
                spot++;
            }
            if (n->left != NULL) {
                string str1 = str + '0';
                EncodeTree(n->left, str1, codeArr);
            }
            if (n->right != NULL) {
                string str2 = str + '1';
                EncodeTree(n->right, str2, codeArr);
            }
        }


} ;

int main(int argc, char **argv) {

    //Declare and open files
    ifstream textFile;
    ofstream inorder;
    ofstream preorder;
    ofstream codeFile;
    textFile.open(argv[1]);
    inorder.open("inorder");
    preorder.open("preorder");
    codeFile.open("code.txt");

    if (argc != 2) cout << "Error: incorrect number of arguments" << endl;

    //Try to open file
    if (!textFile.is_open()) {
        cout << "Error: could not open text file " << argv[1] << endl;
        return 1;
    }

    Tree A;
    char c;
    int ascii;
    int totalCount = 0;
    node Array[128];


    //Store instances of all characters in an array
    while (textFile >> noskipws >> c) {
        ascii = (int) c;
        Array[ascii].ch = c;
        Array[ascii].count++;
        totalCount++;
    }

    //Insert each appearing character into the MinHeap
    for (int i = 0; i < 128; i++) {
        if (Array[i].count > 0)
            A.CreateNode(Array[i].ch, Array[i].count);
    }
    textFile.close();


    //Build the Huffman Tree
    A.HuffManTree();


    //Write traversals to files
    A.PreorderTraversal(preorder, A.root);
    preorder.close();

    A.InorderTraversal(inorder, A.root);
    inorder.close();



    //Write the encoded version of the original text to file
    textFile.open(argv[1]);
    string str; //path of Huffman Tree
    string binary;
    code codeArr[A.origCount]; //Table of paths

    A.EncodeTree(A.root, str, codeArr);

    char b;
    while (textFile >> noskipws >> b) {
        for (int i = 0; i < A.origCount; i++) {
            if (b == codeArr[i].x) {
                codeFile << codeArr[i].path;
                binary += codeArr[i].path;
            }
        }
    }
    codeFile.close();



    //Binary Attempt
    binary = to_string(totalCount) + binary;
    ofstream binaryFile("code.bin", std::ios::binary);
    binaryFile.write((char*) &binary, sizeof(string));

       


    return 0;
}

