#include <iostream>
#include <vector>

using namespace std;

template<typename T, typename CMP>
int Partition(T *a, int n, CMP cmp) {
    if(n <= 1) {
        return 0;
    }
    swap(a[n / 2], a[n - 1]);
    T pivot = a[n - 1];
    T i = 0;
    T j = n - 2;
    while(i <= j) {
        for( ; cmp(a[i], pivot) < 0; ++i) {}
        for( ; cmp(j, 0) >= 0 && cmp(a[j], pivot) >= 0; --j) {}
        if(cmp(i, j) < 0) {
            swap(a[i++], a[j--]);
        }
    }
    swap(a[i], a[n-1]);
    return i;
}

template <typename T, typename CMP>
int find_k_stat(T *arr, int size, int n, CMP cmp) {
    int left = 0;
    int right = size;
    int pos = -1;
    while (pos != n) {
        pos = left + Partition(&arr[left], right - left, cmp);
        if (pos < n) {
            left += (pos - left + 1);
        }
        if (pos > n) {
            right -= (right - pos);
        }
    }

    return arr[pos];
}


int cmp(int r, int l) {
    return r - l;
}


int main() {
    int size = 0;
    int n = 0;
    cin >> size >> n;

    int *arr = new int[size];
    for (int i = 0; i < size; ++i) {
        cin >> arr[i];
    }

    cout << find_k_stat(arr, size, n, cmp) << endl;
    delete [] arr;
    return 0;
}

