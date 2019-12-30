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
* щает значение TRUE, а если узел - черный, то FALSE. Эта функция будет 
* полезна, когда мы будем обращаться к проверке цвета узла при удалении,
* либо добавлении вершины в красно-черное дерево. Если узел пустой NULL,
* то возвращается значение FALSE. Это сделано из расчета, что пустой узе
* л по-умолчанию должен быть черным.
*/
static int is_node_red(RBTptr pNode) {
	if (pNode == NULL) {
		return FALSE;
	}
	return (pNode->color == RED) ? TRUE : FALSE;
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
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pKey = pNode->key;
	*pParent = pRBT->root;
	while (*pParent != NULL) {
		if ((*pParent)->right == pNode || (*pParent)->left == pNode) {
			return TRUE;
		}
		if ((*pParent)->key > pKey) {
			*pParent = (*pParent)->left;
		} else if ((*pParent)->key < pKey) {
			*pParent = (*pParent)->right;
		} else {
			break;
		}
	}
	return FALSE;
}

static void Get_parent_of_node(RBT *pRBT, RBTptr pNode, RBTptr *pParent) {
	int RETURN;
	
	RETURN = get_parent_of_node(pRBT, pNode, pParent);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_parent_of_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_parent_of_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Функция проверки на то, что узел pNode является левым, либо правым пот
* омком в узла pParent. Если pParent не является родителем pNode, то фун
* кция возвращает отрицательное значение.
*/
static int is_node_a_left_child(RBTptr pNode, RBTptr pParent) {
	if (pNode == NULL || pParent == NULL) {
		return ER_EMPTYNODE;
	}
	if (pParent->left == pNode) {
		return TRUE;
	} else if (pParent->right == pNode) {
		return FALSE;
	} else {
		return ER_UNKNWPRNT;
	} 
}

static int Is_node_a_left_child(RBTptr pNode, RBTptr pParent) {
	int RETURN;
	
	RETURN = is_node_a_left_child(pNode, pParent);
	switch (RETURN) {
		case ER_EMPTYNODE:
			err_msg("is_node_a_left_child: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
		case ER_UNKNWPRNT:
			err_msg("is_node_a_left_child: ER_UNKNWPRNT\n");
			exit(EXIT_FAILURE);
		default:
			return RETURN;
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
	
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	if (pRBT->root != pNode) {
		Get_parent_of_node(pRBT, pNode, &pParent);
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			*pNodeBrother = pParent->right;
		} else {
			*pNodeBrother = pParent->left;
		}
	} else {
		*pNodeBrother = NULL;
	}
	return TRUE;
}

static void Get_sibling_node(RBT *pRBT, RBTptr pNode, RBTptr *pNodeBrother) {
	int RETURN;
	
	RETURN = get_sibling_node(pRBT, pNode, pNodeBrother);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_sibling_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_sibling_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Функция возвращает вершину с минимальным ключом в поддереве с корнем в
* A. Архитектура дерева такова, что минимальная вершина всегда будет леж
* ать слева от корня. Для дерева приведенного ниже, такой вершиной будет
* узел B.
*      |
*      A
*     / \
*    /   \
*   B     C
*/
static int get_minimal_node(RBTptr pRoot, RBTptr *pMinimalNode) {
	if (pRoot == NULL) {
		return ER_EMPTYNODE;
	}
	*pMinimalNode = pRoot;
	while (*pMinimalNode != NULL) {
		if ((*pMinimalNode)->left == NULL) {
			return TRUE;
		}
		*pMinimalNode = (*pMinimalNode)->left;
	}
	return ER_EMPTYNODE;
}

static void Get_minimal_node(RBTptr pRoot, RBTptr *pNodeBrother) {
	int RETURN;
	
	RETURN = get_minimal_node(pRoot, pNodeBrother);
	switch (RETURN) {
		case ER_EMPTYNODE:
			err_msg("get_sibling_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* 
*/
static int get_node(RBT *pRBT, int pKey, RBTptr *pNode) {
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	*pNode = pRBT->root;
	while (*pNode != NULL) {
		if ((*pNode)->key > pKey) {
			*pNode = (*pNode)->left;
		} else if ((*pNode)->key < pKey) {
			*pNode = (*pNode)->right;
		} else if ((*pNode)->key == pKey) {
			return TRUE;
		} else {
			break;
		}
	}
	return ER_EMPTYNODE;
}

void Get_node(RBT *pRBT, int pKey, RBTptr *pNode) {
	int RETURN;
	
	RETURN = get_node(pRBT, pKey, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_node: ER_EMPTYNODE\n");
			*pNode == NULL;
	}
}

/*
* Заменяет поддерево с корнем в узле pNode поддеревом с корнем в узле   
* pNewNode. Родителю узла pNode устанавливается новый левый или правый п
* отомок. Если содержимое дерева заменяется полностью, то корень pRBT пр
* инимает новое значение корневого узла pNewNode.                       
*/
static int transplant(RBT *pRBT, RBTptr pNode, RBTptr pNewNode) {
	RBTptr pParent;
	
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	if (pRBT->root != pNode) {
		Get_parent_of_node(pRBT, pNode, &pParent);
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			pParent->left = pNewNode;
		} else {
			pParent->right = pNewNode;
		}
	} else {
		pRBT->root = pNewNode;
	}
	return TRUE;
}

static void Transplant(RBT *pRBT, RBTptr pNode, RBTptr pNewNode) {
	int RETURN;
	
	RETURN = transplant(pRBT, pNode, pNewNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("transplant: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("transplant: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
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
	RBTptr pParent;
	RBTptr pGrandParent;
	
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Get_parent_of_node(pRBT, pNode, &pParent);
	Get_parent_of_node(pRBT, pParent, &pGrandParent);
	if (Is_node_a_left_child(pParent, pGrandParent) == TRUE) {
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
}

static int Is_node_external(RBT *pRBT, RBTptr pNode) {
	int RETURN;
	
	RETURN = is_node_external(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("is_node_external: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("is_node_external: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
		default:
			return RETURN;
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
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pNewNode = (*pNode)->right;
	(*pNode)->right = pNewNode->left;
	pNewNode->left = *pNode;
	if (*pNode != pRBT->root) {
		Transplant(pRBT, *pNode, pNewNode);
	} else {
		pRBT->root = pNewNode;
	}
	return TRUE;
}

static void Left_turn(RBT *pRBT, RBTptr *pNode) {
	int RETURN;
	
	RETURN = left_turn(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("left_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("left_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
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
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pNewNode = (*pNode)->left;
	(*pNode)->left = pNewNode->right;
	pNewNode->right = *pNode;
	if (*pNode != pRBT->root) {
		Transplant(pRBT, *pNode, pNewNode);
	} else {
		pRBT->root = pNewNode;
	}
	return TRUE;
}

static void Right_turn(RBT *pRBT, RBTptr *pNode) {
	int RETURN;
	
	RETURN = right_turn(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("right_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("right_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
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
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Right_turn(pRBT, &((*pNode)->right));
	Left_turn(pRBT, pNode);
	return TRUE;
}

static void Left_long_turn(RBT *pRBT, RBTptr *pNode) {
	int RETURN;
	
	RETURN = left_long_turn(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("left_long_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("left_long_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
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
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Left_turn(pRBT, &((*pNode)->left));
	Right_turn(pRBT, pNode);
	return TRUE;
}

static void Right_long_turn(RBT *pRBT, RBTptr *pNode) {
	int RETURN;
	
	RETURN = right_long_turn(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("right_long_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("right_long_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Функция смены цвета узла.
*/
static int node_color_change(RBT *pRBT, RBTptr *pNode) {
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	if (*pNode != pRBT->root) {
		if (is_node_red(*pNode) == TRUE) {
			(*pNode)->color = BLACK;
		} else {
			(*pNode)->color = RED;
		}
	} else {
		(*pNode)->color = BLACK;
	}
	return TRUE;
}

static int Node_color_change(RBT *pRBT, RBTptr *pNode) {
	int RETURN;
	
	RETURN = node_color_change(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("node_color_change: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("node_color_change: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
		default:
			return RETURN;
	}
}

/*
* Каждый раз добавляя узел в дерево, требуется проводить балансировку в 
* согласии с цветом соседних добавляемому узлу вершин. Существует три сл
* учая, требующих отдельного описания:
* TRUE. Когда брат родителя красный: выполняется простая перекраска родител
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
	
	while (is_node_red(pParent) == TRUE) {
		Get_parent_of_node(pRBT, pParent, &pGrandParent);
		Get_sibling_node(pRBT, pParent, &pUncle);
		if (is_node_red(pGrandParent) == FALSE) {
			if (is_node_red(pUncle) == TRUE) {
				pParent->color = pUncle->color = BLACK;
				if (pGrandParent != pRBT->root) {
					pGrandParent->color = RED;
				}
			} else {
				if (is_node_external(pRBT, pNode) == TRUE) {
					if (Is_node_a_left_child(pParent, pGrandParent) == TRUE) {
						Right_turn(pRBT, &pGrandParent);
					} else {
						Left_turn(pRBT, &pGrandParent);
					}
					Node_color_change(pRBT, &pParent);
				} else {
					if (Is_node_a_left_child(pParent, pGrandParent) == TRUE) {
						Right_long_turn(pRBT, &pGrandParent);
					} else {
						Left_long_turn(pRBT, &pGrandParent);
					}
					Node_color_change(pRBT, &pNode);
				}
				Node_color_change(pRBT, &pGrandParent);
			}
			pNode = pGrandParent;
			Get_parent_of_node(pRBT, pNode, &pParent);
		}
	}
	return TRUE;
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
static void insert_node(RBT *pRBT, RBTptr *pRoot, RBTptr pNode, int pKey) {
	RBTptr pParent;
	
	if (pNode == NULL) {
		declare_a_new_node(pRBT, pRoot, &pNode, pKey);
		pNode->color = BLACK;
	} else {
		while (TRUE) {
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
void Insert_node(RBT *pRBT, int pKey) {
	insert_node(pRBT, &pRBT->root, pRBT->root, pKey);
}

/*
* Рекурсивная функция free_tree помещает выделенную память под узел де
* рева в список повторно используемых областей. Указатель pNode может бы
* ть равным NULL и проверка, используемая внутри функции указателя, испо
* льзуется для  последовательное перемещения по поддереву с корнем в 
* pRoot в порядке левого обхода.
*/
static void free_tree(RBTptr pRoot) {
	if (pRoot != NULL) {
		free_tree(pRoot->left);
		free_tree(pRoot->right);
		free(pRoot);
	}
}

/*
* Для очистки всего дерева pRBT используется функция Free_tree, которая за
* действует рекурсивную функцию free_tree, в качестве параметра которо
* й передает корень дерева pRBT->root.
*/
void Free_tree(RBT *pRBT) {
	free_tree(pRBT->root);
}

/*
* 
*/
static void tree_adjustment(RBT *pRBT, RBTptr pBalancingNode) {
	RBTptr pParent;
	RBTptr pSafePositionNode;
	
	if (pBalancingNode != NULL) {
		Get_parent_of_node(pRBT, pBalancingNode, &pParent);
		pSafePositionNode = (Is_node_a_left_child(pBalancingNode, pParent) == TRUE) ? pParent->right : pParent->left;
		while (pSafePositionNode != pRBT->root && !is_node_red(pSafePositionNode)) {
			if (Is_node_a_left_child(pBalancingNode, pParent) == TRUE) {
				if (is_node_red(pBalancingNode) == TRUE) {
					pBalancingNode->color = BLACK;
					pParent->color = RED;
					Right_turn(pRBT, &pParent);
					pBalancingNode = pParent->left;
				} else {
					if ((is_node_red(pBalancingNode->left) == FALSE) && (is_node_red(pBalancingNode->right) == FALSE)) {
						pBalancingNode->color = RED;
						pSafePositionNode = pParent;
					} else if ((is_node_red(pBalancingNode->left) == FALSE) && (is_node_red(pBalancingNode->right) == TRUE)) {
						(pBalancingNode->right)->color = BLACK;
						pBalancingNode->color = RED;
						Left_turn(pRBT, &pBalancingNode);
						pBalancingNode = pParent->left;
					} else if ((is_node_red(pBalancingNode->left) == TRUE) && (is_node_red(pBalancingNode->right) == FALSE)) {
						pBalancingNode->color = pParent->color;
						pParent->color = BLACK;
						(pBalancingNode->left)->color = BLACK;
						Right_turn(pRBT, &pParent);
						pSafePositionNode = pRBT->root;
					}
				}
			} else {
				if (is_node_red(pBalancingNode) == TRUE) {
					pBalancingNode->color = BLACK;
					pParent->color = RED;
					Left_turn(pRBT, &pParent);
					pBalancingNode = pParent->right;
				} else {
					if ((is_node_red(pBalancingNode->left) == FALSE) && (is_node_red(pBalancingNode->right) == FALSE)) {
						pBalancingNode->color = RED;
						pSafePositionNode = pParent;
					} else if ((is_node_red(pBalancingNode->left) == TRUE) && (is_node_red(pBalancingNode->right) == FALSE)) {
						(pBalancingNode->left)->color = BLACK;
						pBalancingNode->color = RED;
						Right_turn(pRBT, &pBalancingNode);
						pBalancingNode = pParent->right;
					} else if ((is_node_red(pBalancingNode->left) == FALSE) && (is_node_red(pBalancingNode->right) == TRUE)) {
						pBalancingNode->color = pParent->color;
						pParent->color = BLACK;
						(pBalancingNode->right)->color = BLACK;
						Left_turn(pRBT, &pParent);
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
	
	Get_sibling_node(pRBT, *pDeletedNode, &pBalancingNode);
	Transplant(pRBT, *pDeletedNode, NULL);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с одним потомком.
*/
static RBTptr case_B(RBT *pRBT, RBTptr *pDeletedNode) {
	RBTptr pNewNode;
	RBTptr pBalancingNode;
	
	Get_sibling_node(pRBT, *pDeletedNode, &pBalancingNode);
	if ((*pDeletedNode)->left != NULL) {
		pNewNode = (*pDeletedNode)->left;
	} else {
		pNewNode = (*pDeletedNode)->right;
	}
	Transplant(pRBT, *pDeletedNode, pNewNode);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с двумя потомками.
*/
static RBTptr case_C(RBT *pRBT, RBTptr *pDeletedNode) {
	RBTptr pMinimalNodeFromRightSubtreeOfDeletedNode;
	RBTptr pParentOfMinimalNodeFromRightSubtreeOfDeletedNode;
	RBTptr pBalancingNode;
	
	Get_minimal_node((*pDeletedNode)->right, &pMinimalNodeFromRightSubtreeOfDeletedNode);
	Get_parent_of_node(pRBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pParentOfMinimalNodeFromRightSubtreeOfDeletedNode);
	Get_sibling_node(pRBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pBalancingNode);
	
	if (pParentOfMinimalNodeFromRightSubtreeOfDeletedNode != *pDeletedNode) {
		pParentOfMinimalNodeFromRightSubtreeOfDeletedNode->left = pMinimalNodeFromRightSubtreeOfDeletedNode->right;
	}
	if (pMinimalNodeFromRightSubtreeOfDeletedNode != (*pDeletedNode)->right) {
		pMinimalNodeFromRightSubtreeOfDeletedNode->right = (*pDeletedNode)->right;
	}
	pMinimalNodeFromRightSubtreeOfDeletedNode->left = (*pDeletedNode)->left;
	Transplant(pRBT, *pDeletedNode, pMinimalNodeFromRightSubtreeOfDeletedNode);
	return pBalancingNode;
}

static int free_node(RBT *pRBT, RBTptr pNode) {
	int originColor;
	RBTptr pBalancingNode;
	
	if (pRBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
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
	return TRUE;
}

void Free_node(RBT *pRBT, RBTptr pNode) {
	int RETURN;
	
	RETURN = free_node(pRBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("free_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("free_node: ER_EMPTYNODE\n");
	}
}
