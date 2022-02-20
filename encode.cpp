#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include "defns.h"

using namespace std;

void merge(tree *arr, int l, int mid, int r);

void mergeSort(tree *arr, int l, int r);

void insertion(tree *arr, int size);

void encode(const char *, const char *);

void encodingTree(struct symbol *symbols);

void sortT(struct tree *arr, int size);

void buildSymbolsTree(struct tree *alpha, int size);

void splitSymbolsArray(struct tree *alpha, struct tree *nonAlpha, struct symbol *symbols);


int main(int argc, char **argv) {
    encode(argv[1], argv[2]);
    return 0;
}

void encode(const char *fileIn, const char *sortType) {
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

  numOfSymbols = nonAlphaNum + alphaNum;    // total number of symbols used

  // initializing Alpha and non-alpha arrays
  struct tree *alpha = new struct tree[alphaNum];
  struct tree *nonAlpha = new struct tree[nonAlphaNum];

  // creating alpha and nonAlpha arrays
  splitSymbolsArray(alpha, nonAlpha, symbols);

  // checking what type of sort is used (i.e. insertion or mergeSort)
  if (strcmp(sortType, "insertion") == 0)
  {
    insertion(alpha, alphaNum);
    insertion(nonAlpha, nonAlphaNum);
  }
  else if (strcmp(sortType, "merge") == 0)
  {
    mergeSort(alpha, 0, alphaNum - 1);
    mergeSort(nonAlpha, 0, nonAlphaNum - 1);
  }

  // creating two separate trees from alpha and nonAplpha arrays
  buildSymbolsTree(alpha, alphaNum);
  buildSymbolsTree(nonAlpha, nonAlphaNum);

  // connecting the two trees using one root of type struct symbol
  struct symbol *root = new struct symbol;
  root->left = alpha[0].root;
  root->right = nonAlpha[0].root;
  alpha[0].root->parent = root;
  nonAlpha[0].root->parent = root;
  root->freq = alpha[0].root->freq + nonAlpha[0].root->freq;

  // creating the encode values for the symbols
  encodingTree(symbols);

  // sending the symbols with their encodings to stdout
  cout << numOfSymbols << '\n';
  for (int i = 0; i < NSYMBOLS; i++)
  {
    if (symbols[i].freq > 0)
      cout << i << '\t' << symbols[i].symbol << '\t' << symbols[i].encoding << '\n';
  }
  cout << '\n';
  char c;

  // sending the binary encoding to stdout as one line
  while (cin.get(c))
  {
    cout << symbols[c].encoding;
  }
  cout << symbols[10].encoding;

  infile.close();
}

// this function takes two param array of struct tree and
// the size of it, and sort the array in lexicographic ordering
// using insertion sort
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
    if (arr[j].freq == arr[j + 1].freq && arr[j].index > arr[j + 1].index)
    {
      tree temp = arr[j];
      arr[j] = arr[j + 1];
      arr[j + 1] = temp;
    }
  }
}

// this funciton is the merge step that is used
// by the mergeSort fucniton.
// it splits one array into two subarrays, sort them
// and then merge them back together.
void merge(struct tree *arr, int const l, int const mid, int const r)
{
  int const sizeLeft = mid - l + 1;
  int const sizeRight = r - mid;
    // Create temp arrays
  struct tree *leftArr = new struct tree[sizeLeft];
  struct tree *rightArr = new struct tree[sizeRight];

    // separate array into 2 sub arrays
  for (int i = 0; i < sizeLeft; i++)
      leftArr[i] = arr[l + i];
  for (int j = 0; j < sizeRight; j++)
      rightArr[j] = arr[mid + 1 + j];
int indexLeftArr = 0;
  int indexRightArr = 0;
  int indexArr = l;

  // mereging the two array into array *arr
  while (indexLeftArr < sizeLeft && indexRightArr < sizeRight)
  {
    if (leftArr[indexLeftArr].freq <= rightArr[indexRightArr].freq)
    {
      arr[indexArr] = leftArr[indexLeftArr];
      indexLeftArr++;
    }
    else
    {
      arr[indexArr] = rightArr[indexRightArr];
      indexRightArr++;
    }
      indexArr++;
  }

  // adding the remaining of elements that were not added.
  for (; indexLeftArr < sizeLeft; indexLeftArr++)
    arr[indexArr++] = leftArr[indexLeftArr];

  for (; indexRightArr < sizeRight; indexRightArr++)
    arr[indexArr++] = rightArr[indexRightArr];
}

// mergeSort takes an array sort it by spliting
// it in half, sort each half, and then mergeing them
// toegether, recursively.
void mergeSort(struct tree *arr, int const l, int const r)
{
  if (l < r)
  {
    int mid = floor((r + l) / 2);
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);
    merge(arr, l, mid, r);
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
      if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122)) //if alpha chars
      {
        alpha[j].index = symbols[i].symbol;
        alpha[j].symbol = symbols[i].symbol;
        alpha[j].freq = symbols[i].freq;
        alpha[j].root = &symbols[i];
        j++;
      }
      else  // if non-alpha chars
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
// and create a tree using that array, with leaf as the
// symbols (chars) that are going to be encoded/decoded
void buildSymbolsTree(struct tree *arr, int size)
{
  while (size > 1)
  {
    struct symbol *T = new struct symbol;
    T->parent = NULL;
    arr[0].root->parent = T;
    arr[1].root->parent = T;
    T->left = arr[0].root;
    T->right = arr[1].root;
    T->freq = arr[0].freq + arr[1].freq;
    // copy to "delete" the first 2 nodes
    copy(arr + 2, arr + size, arr + 1);
    arr[0].freq = T->freq;
    arr[0].index = -1;
    arr[0].root = T;
    arr[size - 1].freq = -1;
    arr[size - 1].root = NULL;
    sortT(arr, size - 1);

    size--;
  }
}
// this function sort the new T symbol nodes that are
// being used to build the alpha and non-alpha trees
// (helper method for the buildSymbolsTree funciton)
void sortT(struct tree *arr, int size)
{
  int i = 0;
  while (arr[i].freq >= arr[i + 1].freq && arr[i + 1].freq != -1 && i < size)
  {
    tree temp = arr[i];
    arr[i] = arr[i + 1];
    arr[i + 1] = temp;
    i++;
  }
}


// this function uses the tree to write the encodings
// to each symbol of the symbols array
void encodingTree(struct symbol *symbols) {
  for (int i = 0; i < NSYMBOLS; i++)
  {
    // declaring two temporary variables ot trevese the tree
    struct symbol *curr;
    struct symbol *temp;
    char tempChar[E_LEN];  // encoding string
    int j = 0;            // index of encoding string
    if (symbols[i].freq > 0)
    {
      curr = &symbols[i];  // address of the leaf node
      temp = curr->parent;  // address of the leaf node parent
      while (temp->parent != NULL)  // is root of the tree
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
      tempChar[j] = '\0';   // ending temporary cstring
      int index = 0;
      // inserting the encoding in correct order (reversing the string)
      for (int k = j - 1; k >= 0; k--)
      {
        symbols[i].encoding[index] = tempChar[k];
        index++;
      }
      symbols[i].encoding[j] = '\0';
    }
  }
}
