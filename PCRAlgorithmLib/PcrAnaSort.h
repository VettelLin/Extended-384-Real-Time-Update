
#ifndef _SORT_H
#define _SORT_H

#include <stdlib.h>
#include <vector>

using namespace std;
enum {BUBBLE = 0, INSERTION, QUICK, SELECTION};
enum {MYLONG = 0, MYDOUBLE, MYINTERGER};

template <class T>

class AFX_EXT_CLASS CPcrAnalysisSort
{
private:
	CPcrAnalysisSort(){}
	typedef void (CPcrAnalysisSort<T>:: * funpt)(T*);
	vector<funpt> f;

public:
	CPcrAnalysisSort(int);								// constructor
	void bubble(T array[]);					// bubble sort
	void insertion(T array[]);				// insertion sort
	void quick(T array[], int, int);		// quick sort
	void selection(T array[]);				// selectioni sort
	void quickHT(T array[]);
	bool SelectSortType(int i, T array[]);

private:
	int size;
};

template <class T>
void CPcrAnalysisSort<T>::quickHT(T array[])
{
	quick(array, 0, size - 1);
} 

template <class T>
bool CPcrAnalysisSort<T>::SelectSortType(int i, T array[])
{
	if(i < BUBBLE || i > SELECTION)
		return false;

	(this->*f[i])(array);
	return true;
}


template <class T>
CPcrAnalysisSort<T>::CPcrAnalysisSort(int s) {
	if (s < 0) 	return;
	size = s;				// if the size is valid, assign it 

	f.push_back(&CPcrAnalysisSort<T>::bubble);
	f.push_back(&CPcrAnalysisSort<T>::insertion);
	f.push_back(&CPcrAnalysisSort<T>::quickHT);
	f.push_back(&CPcrAnalysisSort<T>::selection);
}


template <class T>
void CPcrAnalysisSort<T>::bubble(T array[]) {
	
	T temp;
	int last = size - 1;
	bool sorted = true;

	do {
		sorted = true;
		for (int i = 0; i < last; i++) 
		{
			//	swap elements if the higher index element is 
			//	greater than the smaller index element
			if (array[i] > array[i + 1]) {	
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
				sorted = false;
			}
		} 

		last--;
	} while (!sorted);

}


template <class T>
void CPcrAnalysisSort<T>::insertion(T array[]) {

	T cVal;		// current value being examined
	
	for (int i = 1; i < size; i++) {

		cVal = array[i];
		int n = 0;
		for (int n = i - 1; n >= 0 && cVal < array[n]; n--) {
			array[n + 1] = array[n];
		}

		array[n + 1] = cVal;

	}	// end for loop

}


template <class T>
void CPcrAnalysisSort<T>::quick(T array[], int llimit, int rlimit) {

	T temp;
	int left = llimit;
	int right = rlimit;
	int pivot = (left + right) / 2;	// find the median
	T median = array[pivot];


	do {

		while ((array[left] < median) && (left < rlimit)) {
			left++;
		}

		while ((median < array[right]) && (right > llimit)) {
			right--;
		}

		if (left <= right) {

			// swap elements
			temp = array[left];
			array[left] = array[right];
			array[right] = temp;
			left++;
			right--;
		}

	} while (left <= right);


	if (llimit < right) {
		CPcrAnalysisSort<T>::quick(array, llimit, right);
	}


	if (left < rlimit) {
		CPcrAnalysisSort<T>::quick(array, left, rlimit);
	}

}


template <class T>
void CPcrAnalysisSort<T>::selection(T array[]) {

	T temp;
	int min;

	for (int i = 0; i < size - 1; i++) 
	{
		min = i;

		for (int n = i + 1; n < size; n++) 
		{
			if (array[n] < array[min]) 
			{
				min = n;
			}

		}
		temp = array[min];
		array[min] = array[i];
		array[i] = temp;

	}

}

#endif