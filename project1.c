#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fonction de tri à bulle (Bubble_Sort)
void bubbleSort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Fonction de tri par insertion (Insertion_Sort)
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Fonction de tri par sélection (Selection Sort)
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        int temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
    }
}

// Fonction de tri rapide (Quick Sort)
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Fonction de tri fusion (Merge Sort)
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Fonction pour remplir le tableau avec des valeurs aléatoires (Fills array with random values)
void fillArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

// Fonction pour copier un tableau (Copies array)
void copyArray(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// Mesurer et enregistrer le temps pour chaque algorithme (Measures and records time for each algorithm)
void measureSortingTime(int n, FILE *dataFile) {
    int *arr = (int *)malloc(n * sizeof(int));
    int *arrCopy = (int *)malloc(n * sizeof(int));
    fillArray(arr, n);
    
    double timeSpent;
    clock_t start, end;

    // Bubble Sort
    copyArray(arr, arrCopy, n);
    start = clock();
    bubbleSort(arrCopy, n);
    end = clock();
    timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(dataFile, "%d %f ", n, timeSpent);  

    // Insertion Sort
    copyArray(arr, arrCopy, n);
    start = clock();
    insertionSort(arrCopy, n);
    end = clock();
    timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(dataFile, "%f ", timeSpent);

    // Selection Sort
    copyArray(arr, arrCopy, n);
    start = clock();
    selectionSort(arrCopy, n);
    end = clock();
    timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(dataFile, "%f ", timeSpent);

    // Quick Sort
    copyArray(arr, arrCopy, n);
    start = clock();
    quickSort(arrCopy, 0, n - 1);
    end = clock();
    timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(dataFile, "%f ", timeSpent);

    // Merge Sort
    copyArray(arr, arrCopy, n);
    start = clock();
    mergeSort(arrCopy, 0, n - 1);
    end = clock();
    timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(dataFile, "%f\n", timeSpent);

    free(arr);
    free(arrCopy);
}

int main() {
    FILE *dataFile = fopen("sorting_algorithms_time_data.dat", "w");
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (dataFile == NULL || gnuplotPipe == NULL) {
        printf("Erreur lors de l'ouverture du fichier ou du pipe Gnuplot.\n");
        return 1;
    }

    for (int n = 1000; n <= 10000; n += 1000) {
        measureSortingTime(n, dataFile);
    }

    fclose(dataFile);

    // Génération du graphique avec Gnuplot (Plotting graph with Gnuplot)
    fprintf(gnuplotPipe, "set title 'Sorting Algorithms Time Complexity'\n");
    fprintf(gnuplotPipe, "set xlabel 'Number of Elements'\n");  
    fprintf(gnuplotPipe, "set ylabel 'Time (seconds)'\n");


    fprintf(gnuplotPipe, "plot 'sorting_algorithms_time_data.dat' using 1:2 smooth csplines with lines title 'Bubble Sort', ");
    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:3 smooth csplines with lines title 'Insertion Sort', ");
    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:4 smooth csplines with lines title 'Selection Sort', ");
    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:5 smooth csplines with lines title 'Quick Sort', ");
    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:6 smooth csplines with lines title 'Merge Sort'\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}
