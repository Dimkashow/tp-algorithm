#include <iostream>
#include <cstring>

using namespace std;

template <typename T, typename CMP>
class Heap {
public:
    Heap(const T* array, size_t size, CMP cmp) :
            array_(new T[size]),
            size_(size),
            capacity_(size),
            cmp_(cmp) {
        memcpy(array_, array, sizeof(T) * size_);
        for (int i = size_ / 2 - 1; i >= 0; i--) {
            shiftDown(i);
        }
    }

    void insert(const T &item) {
        if (size_ == capacity_) {
            capacity_ *= 2;
            T *tmp_array = new T[capacity_];
            memcpy(tmp_array, array_, sizeof(T) * (size_));
            delete[] array_;
            array_ = tmp_array;
        }
        size_++;
        array_[size_ - 1] = item;
        shiftUp(size_ - 1);
    }

    T extractMax() {
        T result = array_[0];
        array_[0] = array_[size_ - 1];
        --size_;
        shiftDown(0);
        return result;
    }

    const T &peekMax() const {
        return array_[0];
    }

    int getSize() {
        return size_;
    }

    ~Heap() {
        delete[] array_;
    }

private:
    void shiftUp(int index) {
        if (index == 0) {
            return;
        }
        int parent = (index - 1) / 2;
        if (cmp_(array_[parent], array_[index]) >= 0) {
            return;
        }
        T tmp = array_[parent];
        array_[parent] = array_[index];
        array_[index] = tmp;
        shiftUp(parent);
    }

    void shiftDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;
        if (left < size_ && cmp_(array_[left], array_[index]) > 0) {
            largest = left;
        }
        if (right < size_ && cmp_(array_[right], array_[largest]) > 0) {
            largest = right;
        }
        if (largest != index) {
            T tmp = array_[index];
            array_[index] = array_[largest];
            array_[largest] = tmp;
            shiftDown(largest);
        }
    }

    CMP cmp_;
    T *array_;
    size_t size_;
    size_t capacity_;
};

int cmp(const int l, const int r) { return r - l;}
typedef int (*CMP)(const int l, const int r);

int findTime(int *a, int size) {
    Heap<int, CMP> heap(a, size, cmp);

    int res = 0;
    int sum = 0;
    while (heap.getSize() > 1) {
        sum = heap.extractMax();
        sum += heap.extractMax();
        heap.insert(sum);
        res += sum;
    }
    return res;
}

int main() {
    int n = 0;
    cin >> n;
    int *arr = new int [n];

    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    cout << findTime(arr, n);
    return 0;
}

