#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define PARENT(i)	((i)>>1)
#define LEFT(i)		((i)<<1)
#define RIGHT(i)	(((i)<<1)+1)

#define HEAP_SIZE 10

inline void exchange(int *a, int *b)
{
	int c;
	c = *a;
	*a = *b;
	*b = c;
}

void print_heap(int *a)
{
	int i;
	for(i=1;i<= a[0];i++){
		printf("a[%d]=%d\t",i,a[i]);
		if(!(i%10))
			puts("\n");
	}
}
//a[0] store heap_size
#ifndef ITERATION
void max_heapify(int *a, int i)
{
	int left = LEFT(i);
	int right = RIGHT(i);
	int large = i;
	
	if (left <= a[0] &&  a[left] > a[i])
		large = left;
	if (right <= a[0] && a[right] > a[large])
		large = right;
	
	if (large != i){
		exchange(&a[i],&a[large]);
		max_heapify(a, large);
	}
}
#else
void max_heapify(int *a, int i)
{
	int left,right,large=i;
	
	while (i<=a[0]){
		left = LEFT(i);
		right = RIGHT(i);
		
		if (left <= a[0] &&  a[left] > a[i])
			large = left;
		if (right <= a[0] && a[right] > a[large])
			large = right;
	
		if (large != i){
			exchange(&a[i],&a[large]);
			i = large;
		} else {
			break;
		}
		
	}
}
#endif
void build_max_heap(int *a)
{
	int i;
	a[0]=HEAP_SIZE;
	for(i=HEAP_SIZE/2;i>=1;i--)
		max_heapify(a,i);
}

void heap_sort(int *a)
{
	int i;
	build_max_heap(a);
	for(i=HEAP_SIZE;i>=2;i--){
		exchange(&a[i],&a[1]);
		a[0]--;
		max_heapify(a,1);
	}
}

inline int heap_maximum(int *a)
{
	return a[1];
}

int heap_extract_max(int *a)
{
	if(a[0] < 1){
		printf("heap underflow\n");
		exit(-1);
	}
	
	int max = a[1];
	a[1] = a[a[0]];
	a[0] -= a[0];
	max_heapify(a,1);
	
	return max;
}

int heap_increase_key(int *a, int i, int key)
{
	if (key < a[i]){
		printf("new key is smaller than current key\n");
		return -1;
	}
	a[i] = key;
	while (i>1 && a[PARENT(i)]<a[i]){
		exchange(&a[i], &a[PARENT(i)]);
		i = PARENT(i);
	}
	return 0;
}

int max_heap_insert(int *a, int key)
{
	a[0]++;
	a[a[0]] = INT_MIN;
	heap_increase_key(a,a[0],key);
	
}

int main(void)
{
	int i;
	int a[HEAP_SIZE+10];
	a[0]=HEAP_SIZE;
	srand(time(NULL));
	for(i=1;i<=HEAP_SIZE;i++)
		a[i] = rand()%100;
	
	print_heap(a);
	build_max_heap(a);
	//heap_increase_key(a,5,211);
	max_heap_insert(a,211);
	print_heap(a);
	//for(i=1;i<=HEAP_SIZE;i++)
	//	printf("the largest number in the excess:%d\n",heap_extract_max(a));
	return 0;
}