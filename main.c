#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h> //for the infinity value
#include<float.h>

int MaxElements = 100;

float infinity = DBL_MAX;

typedef struct vertexNode* vertexNodePtr;

typedef struct vertexNode {
    int key;
    double adjDistance;
    vertexNodePtr next;
}vertexNode;

typedef struct vertexPoint {
    int key;
    double bestDistance;
}vertexPoint;

typedef struct vertexPoint* vertexPointPtr;
typedef vertexNodePtr adjlist;
void insertVertexNode(vertexNodePtr header, int key, double dist);

//=============================================================//

struct heapStruct {
    int Capacity;
    int Size;
    vertexPointPtr Elements;
};
typedef struct heapStruct* priorityQueue;
void heapInsert(vertexPoint vert[], priorityQueue H, int locations[]);
vertexPoint DeleteMin(priorityQueue H, int locations[]);
priorityQueue initializeQueue(int MaxElements);
int IsFull(priorityQueue H);
int IsEmpty(priorityQueue H);

vertexPoint DeleteMin(priorityQueue H, int locations[]);

void rePriotize(priorityQueue H, int locations[], int changed_vertex_key, double best[]);

void buildHeap(vertexPoint arr[], int n, int locations[]);
void heapify(vertexPoint arr[], int n, int i, int locations[]);
//============================================================//

typedef int set[101]; //use MaxElements+1, 0 index is not used

void initializeSet(set s, int startingPoint);


void setUnion(set s, int root1, int root2);

int findSet(int element, set s);

void swap(vertexPointPtr x, vertexPointPtr y);

//=============================================================//

//main function
int main(void) {

    int start;
    printf("enter the starting point (1 to 100) : ");
    scanf_s("%d", &start);

    double best[101];

    for (int i = 1; i <= 100; i++) {
        best[i] = infinity;
    }

    best[start] = 0;

    double cost[101][101];


    int y[101];


    for (int x = 1; x <= 100; x++) {
        y[x] = (x * x) % 100;
    }

    double adjDistance;
    for (int x = 1; x <= 100; x++) {
        for (int j = 1; j <= 100; j++) {
            adjDistance = sqrt((x - j) * (x - j) + (y[x] - y[j]) * (y[x] - y[j]));
            if (adjDistance <= 13)  cost[x][j] = adjDistance;
            else cost[x][j] = infinity;
        }
    }

    adjlist adjMat[101];

    for (int x = 1; x <= 100; x++) {
        vertexNodePtr header = (vertexNodePtr)malloc(sizeof(vertexNode));
        if (header) {
            header->next = NULL;
        }
        adjMat[x] = header;
    }


    for (int x = 1; x <= 100; x++) {
        for (int j = 1; j <= 100; j++) {
            if (cost[x][j] != 0 && cost[x][j] != infinity)
                insertVertexNode(adjMat[x], j, cost[x][j]);
        }
    }

    int heap_locations[101];

    set s;

    vertexPoint dataArray[101];


    for (int k = 1; k <= 100; k++) {
        dataArray[k].key = k;
        dataArray[k].bestDistance = best[k];
    }

    initializeSet(s, start);

    int parents[101];

    parents[start] = start;
    priorityQueue H = initializeQueue(MaxElements);
    buildHeap(dataArray, 101, heap_locations);
    heapInsert(dataArray, H, heap_locations);

    vertexPoint closest_vertex;
    while (!IsEmpty(H)) {
        closest_vertex = DeleteMin(H, heap_locations);
        setUnion(s, start, closest_vertex.key);
        vertexNodePtr temp1 = adjMat[closest_vertex.key]->next;
        while (temp1) {
            if (findSet(temp1->key, s) == start) {
                temp1 = temp1->next;
                continue;
            }
            else if (best[temp1->key] > (closest_vertex.bestDistance + temp1->adjDistance)) {
                best[temp1->key] = closest_vertex.bestDistance + temp1->adjDistance;
                parents[temp1->key] = closest_vertex.key;
                rePriotize(H, heap_locations, temp1->key, best);
            }
            temp1 = temp1->next;
        }
    }
    printf("shortest routes...\n");
    for (int i = 1; i <= 100; i++) {
        if (i == start) {
            printf("\nparent: (%-3d,%3d) --> vertex: (%-3d,%3d)  distance: %f Start is here\n", parents[i], y[parents[i]], i, y[i], cost[i][parents[i]]);
            continue;
        }
        printf("\nparent: (%-3d,%3d) --> vertex: (%-3d,%3d)  distance: %f\n", parents[i], y[parents[i]], i, y[i], cost[i][parents[i]]);
    }


    return 0;
}

//=============================================================//
//function declarations


void setUnion(set s, int start, int root2) {
    if (root2 == start) s[root2] = -1;
    else {
        s[root2] = start;
        s[start]--;
    }
}


int findSet(int element, set s) {
    if (s[element] < 0) {
        return element;
    }
    else {
        return s[element];
    }
}

void initializeSet(set s, int starting_point) {
    int i;
    for (i = 1; i <= MaxElements; i++) {
        s[i] = 0;
    }
    s[starting_point] = -1;
}

void insertVertexNode(vertexNodePtr header, int key, double dist) {
    vertexNodePtr newVertex = (vertexNodePtr)malloc(sizeof(vertexNode));
    if (!newVertex) {
        printf("out of memory\n");
        return;
    }
    newVertex->key = key;
    newVertex->adjDistance = dist;

    newVertex->next = header->next;
    header->next = newVertex;
}

priorityQueue initializeQueue(int MaxElements) {

    priorityQueue H;
    H = malloc(sizeof(struct heapStruct));
    if (!H) {
        printf("out of space!!");
    }


    H->Elements = malloc((101) * sizeof(vertexPoint));

    if (H->Elements == NULL) printf("out of space, no heap array\n");

    H->Capacity = MaxElements;
    H->Size = 0;
    return H;
}

int IsFull(priorityQueue H) {
    if (H->Size == H->Capacity) return 1;
    else return 0;
}

vertexPoint DeleteMin(priorityQueue H, int locations[]) {

    int i, Child;
    vertexPoint MinElement, LastElement;

    MinElement = H->Elements[1];
    locations[MinElement.key] = -1;
    LastElement = H->Elements[H->Size--];


    for (i = 1; i * 2 <= H->Size; i = Child) {

        Child = i * 2;

        if (Child != H->Size && H->Elements[Child + 1].bestDistance < H->Elements[Child].bestDistance)
            Child++;


        if (LastElement.bestDistance > H->Elements[Child].bestDistance) {
            H->Elements[i] = H->Elements[Child];
            locations[H->Elements[i].key] = i;
        }
        else
            break;
    }

    H->Elements[i] = LastElement;
    locations[H->Elements[i].key] = i;

    return MinElement;
}

void heapInsert(vertexPoint dataArray[], priorityQueue H, int locations[]) {
    int i;
    static int number = 0;

    for (i = 1; i <= 100; i++) {
        H->Elements[i] = dataArray[i];
        locations[H->Elements[i].key] = i;
        H->Size++;
        number++;
    }
}

int IsEmpty(priorityQueue H) {
    if (H->Size == 0) return 1;
    else return 0;
}


void rePriotize(priorityQueue H, int locations[], int changed_vertex_key, double best[]) {
    vertexPoint temp;
    int heap_location = locations[changed_vertex_key];
    H->Elements[heap_location].bestDistance = best[changed_vertex_key];

    for (int i = heap_location; i > 1 && H->Elements[i / 2].bestDistance > H->Elements[i].bestDistance; i /= 2) {
        temp = H->Elements[i / 2];
        H->Elements[i / 2] = H->Elements[i];
        locations[H->Elements[i / 2].key] = i / 2;

        H->Elements[i] = temp;
        locations[H->Elements[i].key] = i;
    }
}


void heapify(vertexPoint arr[], int n, int i, int locations[])
{
    int smallest = i;
    int l = 2 * i;
    int r = 2 * i + 1;


    if (l < n && arr[l].bestDistance < arr[smallest].bestDistance)
        smallest = l;


    if (r < n && arr[r].bestDistance < arr[smallest].bestDistance)
        smallest = r;


    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        locations[arr[i].key] = i;
        locations[arr[smallest].key] = smallest;
        heapify(arr, n, smallest, locations);
    }
}


void buildHeap(vertexPoint arr[], int n, int locations[])
{

    int startIdx = (n / 2) - 1;
    for (int i = startIdx; i >= 0; i--) {
        heapify(arr, n, i, locations);
    }
}

void swap(vertexPointPtr x, vertexPointPtr y) {
    vertexPoint temp = *y;
    *y = *x;
    *x = temp;
}