#include <iostream>
#include <cmath>

using namespace std;

void div();

void snow(float arr[12]);

void squeeze(int arr[], int size);

int main() {
    //div();

    //float arr[12] = {1, 0, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11};

    int arr2[] = {3,5, 0,-2, 0,7,9};
    //snow(arr);

    squeeze(arr2, 7);
    return 0;
}

void div() {
    int num;

    do {
        // Ask for user input
        cout << "Type an integer: ";
        cin >> num;

        // Calculate the square of the number and print to the screen

        int sqr = pow(num, 2);
        cout << sqr << endl;
        
    } while (!(num % 2 == 0) & !(num % 3 == 0));

    return;
}

void snow(float arr[12]) {
    // array of monthly snowfall
    // function displays 
    // month with the most snowfall
    // average monthly snowfall
    // month with the least snowfall
    // total snowfall

    float max = 0;
    float min = 1e20;

    int maxIndex;
    int minIndex;

    int i;

    float sum = 0;

    for (i=0; i < 12; i++) {
        if (arr[i] > max) {
            max = arr[i];
            maxIndex = i;
        }
        if (arr[i] < min) {
            min = arr[i];
            minIndex = i;
        }

        sum += arr[i];
    }

    float avg = sum / 12;

    cout << "The month with the max snowfall is the: " << maxIndex + 1 << "th month." << endl;
    cout << "The month with the lease snowfall is the: " << minIndex + 1 << "th month." << endl;
    cout << "The average monthly snowfall is: " << avg << endl;
    cout << "The total monthly snowfall is: " << sum << endl;
}

void squeeze(int arr[], int size) {
    // moves all zero elements in an array toward the end and shifts all the other numbers accordingly
    // e.g. 3, 5, 0, -2, 0, 7, 9 -> 3, 5, -2, 7, 9, 0, 0

    // count the number of 0 occurences, save all the non-zero values

    // create new array

    int arr2[size];

    int index = 0; // save index
    int numZero = 0;

    for (int i = 0; i < size; i++) {
        int current = arr[i];

        if (current != 0) {
            arr2[index] = current;
            index++;
        } else if (current == 0) {
            numZero++;
        }
    }

    int j = size - numZero;

    for (j; j < size; j++) {
        arr2[j] = 0;
    }

    for (int k = 0; k < size; k++) {
        cout << arr2[k] << endl;
    }
}