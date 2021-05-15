#include <iostream>
#include <cstring>

using namespace std;

long long getNByte(long long number, size_t n) {
    return (number >> (8 * n)) % 256;
}

int *makeCountArr(long long *data, int size, int n) {
    int *counter = new int[256];
    memset(counter, 0, 256 * sizeof(int));

    for (size_t i = 0; i < size; i++) {
        counter[getNByte(data[i], n)]++;
    }

    for (size_t i = 1; i < 256; i++) {
        counter[i] += counter[i - 1];
    }
    return counter;
}

long long *sortNByteArr(long long *data, int size, int n, int *counters) {
    auto *tmp = new long long[size];

    for (int i = size - 1; i >= 0; i--) {
        long long byte = getNByte(data[i], n);
        int insert = --counters[byte];
        tmp[insert] = data[i];
    }
    return tmp;
}

void countingByteSort(long long *data, int size, const size_t n) {
    int *counter = makeCountArr(data, size, n);

    auto *tmp = sortNByteArr(data, size, n, counter);

    for (size_t i = 0; i < size; i++) {
        data[i] = tmp[i];
    }

    delete[] tmp;
    delete[] counter;
}

void LSDSort(long long *data, size_t size) {
    size_t totalBytes = sizeof(long long);

    for (size_t byte = 0; byte < totalBytes; byte++) {
        countingByteSort(data, size, byte);
    }
}

int main() {
    size_t n = 0;
    std::cin >> n;

    auto *data = new long long[n];

    for (size_t i = 0; i < n; i++) {
        cin >> data[i];
    }

    LSDSort(data, n);

    for (size_t i = 0; i < n; i++) {
        cout << data[i] << " ";
    }

    delete[] data;

    return 0;
}
