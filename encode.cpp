#include <iostream>
#include <string.h>
#include <fstream>
#include "defns.h"
using namespace std;

void insertion(tree *arr, int size);
void encode(const char *, const char*);
void encodingTree(struct symbol *symbols);
void sortT(struct tree *arr, int size);
void buildSymbolsTree(struct tree *alpha, int size);
void splitSymbolsArray(struct tree *alpha, struct tree *nonAlpha, struct symbol *symbols);
int main(int argc, char **argv)
{
    encode(argv[1],argv[2]);
    return 0;
}

void encode(const char *fileIn, const char* sortType)
{
    ifstream infile;     // declaring a name for the ifstream
    infile.open(fileIn); // openning a preprocess file using ifstream
    struct symbol symbols[NSYMBOLS];
    int currToken;
    int nextToken;
    int alphaNum = 0;
    int nonAlphaNum = 0;
    int numOfSymbols = 0;


    // initializing  symbols array
    for (int i = 0; i < NSYMBOLS; i++)
    {
        symbols[i].symbol = i;
        symbols[i].freq = 0;
        symbols[i].parent = NULL;
        symbols[i].left = NULL;
        symbols[i].right = NULL;
        symbols[i].encoding[0] = '\0';
    }

    // reading preprocess file
    while (infile >> currToken)
    {
        infile >> nextToken;
        if ((currToken >= 65 && currToken <= 90) || (currToken >= 97 && currToken <= 122))
            alphaNum += 1;
        else
            nonAlphaNum += 1;

        symbols[currToken].freq = nextToken;
    }

    numOfSymbols = nonAlphaNum + alphaNum;

    // initializing Alpha and non-alpha arrays
    struct tree *alpha = new struct tree[alphaNum];
    struct tree *nonAlpha = new struct tree[nonAlphaNum];

    // creating alpha and nonAlpha arrays
    splitSymbolsArray(alpha, nonAlpha, symbols);

    // checking what type of sort is used
    if (strcmp(sortType ,"insertion") == 0)
    {
        insertion(alpha, alphaNum);
        insertion(nonAlpha, nonAlphaNum);
    }
    // creating two separate trees for alpha and nonAplpha arrays
    buildSymbolsTree(alpha,  alphaNum);
    buildSymbolsTree(nonAlpha,  nonAlphaNum);

    // connecting the two trees using one root of type struct symbol
    struct symbol root;
    root.left = alpha[0].root;
    root.right = nonAlpha[0].root;
    alpha[0].root->parent = &root;
    nonAlpha[0].root->parent = &root;
    root.freq = alpha[0].root->freq + nonAlpha[0].root->freq;

    // creating the encode values for the symbols
    encodingTree(symbols);

    // sending the symbols with their encodings to stdout
    cout<<numOfSymbols<<'\n';
    for (int i = 0; i < NSYMBOLS; i++)
    {
        if (symbols[i].freq > 0)
            cout<<i<<'\t'<<symbols[i].symbol<<'\t'<<symbols[i].encoding<<'\n';
    }
    cout << '\n';
    char c;

    // sending the binary encoding to stdout as one line
    while(cin.get(c))
    {
        cout << symbols[c].encoding;
    }
    cout << symbols[10].encoding ;

    infile.close();
}

// this function takes two param array of struct tree and
// the size of it, and sort the array in lexicographic ordering
void insertion(tree *arr, int size)
{
    tree key;
    for (int i = 1; i < size; i++)
    {
        key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j].freq > key.freq)
        {

            arr[j + 1] = arr[j];
            j -= 1;
        }

        arr[j + 1] = key;

//        if (arr[j].freq == arr[j + 1].freq && arr[j].index > arr[j + 1].index)
//        {
//            tree temp = arr[j];
//            arr[j] = arr[j + 1];
//            arr[j + 1] = temp;
//        }
    }
}

// this function uses the symbols array as an input
// to create 2 array. 1) alpha only contains alphabetical symbols
// and 2) nonAlpha which contains all other symbols
void splitSymbolsArray(struct tree *alpha, struct tree *nonAlpha, struct symbol *symbols)
{
    int k = 0;
    int j = 0;

    for (int i = 0; i < NSYMBOLS; i++)
    {
        if (symbols[i].freq > 0)
        {
            if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122))
            {
                alpha[j].index = symbols[i].symbol;
                alpha[j].symbol = symbols[i].symbol;
                alpha[j].freq = symbols[i].freq;
                alpha[j].root = &symbols[i];
                j++;
            }

            else
            {
                nonAlpha[k].index = symbols[i].symbol;
                nonAlpha[k].symbol = symbols[i].symbol;
                nonAlpha[k].freq = symbols[i].freq;
                nonAlpha[k].root = &symbols[i];
                k++;
            }
        }
    }
}

// this function take an array of type struct tree with its size
// and create a tree using that array
void buildSymbolsTree(struct tree *alpha, int size)
{
    while (size > 1)
    {
        struct symbol *T = new struct symbol;
        T->parent = NULL;
        alpha[0].root->parent = T;
        alpha[1].root->parent = T;
        T->left = alpha[0].root;
        T->right = alpha[1].root;
        T->freq = alpha[0].freq + alpha[1].freq;

        copy(alpha + 2, alpha + size, alpha + 1);
        alpha[0].freq = T->freq;
        alpha[0].index = -1;
        alpha[0].root = T;
        alpha[size-1].freq = -1;
        alpha[size-1].root = NULL;
        sortT(alpha, size - 1);

        size--;
    }
}
// this function sort the new T symbol nodes that are
// being used to build the alpha and non-alpha trees
void sortT(struct tree *arr, int size)
{
    int i =0;
    while (arr[i].freq >= arr[i+1].freq && arr[i+1].freq != -1 && i < size)
    {
        if (arr[i].freq == arr[i+1].freq && arr[i].index == arr[i+1].index)
        {
            return;
        }
        else
        {
            tree temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            i++;
        }
    }
}

// this function uses the tree to write the encodings
// to each symbol of the symbols array
void encodingTree(struct symbol *symbols)
{
    for (int i = 0; i < NSYMBOLS; i++)
    {
        struct symbol *curr;
        struct symbol *temp;
        char tempChar[E_LEN];
        int j = 0;
        if (symbols[i].freq > 0)
        {
            curr = &symbols[i];
            temp = curr->parent;
            while (temp->parent != NULL)
            {
                if (temp->left == curr)
                    tempChar[j] = '0';
                else if (temp->right == curr)
                    tempChar[j] = '1';

                j++;
                temp = temp->parent;
                curr = curr->parent;
            }
            if (temp->parent == NULL && temp->left == curr)
                tempChar[j] = '0';
            else if (temp->parent == NULL && temp->right == curr)
                tempChar[j] = '1';
            j++;
            tempChar[j] = '\0';

            int index = 0;
            for (int k = j - 1; k >= 0; k--){
            symbols[i].encoding[index] = tempChar[k];
                index++;
            }
            symbols[i].encoding[j] = '\0';
        }
    }
}