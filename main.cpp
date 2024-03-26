#include <iostream>
#include "omp.h"
#include <thread>
#include <vector>
#include <chrono>
using namespace std;

void getArraySum() {
	int num;
	cout << "Enter size of array: " << endl;
	cin >> num;
	int* array = new int[num];
	double itog = 0;

	for (int i = 0; i < num; i++) {
		srand(time(NULL));
		array[i] = rand();
	}
#pragma omp parallel num_threads(3) reduction(+:itog)
	{
#pragma omp parallel for
		for (int i = 0; i < num; i++) {
			itog += array[i];
		}
	}
	cout << "Sum = " << itog << endl;
	return;

	}

	void merge(int* array, int low, int mid, int high)
	{
		int* temp = new int[high + 1];
		int i = low;
		int j = mid + 1;
		int k = 0;

		while (i <= mid && j <= high)
		{
			if (array[i] <= array[j])
				temp[k++] = array[i++];
			else
				temp[k++] = array[j++];
		}
		while (i <= mid)
			temp[k++] = array[i++];
		while (j <= high)
			temp[k++] = array[j++];
		k--;
		while (k >= 0)
		{
			array[k + low] = temp[k];
			k--;
		}
	}

	void sort(int* array, int left, int right, bool parallel)
	{

		if (left < right)
		{
			int middle = left + (right - left) / 2;
			if (!parallel) {
				sort(array, left, middle, false);
				sort(array, middle + 1, right, false);
			}
			else {
#pragma omp parallel sections 
				{
#pragma omp section
					sort(array, left, middle, false);
#pragma omp section
					sort(array, middle + 1, right, false);
				}
			}

			merge(array, left, middle, right);
		}
	}

	void sortArray() {
		const int arraySize = 100;
		int array1[arraySize], array2[arraySize];
		long long int sum = 0;
		for (int i = 0; i < arraySize; ++i) {
			array1[i] = rand() % 500;
			array2[i] = array1[i];
		}

		chrono::high_resolution_clock::time_point start1 = chrono::high_resolution_clock::now();
		sort(array1, 0, arraySize - 1, false);
		chrono::high_resolution_clock::time_point end1 = chrono::high_resolution_clock::now();
		chrono::nanoseconds duration1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);

		chrono::high_resolution_clock::time_point start2 = chrono::high_resolution_clock::now();
		sort(array2, 0, arraySize - 1, true);
		chrono::high_resolution_clock::time_point end2 = chrono::high_resolution_clock::now();
		chrono::nanoseconds duration2 = chrono::duration_cast<chrono::nanoseconds>(end2 - start2);

		cout << "Array 1: " << endl;
		for (int i = 0; i < arraySize; i++) {
			cout << array1[i] << " ";
		}
		cout << endl;
		cout << "Array 2: " << endl;
		for (int i = 0; i < arraySize; i++) {
			cout << array2[i] << " ";
		}
		cout << endl;

		cout << "no parrallel: "<< duration1.count() << endl;
		cout << "parrallel: " << duration2.count() << endl;
	}

	int main() {
		cout << "Sum:" << endl;
		getArraySum();
		cout << "----------------------" << endl;
		cout << "Sort: " << endl;
		sortArray();
	}