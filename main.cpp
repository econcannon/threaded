#include <iostream>
#include <thread>
#include <random>
#include <cmath>
#include <sys/time.h>

using namespace std;

int *merge(int a[], int b[], int bigt, int smallt);
void finalize(int arr[]);
void generate_array(int arr[], int size);
int *divide_array(int arr[], int n, int t_num, int strack);
void printArray(int arr[], int size);
void bubbleSort(int arr[], int n);
int *divide_conquer(int arr[], int size, int tcount);
double nonThreadedSortTime(int arr[], int size);
double threadedSortTime(int arr[], int size, int tcount);

int main() {

    //User input for array information
    int size;
    cout << "Enter the size of the array: ";
    cin >> size;
    size = pow(2, size);
    int tcount;
    cout << "Enter the thread count: ";
    cin >> tcount;

    //Creates arrays to be sorted
    int *arr1 = new int[size];
    int *arr2 = new int[size];
    generate_array(arr1, size);
    generate_array(arr2, size);

    cout << endl <<"Array Created!" << endl;

    //Returns sorting time for each sorting method
    double elapsed1 = threadedSortTime(arr1, size, tcount);
    double elapsed2 = nonThreadedSortTime(arr2, size);

    cout << endl <<"Threaded Execution time: "<<endl<<elapsed1<<" (seconds)";
    cout << endl <<"Nonthreaded Execution time: "<<endl<<elapsed2<<" (seconds)";

    //Cleans up memory
    finalize(arr1);
    finalize(arr2);
    return 0;
}

int *divide_conquer(int arr[], int size, int tcount){

    //cout << endl << "Entered divide and conquer" << endl;
    int **tarray = new int *[tcount];
    tarray[0] = arr;
    int strack = size;
    for (int i = 1; i < tcount; i++)
    {
        strack -= size/tcount;
        tarray[i] = divide_array(arr, size/tcount, i, strack);
    }

    thread *tnames = new thread[tcount];
    for (int i = 0; i < tcount; i++)
    {
        tnames[i] = thread(bubbleSort, tarray[i], size/tcount);
        //cout << endl << "Thread " << i << " created!" << endl;
    }
    //cout << endl << "Threads created!" << endl;

    for (int i = 0; i < tcount; i++)
    {
        tnames[i].join();
        //cout << endl << "Thread " << i << " returned!" << endl;
    }
    //cout << endl << "Threads returned!" << endl;

    delete [] tnames;
    //cout << endl << "tnames Deleted!" << endl;

    int *result = tarray[0];
    for (int i = 1; i < tcount; i++)
    {
        result = merge(result, tarray[i], size/tcount*(i), size/tcount);
    }

    for (int i = 1; i < tcount; i++)
    {
        delete [] tarray[i];
    }

    delete [] tarray;
    //cout << endl << "tarray Deleted!" << endl;
    return result;
}

int *divide_array(int arr[], int n, int t_num, int strack){

    int *a = new int[strack];
    int *b = new int[n];

    for (int i = strack; i < strack + n; i++){
        b[i-strack] = arr[i];
    }
    for (int i = 0; i < strack; i++){
        a[i] = arr[i];
    }
    arr = a;
    return b;
}

void generate_array(int arr[], int size){

    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % size;
    }
}

void finalize(int arr[]){

    delete [] arr;
    cout << endl << "Array Deleted!" << endl;
}

int *merge(int a[], int b[], int bigt, int smallt){

    int *arr = new int[bigt + smallt];
    int i, j, k;
    i = 0;
    j = 0;
    k = 0;
    while (i < bigt && j < smallt) {
        if (a[i] < b[j]) {
            arr[k] = a[i];
            ++k;
            ++i;
        } else {
            arr[k] = b[j];
            ++k;
            ++j;
        }
    }
    while (i < bigt) {
        arr[k] = a[i];
        ++k;
        ++i;
    }
    while (j < smallt) {
        arr[k] = b[j];
        ++k;
        ++j;
    }
    return arr;
}

void printArray(int arr[], int size){

    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << '\t';
    }
}

void bubbleSort(int arr[], int n){

    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already
        // in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}


double nonThreadedSortTime(int arr[], int size){

    cout << endl << "NONTHREADED STARTING" << endl;
    //Timing Start

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    bubbleSort(arr, size);

    //Timing Finish

    gettimeofday(&end,0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    cout << endl << "NONTHREADED FINISHED" << endl;
    return elapsed;
}

double threadedSortTime(int arr[], int size, int tcount){

    cout << endl << "THREADED STARTED" << endl;
    //Timing Start

    struct timeval begin, end;
    gettimeofday(&begin, 0);

    arr = divide_conquer(arr, size, tcount);

    //Timing Finish

    gettimeofday(&end,0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    cout << endl <<"THREADED FINISHED" << endl;
    return elapsed;
}