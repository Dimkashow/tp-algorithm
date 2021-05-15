#include <iostream>

int bin_search(int const *array, int left, int right) {
    while (true) {
        int mid = left + (right - left) / 2;

        if (array[mid] > array[mid + 1] && array[mid] > array[mid - 1])
            return mid;

        if (array[mid] > array[mid + 1])
            right = mid;
        else
            left = mid + 1;
    }
}

int find_answer(int const *array, int begin, int end) {

    if (end + 1 == 2)
        if (array[0] < array[1])
            return 1;
        else
            return 0;
        
    if (array[0] > array[1])
        return 0;

    if (array[end - 1] < array[end])
        return end;

    return (bin_search(array, begin, end));
}

int main() {
    int N = 0;
    std::cin >> N;
    int *array = new int[N];
    for (int i = 0; i < N; ++i)
        std::cin >> array[i];
    
    std::cout << find_answer(array, 0, N - 1) << std::endl;
    delete [] array;
    return 0;
}
