#include <stdio.h>
#include <math.h>
#include "./libs/lib.h"
#include "./libs/RBT.h"

int main (int argc, char *argv[]) {
	RBT pRBT = {0, NULL};
	RBTptr pNode;
	Insert_node(&pRBT, 40);
	Insert_node(&pRBT, 30);
	Insert_node(&pRBT, 35);
	Insert_node(&pRBT, 34);
	Insert_node(&pRBT, 32);
	Insert_node(&pRBT, 25);
	Insert_node(&pRBT, 1);
	Insert_node(&pRBT, 27);
	Insert_node(&pRBT, 26);
	Insert_node(&pRBT, 28);
	Insert_node(&pRBT, 60);
	Insert_node(&pRBT, 50);
	Insert_node(&pRBT, 47);
	Insert_node(&pRBT, 55);
	Insert_node(&pRBT, 57);
	Insert_node(&pRBT, 52);
	Insert_node(&pRBT, 54);
	Insert_node(&pRBT, 53);
	Insert_node(&pRBT, 70);
	Insert_node(&pRBT, 65);
	Insert_node(&pRBT, 75);
	tree_info(&pRBT);
	
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 25, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 75, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 34, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 32, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 1, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pRBT, 33, &pNode);
	
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "Exit\n");
	Free_tree(&pRBT);
	
	return 0;
}
