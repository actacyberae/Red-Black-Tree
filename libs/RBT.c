#include "./RBT.h"

/*
* Простая функция распечатки информации об узле pNode: ключе и цвете вер
* шины. Если узел имеет левого и/или правого потомка, то информация такж
* е выводится и для них. Формат информации следующий:
* [КЛП(ЦЛП)|КВ(ЦВ)|КПП(ЦПП)], где
* КЛП - ключ левого потомка
* ЦЛП - цвет левого потомка
* КПП - ключ правого потомка
* ЦПП - цвет правого потомка
* КВ  - ключ вершины
* ЦВ  - цвет вершины
* Если потомок у вершины отсутствует, то вместо информации о потомке, пи
* шется слово NULL.
*/
void node_info(RBTptr pNode) {
	print_line(STDOUT_FILENO, "[");
	if (pNode != NULL) {
		if (pNode->left != NULL) {
			print_line(STDOUT_FILENO, "%d (%d)|", (pNode->left)->key, (pNode->left)->color);
		} else {
			print_line(STDOUT_FILENO, "NULL|");
		}
		print_line(STDOUT_FILENO, "%d (%d)", pNode->key, pNode->color);
			if (pNode->right != NULL) {
			print_line(STDOUT_FILENO, "|%d (%d)", (pNode->right)->key, (pNode->right)->color);
		} else {
			print_line(STDOUT_FILENO, "|NULL");
		}
	} else {
		print_line(STDOUT_FILENO, "NULL");
	}
	print_line(STDOUT_FILENO, "]\n");
}

/*
* Рекурсивная функция распечатки содержимого поддерева с корнем pRoot в 
* порядке симметричного обхода дерева. Пусть корень поддерева обозначает
* ся буквой A, а его левый и правый потомки B и C соответственно.
*      |
*      A
*     / \
*    /   \
*   B     C
* Печать:
* > B A C
*/
static void info_in_order_traversal(RBTptr pRoot) {
	if (pRoot != NULL) {
		info_in_order_traversal(pRoot->left);
		node_info(pRoot);
		info_in_order_traversal(pRoot->right);
	}
}

/*
* Обобщенная функция печати дерева pRBT. В качестве входящего параметра 
* рекурсивной функции печати выступает корень дерева pRBT. Т. о., дерево
* обходится в порядке симметричного обхода и последовательно распечатыва
* ется вся информация об узлах.
*/
void tree_info(RBT *pRBT) {
	info_in_order_traversal(pRBT->root);
}

/*
* Проверка цвета узла pNode. В случае, если узел красный, функция возвра
* щает значение 1, а если узел - черный, то 0. Эта функция будет полезна
* , когда мы будем обращаться к проверке цвета узла при удалении, либо д
* обавлении вершины в красно-черное дерево. Если узел пустой NULL, то во
* звращается значение 0. Это сделано из расчета, что пустой узел по-умол
* чанию должен быть черным.
*/
static int is_node_red(RBTptr pNode) {
	if (pNode == NULL) {
		return 0;
	}
	return (pNode->color == RED) ? 1 : 0;
}

/*
* 
*/
static int node_color_change(RBT *pRBT, RBTptr *pNode) {
	int isRed;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция node_color_change\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (*pNode == NULL) {
		err_msg("> <?!>\n> Функция node_color_change\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (*pNode != pRBT->root) {
		if ((isRed = is_node_red(*pNode)) == -1) {
			err_msg("> <?!>\n> node_color_change -> is_node_red\n> Проблемы обнаружены при обработке функции node_color_change, запускающей функцию is_node_red.\n");
			return -1;
		}
		if (isRed == 1) {
			(*pNode)->color = BLACK;
		} else {
			(*pNode)->color = RED;
		}
	} else {
		(*pNode)->color = BLACK;
	}
	return 1;
}

/*
* Функция возвращает родителя вершины pNode дерева pRBT. В случае, если 
* берется корень дерева pRBT, то возвращается само корневое значение. Пу
* сть корень поддерева обозначается буквой A, а его левый и правый потом
* ки B и C соответственно. Данные сохраняются в pParent по значению.
*      |
*      A
*     / \
*    /   \
*   B     C
* Тогда вызов функции get_parent_of_node для узла C вернет вершину A.
*/
static int get_parent_of_node(RBT *pRBT, RBTptr pNode, RBTptr *pParent) {
	int pKey;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция get_parent_of_node\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция get_parent_of_node\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	pKey = pNode->key;
	*pParent = pRBT->root;
	while (*pParent != NULL) {
		if ((*pParent)->right == pNode || (*pParent)->left == pNode) {
			return 1;
		}
		if ((*pParent)->key > pKey) {
			*pParent = (*pParent)->left;
		} else if ((*pParent)->key < pKey) {
			*pParent = (*pParent)->right;
		} else {
			break;
		}
	}
	return 0;
}

/*
* Функция проверки на то, что узел pNode является левым, либо правым пот
* омком в узла pParent. Если pParent не является родителем pNode, то фун
* кция возвращает отрицательное значение.
*/
static int is_node_a_left_child(RBTptr pNode, RBTptr pParent) {
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция is_node_a_left_child\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (pParent == NULL) {
		err_msg("> <?!>\n> Функция is_node_a_left_child\n> Указанный родитель pParent является пустым.\n");
		return -1;
	}
	if (pParent->left == pNode) {
		return 1;
	} else if (pParent->right == pNode) {
		return 0;
	} else {
		err_msg("> <?!>\n> Функция is_node_a_left_child\n> Вершина pParent с ключом [%d] не является родителем вершины pNode с ключом [%d].\n", pParent->key, pNode->key);
		return -1;
	} 
}

/*
* Функция возвращает брата вершины pNode дерева pRBT. В случае, если бер
* ется корень дерева pRBT, то возвращается само корневое значение. Пусть
* корень поддерева обозначается буквой A, а его левый и правый потомки -
* B и C соответственно. Данные сохраняются в pNodeBrother по значению.  
*      |
*      A
*     / \
*    /   \
*   B     C
* Тогда вызов функции get_sibling_node для узла C вернет вершину B.
*/
static int get_sibling_node(RBT *pRBT, RBTptr pNode, RBTptr *pNodeBrother) {
	RBTptr pParent;
	int isNodeLeft;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция get_sibling_node\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция get_sibling_node\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (pRBT->root != pNode) {
		if (get_parent_of_node(pRBT, pNode, &pParent) == -1) {
			err_msg("> <?!>\n> get_sibling_node -> get_parent_of_node\n> Проблемы обнаружены при обработке функции get_sibling_node, запускающей функцию get_parent_of_node.\n");
			return -1;
		}
		if ((isNodeLeft = is_node_a_left_child(pNode, pParent)) == -1) {
			err_msg("> <?!>\n> get_sibling_node -> is_node_a_left_child\n> Проблемы обнаружены при обработке функции get_sibling_node, запускающей функцию is_node_a_left_child.\n");
			return -1;
		}
		if (isNodeLeft == 1) {
			*pNodeBrother = pParent->right;
		} else {
			*pNodeBrother = pParent->left;
		}
	} else {
		*pNodeBrother = NULL;
	}
	return 1;
}

/*
* Заменяет поддерево с корнем в узле pNode поддеревом с корнем в узле   
* pNewNode. Родителю узла pNode устанавливается новый левый или правый п
* отомок. Если содержимое дерева заменяется полностью, то корень pRBT пр
* инимает новое значение корневого узла pNewNode.                       
*/
static int transplant(RBT *pRBT, RBTptr pNode, RBTptr pNewNode) {
	RBTptr pParent;
	int isNodeLeft;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция transplant\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция transplant\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (pRBT->root != pNode) {
		if (get_parent_of_node(pRBT, pNode, &pParent) == -1) {
			err_msg("> <?!>\n> transplant -> get_parent_of_node\n> Проблемы обнаружены при обработке функции transplant, запускающей функцию get_parent_of_node.\n");
			return -1;
		}
		if ((isNodeLeft = is_node_a_left_child(pNode, pParent)) == -1) {
			err_msg("> <?!>\n> transplant -> is_node_a_left_child\n> Проблемы обнаружены при обработке функции transplant, запускающей функцию is_node_a_left_child.\n");
			return -1;
		}
		if (isNodeLeft == 1) {
			pParent->left = pNewNode;
		} else {
			pParent->right = pNewNode;
		}
	} else {
		pRBT->root = pNewNode;
	}
	return 1;
}

/*
* Простая функция проверки на то, является ли вершина внешней по отношен
* ию к родителю. Например, мы имеем поддерево с корнем, обозначенным бук
* вой A, а его левый и правый потомки - B и C соответственно. Каждый пот
* мок имеет дополнительно пару потомков b1, b2 и c1, c2. Тогда внешним у
* злом по отношению к вершине B является узел b1, а внешним узлом по отн
* ошению к вершине C - с2. Такое определение внешних вершин будет полезн
* ым при определении операции вставки нового узла в дерево, когда поведе
* ние алгоритма должно различаться не только для положения ветвей, но и 
* для расположения потомков этих ветвей поддерева.                      
*           |                                                           
*           A                                                           
*          / \                                                          
*         /   \                                                         
*        /     \                                                        
*       /       \                                                       
*      B         C                                                      
*     / \       / \                                                     
*    /   \     /   \                                                    
*   b1   b2   c1   c2                                                   
*/
static int is_node_external(RBT *pRBT, RBTptr pNode) {
	int isLeftChild;
	int isNodeALeftChild;
	RBTptr pParent;
	RBTptr pGrandParent;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция is_node_external\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция is_node_external\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (get_parent_of_node(pRBT, pNode, &pParent) == -1) {
		err_msg("> <?!>\n> is_node_external -> get_parent_of_node\n> Проблемы обнаружены при обработке функции is_node_external, запускающей функцию get_parent_of_node.\n");
		return -1;
	}
	if (get_parent_of_node(pRBT, pParent, &pGrandParent) == -1) {
		err_msg("> <?!>\n> is_node_external -> get_parent_of_node\n> Проблемы обнаружены при обработке функции is_node_external, запускающей функцию get_parent_of_node.\n");
		return -1;
	}
	if ((isLeftChild = is_node_a_left_child(pParent, pGrandParent)) == -1) {
		err_msg("> <?!>\n> is_node_external -> is_node_a_left_child\n> Проблемы обнаружены при обработке функции is_node_external, запускающей функцию is_node_a_left_child.\n");
		return -1;
	}
	if (isLeftChild == 1) {
		if ((isLeftChild = is_node_a_left_child(pNode, pParent)) == -1) {
			err_msg("> <?!>\n> is_node_external -> is_node_a_left_child\n> Проблемы обнаружены при обработке функции is_node_external, запускающей функцию is_node_a_left_child.\n");
			return -1;
		}
		if (isLeftChild == 1) {
			return 1;
		} else {
			return 0;
		}
		
	} else {
		if ((isLeftChild = is_node_a_left_child(pNode, pParent)) == -1) {
			err_msg("> <?!>\n> is_node_external -> is_node_a_left_child\n> Проблемы обнаружены при обработке функции is_node_external, запускающей функцию is_node_a_left_child.\n");
			return -1;
		}
		if (isLeftChild == 1) {
			return 0;
		} else {
			return 1;
		}
	}
}

/*
* Левый поворот поддерева с корнем в вершине A.                         
*      |         |         |                                            
*      A         |         B                                            
*     / \        |        / \                                           
*    /   \       |       /   \                                          
*   a1    B      |      A     b2                                        
*        / \     |     / \                                              
*       /   \    |    /   \                                             
*      b1   b2   |   a1   b1                                            
*/
static int left_turn(RBT *pRBT, RBTptr *pNode) {
	RBTptr pNewNode;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция left_turn\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (pNode == NULL) {
		err_msg("> <?!>\n> Функция left_turn\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	pNewNode = (*pNode)->right;
	(*pNode)->right = pNewNode->left;
	pNewNode->left = *pNode;
	if (*pNode != pRBT->root) {
		if (transplant(pRBT, *pNode, pNewNode) == -1) {
			err_msg("> <?!>\n> left_turn -> transplant\n> Проблемы обнаружены при обработке функции left_turn, запускающей функцию transplant.\n");
			return -1;
		}
	} else {
		pRBT->root = pNewNode;
	}
	return 1;
}

/*
* Правый поворот поддерева с корнем в вершине A.                        
*         |      |      |                                               
*         A      |      B                                               
*        / \     |     / \                                              
*       /   \    |    /   \                                             
*      B    a1   |   b1    A                                            
*     / \        |        / \                                           
*    /   \       |       /   \                                          
*   b1   b2      |      b2   a1                                         
*/
static int right_turn(RBT *pRBT, RBTptr *pNode) {
	RBTptr pNewNode;
	
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция right_turn\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (*pNode == NULL) {
		err_msg("> <?!>\n> Функция right_turn\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	pNewNode = (*pNode)->left;
	(*pNode)->left = pNewNode->right;
	pNewNode->right = *pNode;
	if (*pNode != pRBT->root) {
		if (transplant(pRBT, *pNode, pNewNode) == -1) {
			err_msg("> <?!>\n> right_turn -> transplant\n> Проблемы обнаружены при обработке функции right_turn, запускающей функцию transplant.\n");
			return -1;
		}
	} else {
		pRBT->root = pNewNode;
	}
	return 1;
}

/*
* Большой левый поворот поддерева с корнем в вершине A осуществляется че
* ерез последовательное применение правого поворота для вершин B и C, а 
* затем левого поворота полученного поддерева относительно вершины A.   
*      |         |      |            |             C                    
*      A         |      A            |            / \                   
*     / \        |     / \           |           /   \                  
*    /   \       |    /   \          |          /     \                 
*   a1    B      |   a1    C         |         /       \                
*        / \     |        / \        |        /         \               
*       /   \    |       /   \       |       A           B              
*      C     b   |      c1    B      |      / \         / \             
*     / \        |           / \     |     /   \       /   \            
*    /   \       |          /   \    |    /     \     /     \           
*   c1   c2      |         c2    b   |   a1     c1   c2      b          
*/
static int left_long_turn(RBT *pRBT, RBTptr *pNode) {
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция left_long_turn\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (*pNode == NULL) {
		err_msg("> <?!>\n> Функция left_long_turn\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (right_turn(pRBT, &((*pNode)->right)) == -1) {
		err_msg("> <?!>\n> left_long_turn -> right_turn\n> Проблемы обнаружены при обработке функции left_long_turn, запускающей функцию right_turn.\n");
		return -1;
	}
	if (left_turn(pRBT, pNode) == -1) {
		err_msg("> <?!>\n> left_long_turn -> left_turn\n> Проблемы обнаружены при обработке функции left_long_turn, запускающей функцию left_turn.\n");
		return -1;
	}
	return 1;
}

/*
* Большой правый поворот поддерева с корнем в вершине A осуществляется ч
* ерез последовательное применение левого поворота для вершин B и C, а з
* атем правого поворота полученного поддерева относительно вершины A.   
*         |      |            |      |             C                    
*         A      |            A      |            / \                   
*        / \     |           / \     |           /   \                  
*       /   \    |          /   \    |          /     \                 
*      B    a1   |         C    a1   |         /       \                
*     / \        |        / \        |        /         \               
*    /   \       |       /   \       |       B           A              
*   b     C      |      B    c2      |      / \         / \             
*        / \     |     / \           |     /   \       /   \            
*       /   \    |    /   \          |    /     \     /     \           
*      c1   c2   |   b    c1         |   b      c1   c2     a1          
*/
static int right_long_turn(RBT *pRBT, RBTptr *pNode) {
	if (pRBT == NULL) {
		err_msg("> <?!>\n> Функция right_long_turn\n> Дерево pRBT является пустым.\n");
		return -1;
	}
	if (*pNode == NULL) {
		err_msg("> <?!>\n> Функция right_long_turn\n> Проверяемая вершина pNode является пустой.\n");
		return -1;
	}
	if (left_turn(pRBT, &((*pNode)->left)) == -1) {
		err_msg("> <?!>\n> right_long_turn -> left_turn\n> Проблемы обнаружены при обработке функции right_long_turn, запускающей функцию left_turn.\n");
		return -1;
	}
	if (right_turn(pRBT, pNode) == -1) {
		err_msg("> <?!>\n> right_long_turn -> right_turn\n> Проблемы обнаружены при обработке функции right_long_turn, запускающей функцию right_turn.\n");
		return -1;
	}
	return 1;
}

/*
* Каждый раз добавляя узел в дерево, требуется проводить балансировку в 
* согласии с цветом соседних добавляемому узлу вершин. Существует три сл
* учая, требующих отдельного описания:
* 1. Когда брат родителя красный: выполняется простая перекраска родител
* я и его брата в черный цвет, а родителя родителя, если тот не является
* корнем дерева, в красный цвет.
* 2. Когда брат родителя черный: в зависимости от расположения добавляем
* ого узла и цвета брата родителя выбирается один из пары подслучаев. Ес
* ли узел является внешним, то выполняется левый или правый повороты. Ес
* ли узел внутренний, то требуется выполнить большие левый или правый по
* вороты.
*/
static int tree_balancing(RBT *pRBT, RBTptr pNode, RBTptr pParent) {
	RBTptr pGrandParent;
	RBTptr pUncle;
	
	while (is_node_red(pParent) == 1) {
		if (get_parent_of_node(pRBT, pParent, &pGrandParent) == -1) {
			return -1;
		}
		if (get_sibling_node(pRBT, pParent, &pUncle) == -1) {
			return -1;
		}
		if (is_node_red(pGrandParent) == 0) {
			if (is_node_red(pUncle) == 1) {
				pParent->color = pUncle->color = BLACK;
				if (pGrandParent != pRBT->root) {
					pGrandParent->color = RED;
				}
			} else {
				if (is_node_external(pRBT, pNode) == 1) {
					if (is_node_a_left_child(pParent, pGrandParent)) {
						right_turn(pRBT, &pGrandParent);
					} else {
						left_turn(pRBT, &pGrandParent);
					}
					node_color_change(pRBT, &pParent);
				} else {
					if (is_node_a_left_child(pParent, pGrandParent)) {
						right_long_turn(pRBT, &pGrandParent);
					} else {
						left_long_turn(pRBT, &pGrandParent);
					}
					node_color_change(pRBT, &pNode);
				}
				node_color_change(pRBT, &pGrandParent);
			}
			pNode = pGrandParent;
			get_parent_of_node(pRBT, pNode, &pParent);
		}
	}
	return 1;
}

/*
* 
*/
static void declare_a_new_node(RBT *pRBT, RBTptr *pRoot, RBTptr *pNode, int pKey) {
	*pNode = *pRoot = (RBTptr) Malloc(sizeof(RBTnode));
	(*pNode)->key = pKey;
	(*pNode)->left = (*pNode)->right = NULL;
	(*pNode)->color = RED;
	pRBT->size++;
}

/*
* 
*/
static void stick_node_to_tree(RBT *pRBT, RBTptr *pRoot, RBTptr pNode, int pKey) {
	RBTptr pParent;
	
	if (pNode == NULL) {
		declare_a_new_node(pRBT, pRoot, &pNode, pKey);
		pNode->color = BLACK;
	} else {
		while (1) {
			if (pKey < pNode->key) {
				if (pNode->left == NULL) {
					pParent = pNode;
					declare_a_new_node(pRBT, &pNode->left, &pNode, pKey);
					break;
				} else {
					pRoot = &pNode->left;
					pNode = pNode->left;
				}
			} else {
				if (pNode->right == NULL) {
					pParent = pNode;
					declare_a_new_node(pRBT, &pNode->right, &pNode, pKey);
					break;
				} else {
					pRoot = &pNode->right;
					pNode = pNode->right;
				}
			}
		}
		tree_balancing(pRBT, pNode, pParent);
	}
}

/*
 * 
 * */
void insert_node_into_RBT(RBT *pRBT, int pKey) {
	stick_node_to_tree(pRBT, &pRBT->root, pRBT->root, pKey);
}

/*
* Рекурсивная функция free_node помещает выделенную память под узел де
* рева в список повторно используемых областей. Указатель pNode может бы
* ть равным NULL и проверка, используемая внутри функции указателя, испо
* льзуется для  последовательное перемещения по поддереву с корнем в 
* pRoot в порядке левого обхода.
*/
static void free_node(RBTptr pRoot) {
	if (pRoot != NULL) {
		free_node(pRoot->left);
		free_node(pRoot->right);
		free(pRoot);
	}
}

/*
* Для очистки всего дерева pRBT используется функция delete_tree, которая за
* действует рекурсивную функцию free_node, в качестве параметра которо
* й передает корень дерева pRBT->root.
*/
void delete_tree(RBT *pRBT) {
	free_node(pRBT->root);
}

/*
* 
*/
static RBTptr get_minimal_node_in_subtree(RBTptr pNode) {
	if (pNode != NULL) {
		if (pNode->left != NULL) {
			return get_minimal_node_in_subtree(pNode->left);
		} else {
			return pNode;
		}
	} else {
		return NULL;
	}
}

/*
* 
*/
static RBTptr get_minimal_node(RBT *pRBT) {
	return get_minimal_node_in_subtree(pRBT->root);
}

/*
* 
*/
static void tree_adjustment(RBT *pRBT, RBTptr pBalancingNode) {
	RBTptr pParent;
	RBTptr pSafePositionNode;
	
	if (pBalancingNode != NULL) {
		get_parent_of_node(pRBT, pBalancingNode, &pParent);
		pSafePositionNode = (is_node_a_left_child(pBalancingNode, pParent) == 1) ? pParent->right : pParent->left;
		while (pSafePositionNode != pRBT->root && !is_node_red(pSafePositionNode)) {
			if (!is_node_a_left_child(pBalancingNode, pParent)) {
				if (is_node_red(pBalancingNode)) {
					pBalancingNode->color = BLACK;
					pParent->color = RED;
					left_turn(pRBT, &pParent);
					pBalancingNode = pParent->right;
				} else {
					if (!is_node_red(pBalancingNode->left) && !is_node_red(pBalancingNode->right)) {
						pBalancingNode->color = RED;
						pSafePositionNode = pParent;
					} else if (is_node_red(pBalancingNode->left) && !is_node_red(pBalancingNode->right)) {
						(pBalancingNode->left)->color = BLACK;
						pBalancingNode->color = RED;
						right_turn(pRBT, &pBalancingNode);
						pBalancingNode = pParent->right;
					} else if (!is_node_red(pBalancingNode->left) && is_node_red(pBalancingNode->right)) {
						pBalancingNode->color = pParent->color;
						pParent->color = BLACK;
						(pBalancingNode->right)->color = BLACK;
						left_turn(pRBT, &pParent);
						pSafePositionNode = pRBT->root;
					}
				}
			} else {
				if (is_node_red(pBalancingNode)) {
					pBalancingNode->color = BLACK;
					pParent->color = RED;
					right_turn(pRBT, &pParent);
					pBalancingNode = pParent->left;
				} else {
					if (!is_node_red(pBalancingNode->left) && !is_node_red(pBalancingNode->right)) {
						pBalancingNode->color = RED;
						pSafePositionNode = pParent;
					} else if (!is_node_red(pBalancingNode->left) && is_node_red(pBalancingNode->right)) {
						(pBalancingNode->right)->color = BLACK;
						pBalancingNode->color = RED;
						left_turn(pRBT, &pBalancingNode);
						pBalancingNode = pParent->left;
					} else if (is_node_red(pBalancingNode->left) && !is_node_red(pBalancingNode->right)) {
						pBalancingNode->color = pParent->color;
						pParent->color = BLACK;
						(pBalancingNode->left)->color = BLACK;
						right_turn(pRBT, &pParent);
						pSafePositionNode = pRBT->root;
					}
				}
			}
		}
		pSafePositionNode->color = BLACK;
	}
}

/*
* Случай, когда удаляется лист дерева.
*/
static RBTptr case_A(RBT *pRBT, RBTptr *pDeletedNode) {
	RBTptr pBalancingNode;
	
	get_sibling_node(pRBT, *pDeletedNode, &pBalancingNode);
	transplant(pRBT, *pDeletedNode, NULL);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с одним потомком.
*/
static RBTptr case_B(RBT *pRBT, RBTptr *pDeletedNode) {
	RBTptr pNewNode;
	RBTptr pBalancingNode;
	
	get_sibling_node(pRBT, *pDeletedNode, &pBalancingNode);
	if ((*pDeletedNode)->left != NULL) {
		pNewNode = (*pDeletedNode)->left;
	} else {
		pNewNode = (*pDeletedNode)->right;
	}
	transplant(pRBT, *pDeletedNode, pNewNode);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с двумя потомками.
*/
static RBTptr case_C(RBT *pRBT, RBTptr *pDeletedNode) {
	RBTptr pMinimalNodeFromRightSubtreeOfDeletedNode;
	RBTptr pParentOfMinimalNodeFromRightSubtreeOfDeletedNode;
	RBTptr pBalancingNode;
	
	pMinimalNodeFromRightSubtreeOfDeletedNode = get_minimal_node_in_subtree((*pDeletedNode)->right);
	get_parent_of_node(pRBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pParentOfMinimalNodeFromRightSubtreeOfDeletedNode);
	get_sibling_node(pRBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pBalancingNode);
	
	if (pParentOfMinimalNodeFromRightSubtreeOfDeletedNode != *pDeletedNode) {
		pParentOfMinimalNodeFromRightSubtreeOfDeletedNode->left = pMinimalNodeFromRightSubtreeOfDeletedNode->right;
	}
	if (pMinimalNodeFromRightSubtreeOfDeletedNode != (*pDeletedNode)->right) {
		pMinimalNodeFromRightSubtreeOfDeletedNode->right = (*pDeletedNode)->right;
	}
	pMinimalNodeFromRightSubtreeOfDeletedNode->left = (*pDeletedNode)->left;
	transplant(pRBT, *pDeletedNode, pMinimalNodeFromRightSubtreeOfDeletedNode);
	return pBalancingNode;
}

void delete_node(RBT *pRBT, RBTptr pNode) {
	int originColor;
	RBTptr pBalancingNode;
	
	if (pNode != NULL) {
		pRBT->size--;
		if (pNode->right == NULL && pNode->left == NULL) {
			pBalancingNode = case_A(pRBT, &pNode);
		} else if (pNode->right == NULL || pNode->left == NULL) {
			pBalancingNode = case_B(pRBT, &pNode);
		} else {
			pBalancingNode = case_C(pRBT, &pNode);
		}
		tree_adjustment(pRBT, pBalancingNode);
		free(pNode);
	}
}

/*
* 
*/
static RBTptr get_node_by_key_in_subtree(RBTptr pNode, int pKey) {
	if (pNode != NULL) {
		if (pKey < pNode->key) {
			return get_node_by_key_in_subtree(pNode->left, pKey);
		} else if (pKey > pNode->key) {
			return get_node_by_key_in_subtree(pNode->right, pKey);
		} else {
			return pNode;
		}
	} else {
		return NULL;
	}
}

/*
* 
*/
RBTptr get_node_by_key(RBT *pRBT, int pKey) {
	return get_node_by_key_in_subtree(pRBT->root, pKey);
}
