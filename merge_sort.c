#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_SIZE 100

#ifdef USE_INT_MAX
void merge(int *a, int p, int q, int r)
{
	int n1 = q-p+1;
	int n2 = r-q;
	int *left = malloc(n1 * sizeof(int)+1);
	int *right = malloc(n2 * sizeof(int)+1);
	int i,j,k;
	
	for (i=0;i<n1;i++)
		left[i]=a[p+i];
	
	left[i]=INT_MAX;

	for(j=0;j<n2;j++)
		right[j]=a[q+j+1];
	
	right[j]=INT_MAX;
	
	i=j=0
	for(k=p;k<=r;k++){
		if(left[i] <= right[j]){
			a[k] = left[i++];
		} else {
			a[k] = right[j++];
		}	
	}
}
#else
void merge(int *a, int p, int q, int r)
{
	int n1 = q-p+1;
	int n2 = r-q;
	int *left = malloc(n1 * sizeof(int));
	int *right = malloc(n2 * sizeof(int));
	int i,j,k;
	
	for (i=0;i<n1;i++)
		left[i]=a[p+i];

	for(j=0;j<n2;j++)
		right[j]=a[q+j+1];
	
	i=j=0;
	k=p;
	while(i<n1 & j<n2){
		if(left[i] <= right[j]){
			a[k++] = left[i++];
		} else {
			a[k++] = right[j++];
		}
	}
	while(i<n1){
		a[k++]=left[i++];
	}
	while(j<n2){
		a[k++]=right[j++];
	}
	
		
}
#endif

void merge_sort(int *a, int p, int r)
{
	int q;
	if (p<r){
		q = (p+r)/2;
		merge_sort(a, p, q);
		merge_sort(a, q+1, r);
		merge(a, p, q, r);
	}
}

void print_arr(int *a, int len)
{
	int i;
	for(i=0;i<len;i++){
		printf("a[%d]=%d\t", i, a[i]);
	}
	printf("\n");
}
int main(void)
{
	int arr[ARR_SIZE];
	int i;
	
	srand(time(NULL));
	for(i=0;i<ARR_SIZE;i++)
		arr[i]=rand()%(ARR_SIZE*10);
	
	print_arr(arr, ARR_SIZE);
	
	merge_sort(arr, 0, sizeof(arr)/sizeof(int)-1);
	
	print_arr(arr, ARR_SIZE);
	
	return 0;
}