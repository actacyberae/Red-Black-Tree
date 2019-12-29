#include <stdio.h>
#include <math.h>
#include "./libs/lib.h"
#include "./libs/RBT.h"

int main (int argc, char *argv[]) {
	RBT pRBT = {0, NULL};
	RBTptr pNode;
	insert_node_into_RBT(&pRBT, 40);
	insert_node_into_RBT(&pRBT, 30);
	insert_node_into_RBT(&pRBT, 35);
	insert_node_into_RBT(&pRBT, 34);
	insert_node_into_RBT(&pRBT, 32);
	insert_node_into_RBT(&pRBT, 25);
	insert_node_into_RBT(&pRBT, 1);
	insert_node_into_RBT(&pRBT, 27);
	insert_node_into_RBT(&pRBT, 26);
	insert_node_into_RBT(&pRBT, 28);
	insert_node_into_RBT(&pRBT, 60);
	insert_node_into_RBT(&pRBT, 50);
	insert_node_into_RBT(&pRBT, 47);
	insert_node_into_RBT(&pRBT, 55);
	insert_node_into_RBT(&pRBT, 57);
	insert_node_into_RBT(&pRBT, 52);
	insert_node_into_RBT(&pRBT, 54);
	insert_node_into_RBT(&pRBT, 53);
	insert_node_into_RBT(&pRBT, 70);
	insert_node_into_RBT(&pRBT, 65);
	insert_node_into_RBT(&pRBT, 75);
	tree_info(&pRBT);
	
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 25);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 75);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 34);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 32);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 1);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	
	print_line(STDOUT_FILENO, "========\n");
	pNode = get_node_by_key(&pRBT, 33);
	
	print_line(STDOUT_FILENO, "========\n");
	delete_node(&pRBT, pNode);
	tree_info(&pRBT);
	
	print_line(STDOUT_FILENO, "Exit\n");
	delete_tree(&pRBT);
	
	return 0;
}
