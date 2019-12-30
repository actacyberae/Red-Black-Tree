#ifndef RBT_LIBRARY
#define RBT_LIBRARY

#include "./lib.h"

#define BLACK 0
#define RED 1

#define ER_EMPTYTREE -1
#define ER_EMPTYNODE -2
#define ER_UNKNWPRNT -3

#define TRUE 1
#define FALSE 0

typedef struct node *RBTptr;

typedef unsigned char tColor;

typedef struct node {
	int key;
	tColor color;
	RBTptr left, right;
} RBTnode;

typedef struct RBT {
	int size;
	RBTptr root;
} RBT;

void insert_node_into_RBT(RBT *pRBT, int pKey);

void tree_info(RBT *pRBT);

void delete_tree(RBT *pRBT);

void delete_node(RBT *pRBT, RBTptr pNode);

RBTptr get_node_by_key(RBT *pRBT, int pKey);

#endif
