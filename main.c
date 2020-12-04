/*
	아래에 주어진 선언과 p.289의 program 6.5와 p.290의 program6.6을 이용하며,
	bicon()에서 사용할 스택을 singly linked list로 구현한다.
*/
#define _CRT_SECURE_NO_WARNINGS
#define MAX_VERTICES 30
#define MIN2(x, y) ((x) < (y) ? (x) : (y))
#include <stdio.h>
#include <stdlib.h>

short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];

// p.18 adjacency list representation 
typedef struct node* node_pointer;
typedef struct node {
	int vertex;
	node_pointer link;
}node;

typedef struct edge* edge_pointer;
typedef struct edge {
	int u;
	int w;
	edge_pointer link;
}edge;

int num;
int n;
node_pointer* graph;
edge_pointer top;

node_pointer initNodePtr(int vertex);
edge_pointer initEdgePtr(int u, int w);

void init();
void getGraph(FILE* inputFile);
void pushNode(node_pointer head, node_pointer node);

void bicon(int u, int v);
void add(edge_pointer top, int x, int y);
void delete(edge_pointer top, int* x, int* y);

void readGraph();
void readList();

void main()
{
	// 파일 오픈하여 파일을 읽어 adjancey list에 저장한다.
	FILE* inputFile = fopen("input.txt", "r");

	getGraph(inputFile);
	readGraph();
	
	 init();
	 bicon(3, -1);
	 readList();

	 return;
}

node_pointer initNodePtr(int vertex) {
	node_pointer tempNode = malloc(sizeof(node));
	tempNode->link = NULL;
	tempNode->vertex = vertex;
	return tempNode;
}

edge_pointer initEdgePtr(int u, int w) {
	edge_pointer tempEdge = malloc(sizeof(edge));
	tempEdge->u = u;
	tempEdge->w = w;
	tempEdge->link = NULL;
	return tempEdge;
}

void getGraph(FILE* inputFile) {
	/* 참조: https://stackoverflow.com/questions/14001907/read-data-from-file-till-end-of-line-in-c-c */
	fscanf(inputFile, "%d", &n);
	while (fgetc(inputFile) != '\n');
	graph = malloc(n * sizeof(node));

	char buffer[100];
	char* pbuff;
	int vertex;

	for (int i = 0; i < n; i++) {
		if (!fgets(buffer, sizeof buffer, inputFile)) break;
		pbuff = buffer;
		vertex = strtol(pbuff, &pbuff, 10);
		node_pointer head = initNodePtr(vertex);
		graph[i] = head;
		while (1) {
			if (*pbuff == '\n' || *pbuff == '\0') break;
			vertex = strtol(pbuff, &pbuff, 10);
			node_pointer node = initNodePtr(vertex);
			pushNode(head, node);
		}
	}
}

void readGraph() {
	for (int i = 0; i < n; i++) {
		node_pointer nodePtr = graph[i];
		printf("%3d\t", nodePtr->vertex);
		nodePtr = nodePtr->link;
		while (nodePtr != NULL) {
			printf("%3d", nodePtr->vertex);
			nodePtr = nodePtr->link;
		}
		printf("\n");
	}
}

void readList() {
	printf("vertex\t");
	for (int i = 0; i < n; i++)
		printf("%3d", i);
	printf("\n");
	printf("dfn\t");
	for (int i = 0; i < n; i++)
		printf("%3d", dfn[i]);
	printf("\n");
	printf("low\t");
	for (int i = 0; i < n; i++)
		printf("%3d", low[i]);
}

void pushNode(node_pointer head, node_pointer node) {
	node->link = head->link;
	head->link = node;
}

void init() {
	int i;
	top = initEdgePtr(-99, -99);

	for (i = 0; i < n; i++) {
		dfn[i] = low[i] = -1;
	}
	num = 0;
}

void add(edge_pointer top, int x, int y) {
	edge_pointer edge = initEdgePtr(x, y);
	edge->link = top->link;
	top->link = edge;
}

void delete(edge_pointer top, int* x, int* y) {
	if (top->link == NULL) return;
	edge_pointer edge = top->link;
	*x = edge->u;
	*y = edge->w;
	top->link = edge->link;
	edge->link = NULL;
	//free(edge);
}

void bicon(int u, int v) {
	int w, x, y;
	node_pointer ptr;
	dfn[u] = low[u] = num++;
	for (ptr = graph[u]; ptr; ptr = ptr->link) {
		w = ptr->vertex;
		printf("v: %d\tu: %d\tw:%d\n", v, u, w);
		if (v != w && dfn[w] < dfn[u]) { /* add edge to stack */
			// ...
			// w가 u의 자식노드라면...
			//_add(u, w);
			add(top, u, w);
		}
		if (dfn[w] < 0)
		{ /* w is an unvisited vertex */
			bicon(w, u);
			low[u] = MIN2(low[u], low[w]);
			// dfnlow 호출 후 업데이트 된 low[w]와 비교하여 low[u]를 업데이트한다.
			if (low[w] >= dfn[u])
			{
				printf("New biconnected component: ");
				do
				{ /* delete edge from stack */
					//_delete(&x, &y);
					delete(top, &x, &y);
					printf("<%d, %d>", x, y);
				} while (!((x == u) && (y == w)));
				printf("\n");
			}
		}
		else if (w != v) low[u] = MIN2(low[u], dfn[w]);
		// 이미 검사가 끝난 인덱스는 low와 low의 비교는 이미 이뤄짐
		// low[u]는 다른 연결된 노드에 의해 내려갈수 있는 최대 노드를 가리킨다.
		// dfn[w]는 새로 확인하는 연결된 노드의 dfn을 확인한다.
	}
}