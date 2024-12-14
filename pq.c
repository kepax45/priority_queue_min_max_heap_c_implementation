#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct pq {
	int n;
	int* arr;
	int last_index;
	int resize_if_needed;
	int (*cmp)(int a, int b);
} priority_queue;




/* init_queue - Initializes a priority queue struct with the given parameters (Using the form of a binary heaped stored inside an array).
 * ---> int n - initial size of the array used to represent binary tree.
 * ---> int (*cmp)(int a, int b) - a comparator function for the PQ, (by using min_cmp or max_cmp you switch between min and max heaps).
 * ---> int resize_if_needed - two types of pq initialization are available. Either a resizable pq which resizes the space for the heap by the factor of 1.5 whenever 
 *      the pq is filled, or a fixed size pq which throws a segmentation fault upon trying to push a new element into the pq.
 * 
*/


priority_queue* init_queue(int n, int (*cmp)(int a, int b), int resize_if_needed) {
	priority_queue* pq = (priority_queue*) malloc(sizeof(priority_queue));
	pq->n = n;
	pq->arr = (int*) malloc(n*sizeof(int));
	pq->last_index = 0;
	pq->cmp = cmp;
	pq->resize_if_needed = resize_if_needed;
	return pq;
}

/* delete_pq - Deletes the passed priority_queue struct from memory, along with it's array used to represent the binary heap.
*/



void delete_pq(priority_queue* pq) {
	free(pq->arr);
	free(pq);
}



/* father - Returns the index of the father for the node at a particular index inside the array used to reprsent a binary heap. 
 * If the given index is <= 0, it returns -1, which is used to mark the lack of an existing father inside the binary heap.
*/

int father(priority_queue* pq, int index) {
	if(index <= 0) return -1;
	return (index - 1) / 2;
}

/* push_pq - Adds another element inside the priority queue. If the priority queue is of the fixed type, once the heap is filled and an 'overflow' is attempted,
 * the program forces a segmentation fault by trying to assign a value to a NULL pointer. If the PQ is not full, an element is added at the end of the heap and then
 * pushed towards the root as high as possible, ensuring it's proper position within the heap (search for binary heap insertion logic online).
*/

void push_pq(priority_queue* pq, int new_elem) {
	if(pq->last_index == pq->n && pq->resize_if_needed) {
		pq->n = (int) ceil(pq->n * 1.5);
		int* old_ptr = pq->arr;
		pq->arr = (int*) realloc(pq->arr, pq->n*sizeof(int));
	} else if(pq->last_index == pq->n && !pq->resize_if_needed) {
		*((int*)NULL) = 1;
		return;
	}
	pq->arr[pq->last_index] = new_elem;
	int tmp_ind = pq->last_index;
	int tmp_val;
	while(father(pq, tmp_ind) != -1 && pq->cmp(pq->arr[tmp_ind], pq->arr[father(pq, tmp_ind)]) == 1) {
		tmp_val = pq->arr[father(pq, tmp_ind)];
		pq->arr[father(pq, tmp_ind)] = pq->arr[tmp_ind];
		pq->arr[tmp_ind] = tmp_val;
		tmp_ind = father(pq, tmp_ind);
	}
	pq->last_index++;
}

/* left_son - returns the index of the left son for a node which is located at a particular index within the array.
*/

int left_son(priority_queue* pq, int index) {
    int son_index = (index)*2 + 1;
    if(son_index < 0 || son_index >= pq->last_index) return -1;
    return son_index;
}

/* right_son - returns the index of the right son for a node which is located at a particular index within the array.
*/

int right_son(priority_queue* pq, int index) {
    int son_index = (index)*2 + 2;
    if(son_index < 0 || son_index >= pq->last_index) return -1;
    return son_index;
}

/* min_cmp - comparation function used with the binary heap for achieving a min-heap. Returns true if a < b, and false otherwise.
*/

int min_cmp(int a, int b) {
	return (a < b);
}

/* min_cmp - comparation function used with the binary heap for achieving a max-heap. Returns true if a > b, and false otherwise.
*/

int max_cmp(int a, int b) {
	return (a > b);
}

/* top_pq - returns the element at the top of PQ (for a max-heap PQ this would return the largest element, for a min-heap PQ it would return the smallest element).
 * in the case of the PQ being empty, the function performs a "forced crash".
*/
int top_pq(priority_queue* pq) {
	if(pq->last_index == 0) { *((int*)NULL) = 1;}
    return pq->arr[0];
}

/* pop_pq - removes the element with the top priority. After which, the PQ is reorganized (search for binary heap element deletion logic online), 
 * so that the second largest element is now the new highest priority element.
*/
void pop_pq(priority_queue* pq) {
    if(pq->last_index == 0) {
		*((int*)NULL) = 1;
		return;
	}
    int tmp_val;
    tmp_val = pq->arr[0];
    pq->arr[pq->last_index] = 0;
    pq->arr[0] = pq->arr[--(pq->last_index)];
    int tmp_ind = 0;
    while(left_son(pq, tmp_ind) != -1 || right_son(pq, tmp_ind) != -1) {
        int val = pq->arr[tmp_ind];
        int ind = tmp_ind;
        if(left_son(pq, tmp_ind) != -1 && pq->cmp(val, pq->arr[left_son(pq, tmp_ind)]) == 0) {
            val = pq->arr[left_son(pq, tmp_ind)];
            ind = left_son(pq, tmp_ind);
        }
        if(right_son(pq, tmp_ind) != -1 && pq->cmp(val, pq->arr[right_son(pq, tmp_ind)]) == 0) {
            val = pq->arr[right_son(pq, tmp_ind)];
            ind = right_son(pq, tmp_ind);
        }
        if(tmp_ind == ind) break;
        tmp_val = pq->arr[tmp_ind];
        pq->arr[tmp_ind] = val;
        pq->arr[ind] = tmp_val;
        tmp_ind = ind;
    }
}

/* init_fixed_size_priority_queue - A wrapper function used to define a new priority queue which has a fixed size property.
*/

priority_queue* init_fixed_size_priority_queue(int n, int (*cmp)(int a, int b)) {
	return init_queue(n, cmp, 0);
}


/* init_resizable_priority_queue - A wrapper function used to define a new priority queue which has a resizability property.
*/

priority_queue* init_resizable_priority_queue(int (*cmp)(int a, int b)) {
	return init_queue(1, cmp, 1);
}
/* returns true if the PQ is empty, false otherwise.
*/

int empty_pq(priority_queue* pq) {
    return pq->last_index == 0;
}



int main() {
	//priority_queue* pq = init_fixed_size_priority_queue(2, &min_cmp);
	//priority_queue* pq = init_resizable_priority_queue(&max_cmp);
	return 0;
}
