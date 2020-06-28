#pragma region Library
#include <iostream>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<time.h>
#include<vector>
#include<math.h>
#pragma endregion

#pragma region NODE
using namespace std;
enum class Actions { left, right, up, down };
#define SIZE 3  
#define N 9
#ifndef _CLOCK_T
#define _CLOCK_T
#endif 

int foundFirst = 0;
int unfoundFirst = 0;
int foundHill = 0;
int unfoundHill = 0;
int foundSimulated = 0;
int unfoundSimulated = 0;
int stepHill = 0;
int stepFirst = 0;
int stepSimulated = 0;

typedef struct Node_array
{
	int state[SIZE][SIZE];
	Actions action;
	struct Node_array* parent;
	int path_cost;
	int depth;
	struct Node_array* nextNode;
} Node;

struct NodeList
{
	unsigned int nodeCount;
	Node* head;
	Node* tail;
};

Node* Goal;
Node* Start;

unsigned int CountCellWarnning(Node* current, Node* Goal)
{
	unsigned int count = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (current->state[i][j] != Goal->state[i][j])
				count++;
		}
	}
	return count;
}

void copy_array(int a[][SIZE], int b[][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			a[i][j] = b[i][j];
}
void copy_array1(int A[N], int B[N])
{
	for (int i = 0; i < N; i++)
	{
		A[i] = B[i];
	}
}

int is_equal(int a[][SIZE], int b[][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			if (a[i][j] != b[i][j])
			{
				return 0;
			}

	return 1;
}

void swap(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}

Node* doiNode(int A[])
{
	Node* node = (Node*)malloc(sizeof(Node));
	int i, j;
	int m = 0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
		{
			node->state[i][j] = A[m];
			m++;
		}
	return node;
}

void print_array(int a[][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
			if (a[i][j] != 0)
				printf("%d  | ", a[i][j]);
			else printf("   | ");
		printf("\n");
		for (j = 0; j < SIZE; j++)
			printf("---|-");

		printf("\n");
	}
	printf("\n");
}

bool isLegalAction(Node* node, Actions action, int& index_i, int& index_j) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (node->state[i][j] == 0) {
				index_i = i;
				index_j = j;
				i = SIZE;
				break;
			}
		}
	}
	if (action == Actions::left && index_j == 0)  return false;
	if (action == Actions::right && index_j == 2)  return false;

	switch (index_i)
	{
	case 0:
		if (action == Actions::up) return false;
		return true;
	case 2:
		if (action == Actions::down) return false;
		return true;
	default:
		return true;
	}
	return true;
}

Node* Child_node(Node* node, Actions action)
{
	int index_i, index_j;
	Node* current = (Node*)malloc(sizeof(Node));

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (node->state[i][j] == 0) {
				index_i = i;
				index_j = j;
				i = SIZE;
				break;
			}
		}
	}
	if (isLegalAction(node, action, index_i, index_j) == true)
	{
		copy_array(current->state, node->state);
		current->action = action;
		current->nextNode = NULL;
		current->parent = node;
		current->depth = node->depth + 1;

		if (action == Actions::left)
		{
			swap(current->state[index_i][index_j], current->state[index_i][index_j - 1]);
		}
		else if (action == Actions::right)
		{
			swap(current->state[index_i][index_j], current->state[index_i][index_j + 1]);
		}
		else if (action == Actions::up)
		{
			swap(current->state[index_i][index_j], current->state[index_i - 1][index_j]);
		}
		else if (action == Actions::down)
		{
			swap(current->state[index_i][index_j], current->state[index_i + 1][index_j]);
		}
		current->path_cost = node->depth + 1;
	}
	else return NULL;

	return current;
}

NodeList* FIFO_initial()
{
	NodeList* list = (NodeList*)malloc(sizeof(NodeList));
	list->head = NULL;
	list->tail = NULL;
	list->nodeCount = 0;
	return list;
}

void FIFO_add(NodeList* list, Node* node)
{
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
		list->nodeCount++;
	}
	else
	{
		list->tail->nextNode = node;
		list->tail = node;
		list->nodeCount++;
	}
}

Node* LIFO_pop(NodeList* list)
{
	Node* current = NULL;
	if (list->head == NULL)
		return NULL;
	else {
		if (list->head->nextNode == NULL)
		{
			current = list->head;
			list->head = current->nextNode;
			list->nodeCount = 0;
			list->head = list->tail = NULL;


		}
		else
		{
			current = list->tail;
			Node* p = list->head;
			while (p->nextNode != current)
			{
				p = p->nextNode;
			}
			list->tail = p;
			list->tail->nextNode = NULL;
			list->nodeCount--;
		}


	}
	return current;
}

Node* FIFO_pop(NodeList* list)
{
	Node* current = NULL;
	if (list->head == NULL)
		return NULL;
	else {
		if (list->head->nextNode == NULL)
		{
			current = list->head;
			list->head = current->nextNode;
			list->nodeCount = 0;
			list->head = list->tail = NULL;


		}
		else
		{
			current = list->head;
			list->head = current->nextNode;
			list->nodeCount--;
		}
	}
	return current;
}

bool Goal_test(Node* node, Node* goal)
{
	return is_equal(node->state, goal->state);
}

bool checkExist(NodeList* list, Node* node)
{
	if (list->head != NULL)
	{
		Node* pzl = list->head;
		while (pzl != NULL)
		{
			if (is_equal(pzl->state, node->state))
			{
				return true;
			}
			pzl = pzl->nextNode;
		}
	}
	return false;
}

Node* createNode(Node* current)
{
	Node* pzl = (Node*)malloc(sizeof(Node));

	copy_array(pzl->state, current->state);
	pzl->action = current->action;
	pzl->depth = current->depth;
	pzl->nextNode = current->nextNode;
	pzl->parent = current->parent;
	pzl->path_cost = current->path_cost;
	return pzl;
}

int Steps(Node* current)
{
	NodeList* List = FIFO_initial();
	while (current->parent != NULL)
	{
		FIFO_add(List, current);
		current = current->parent;
	}
	return List->nodeCount;
}

int value(Node* start, Node* goal)
{
	int count = 0;
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
		{
			if (start->state[i][j] > 0)
			{
				for (int i1 = 0; i1 < SIZE; i1++)
					for (int j1 = 0; j1 < SIZE; j1++)
						if (goal->state[i1][j1] == start->state[i][j])
							count += abs(i - i1) + abs(j - j1);
			}
		}
	return count;
}

int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

Node* move(Node* check, Node* Goal)
{
	NodeList* now = FIFO_initial();
	Node* Up = (Node*)malloc(sizeof(Node));
	Node* Down = (Node*)malloc(sizeof(Node));
	Node* Left = (Node*)malloc(sizeof(Node));
	Node* Right = (Node*)malloc(sizeof(Node));

	Up = Child_node(check, Actions::up);
	Down = Child_node(check, Actions::down);
	Left = Child_node(check, Actions::left);
	Right = Child_node(check, Actions::right);
	int res, res1;

	if (Up == NULL)
	{
		res = value(Down, Goal);
	}
	else if (Down == NULL)
	{
		res = value(Up, Goal);
	}
	else if (Up == NULL && Down == NULL)
	{
		res = 9999;
	}
	else
	{
		int kq = value(Up, Goal);
		int kq1 = value(Down, Goal);
		res = min(kq, kq1);
	}
	if (Left == NULL)
	{
		res1 = value(Right, Goal);
	}
	else if (Right == NULL)
	{
		res1 = value(Left, Goal);
	}
	else if (Left == NULL && Right == NULL)
	{
		res1 = 9999;
	}
	else
	{
		int kq = value(Left, Goal);
		int kq1 = value(Right, Goal);
		res1 = min(kq, kq1);
	}
	int result = min(res, res1);

	if (Up != NULL && value(Up, Goal) == result)
	{
		return Up;
	}
	if (Down != NULL && value(Down, Goal) == result)
	{
		return Down;
	}
	if (Left != NULL && value(Left, Goal) == result)
	{
		return Left;
	}
	if (Right != NULL && value(Right, Goal) == result)
	{
		return Right;
	}
}
void Solution(Node* node)
{
	NodeList* list = FIFO_initial();
	while (node->parent != NULL)
	{
		FIFO_add(list, node);
		node = node->parent;
	}
	while (list->head != NULL)
	{
		Node* pop = LIFO_pop(list);
		print_array(pop->state);
	}
}
#pragma endregion

#pragma region ALGORITHM
void HILL_CLIMBING(Node* Start, Node* Goal)
{
	Node* Current = Start;
	Current->parent = NULL;
	Node* Neighbor = (Node*)malloc(sizeof(Node));;
	do
	{
		Neighbor = move(Current, Goal);
		if (value(Neighbor, Goal) >= value(Current, Goal))
		{
			if (Goal_test(Current, Goal) == true)
			{
				foundHill++;
			}
			else
			{
				stepHill += Steps(Current);
				unfoundHill++;
			}
			return;
		}
		Current = Neighbor;
	} while (true);
}

Node* random_Firstchoice(Node* start, Node* goal)
{
	Actions action;
	Node* current = start;
	Node* Neighbor = (Node*)malloc(sizeof(Node));
	int count = 0;
	srand(time(NULL));
	do
	{
		do
		{
			action = Actions(rand() % 4);
			Neighbor = Child_node(current, action);
		} while (Neighbor == NULL);
		count++;
		if (value(Neighbor, Goal) <= value(current, Goal))
			return Neighbor;
		if (count == 10)
			return NULL;
	} while (true);
}

void FIRST_CHOICE_HILL_CLIMBING(Node* Start, Node* Goal)
{
	Node* Current = Start;
	Actions action;
	Node* Neighbor = (Node*)malloc(sizeof(Node));
	srand(time(NULL));
	do
	{
		Neighbor = random_Firstchoice(Current, Goal);
		if (Neighbor == NULL || value(Neighbor, Goal) >= value(Current, Goal))
			break;
		Current = Neighbor;
	} while (true);
	if (Goal_test(Current, Goal) == true)
	{
		foundFirst++;
	}
	else
	{
		stepFirst += Steps(Current);
		unfoundFirst++;
	}
}

void RANDOM_RESTART_HILL_CLIMBING(Node* Start, Node* Goal)
{
	srand(time(NULL));
	int A[N];
	int i;
	int y;
	Node* goal;
	do
	{
		for (i = 0; i < N; i++)
		{
			A[i] = rand() % 9;
			for (int j = i - 1; j >= 0; j--)
			{
				if (A[i] != A[j])
					continue;
				else
				{

					A[i] = rand() % 9;
					j = i;
				}
			}
		}
		Start = doiNode(A);
		HILL_CLIMBING(Start, Goal);
		y = value(Start, Goal);
	} while (y > 0);
}

Node* Random(Node* Start)
{
	srand(time(NULL));
	Node* Neighbor = (Node*)malloc(sizeof(Node));
	Actions action;
	do
	{
		action = Actions(rand() % 4);
		Neighbor = Child_node(Start, action);
	} while (Neighbor == NULL);
	return Neighbor;
}

void SIMULATED_ANNEALING(Node* Start, Node* Goal, float* schedule)
{
	srand(time(NULL));
	Node* Current = (Node*)malloc(sizeof(Node));
	Node* Neighbor = (Node*)malloc(sizeof(Node));
	Current = Start;
	int delTaE;
	int t = 0;
	float T;
	float P;
	while (Goal_test(Current, Goal) == false)
	{
		if (t > 1000)
			break;
		T = (t + 1000) / (1000 * t + 2);
		Neighbor = Random(Current);
		delTaE = value(Current, Goal) - value(Neighbor, Goal);
		if (delTaE > 0)
		{
			Current = Neighbor;
		}
		else
		{
			P = exp(delTaE / T);
			if (1.0 * rand() / RAND_MAX < P)
			{
				Current = Neighbor;
			}
		}
		t++;
	}
	if (Goal_test(Current, Goal) == true)
	{
		foundSimulated++;
	}
	else
	{
		stepSimulated += Steps(Current);
		unfoundSimulated++;
	}
}
#pragma endregion

int* RanSteps(Node* Goal, int Steps)
{
	int G[9];
	int m = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			G[m] = Goal->state[i][j];
			m++;
		}

	int* A = new int[9];
	for (int i = 0; i < 9; i++)
	{
		A[i] = G[i];
	}
	int step = 0;
	do
	{
		int index = -1;
		for (int i = 0; i < 9; i++)
		{
			if (A[i] == 0)
			{
				index = i;
				break;
			}
		}
		int k;
		do
		{
			k = rand() % 4;
			if (k == 0)
			{
				if (index % 3 != 2)
				{
					swap(A[index], A[index + 1]);
					break;
				}

			}
			else if (k == 1)
			{
				if (index % 3 != 0)
				{
					swap(A[index], A[index - 1]);
					break;
				}
			}
			else if (k == 2)
			{
				if (index / 3 != 2)
				{
					swap(A[index], A[index + 3]);
					break;
				}

			}
			else {
				if (index / 3 != 0)
				{
					swap(A[index], A[index - 3]);
					break;
				}
			}
		} while (true);
		step++;
	} while (step < Steps);
	return A;
}

int** RanArrayGoal(Node* Goal)
{
	srand(time(NULL));
	int** Start;
	int n = 10000;
	Start = new int* [n];
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (Start[i] == Start[j])
			{
				Start[i] = new int[N];
				Start[i] = RanSteps(Goal, 30);
			}
		}
	}
	return Start;
}

#pragma region MENU
void Menu()
{
	cout << "\nPROJECT ARTIFICIAL INTELLIGENCE\n";
	cout << "----------------------------------------\n";
	cout << "1. So sanh 3 thuat toan\n";
	cout << "2. Exit\n";
	cout << "----------------------------------------\n";
}

int Choice()
{
	int choose = 0;
	cout << "Choose the algorithm to use: ";
	cin >> choose;
	if (choose > 0 || choose < 3)
	{
		return choose;
	}
	else
		return Choice();
}

void Use()
{
	int G[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	Goal = (Node*)malloc(sizeof(Node));
	float schedule[1000];
	int T = 100;
	schedule[0] = T;
	clock_t start, end;
	double time;
	Goal = doiNode(G);
	int** st = RanArrayGoal(Goal);
	int choose = Choice();
	switch (choose)
	{
	case 1:
	{
		int startValue = 0;
		int endValue;
		foundHill = 0;
		unfoundHill = 0;
		foundFirst = 0;
		unfoundFirst = 0;
		foundSimulated = 0;
		unfoundSimulated = 0;
		stepFirst = 0;
		stepHill = 0;
		stepSimulated = 0;
		cout << "Nhap so du lieu dau vao: ";
		cin >> endValue;
		std::cout << "\n";
		start = clock();
		int i = 0;
		do
		{
			Node* Start = (Node*)malloc(sizeof(Node));
			Start = doiNode(st[i]);
			HILL_CLIMBING(Start, Goal);
			FIRST_CHOICE_HILL_CLIMBING(Start, Goal);
			SIMULATED_ANNEALING(Start, Goal, schedule);
			startValue++;
			i++;
		} while (startValue < endValue);
		end = clock();
		time = ((double)(end - start)) / CLOCKS_PER_SEC;
		cout << "HILL CLIMBING\n";
		cout << "FOUNDED: " << foundHill << endl;
		cout << "NOT FOUND: " << unfoundHill << endl;
		cout << "STEPS: " << stepHill << "\n" << endl;
		cout << "FIRST CHOICE HILL CLIMBING\n";
		cout << "FOUNDED: " << foundFirst << endl;
		cout << "NOT FOUND: " << unfoundFirst << endl;
		cout << "STEPS: " << stepFirst << "\n" << endl;
		cout << "SIMULATED-ANNEALING\n";
		cout << "FOUNDED: " << foundSimulated << endl;
		cout << "NOT FOUND: " << unfoundSimulated << endl;
		cout << "STEPS: " << stepSimulated << "\n" << endl;
		break;
	}
	case 2:
	{
		exit(0);
		break;
	}
	}
}
#pragma endregion

#pragma region MAIN
int main()
{
	Menu();
	while (true)
	{
		Use();
		std::cout << "\n";
		Menu();
	}
	return 0;
}
#pragma endregion