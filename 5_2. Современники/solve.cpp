#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
struct man_time {
    struct tm time;
    bool live;
};


int cmp_data(struct tm a, struct tm b) {
    if (a.tm_year < b.tm_year)
        return -1;
    if (a.tm_year > b.tm_year)
        return 1;
    if (a.tm_mon < b.tm_mon)
        return -1;
    if (a.tm_mon > b.tm_mon)
        return 1;
    if (a.tm_mday < b.tm_mday)
        return -1;
    if (a.tm_mday > b.tm_mday)
        return 1;
    else
        return 0;
}


int cmp(const void *a, const void *b) {
    struct man_time a1 = *static_cast<const struct man_time*>(a);
    struct man_time b1 = *static_cast<const struct man_time*>(b);

    if (cmp_data(a1.time, b1.time) < 0) return -1;
    else if (cmp_data(a1.time, b1.time) > 0) return 1;
    else if (a1.live > b1.live) return -1;
    else if (a1.live < b1.live) return 1;
    else return 0;
}

int main() {
    tm birth{}, dead{};

    int n = 0;
    cin >> n;
    int size = 0;
    struct man_time arr[n * 2];

    for (int i = 0; i < n; i++) {
        cin >> birth.tm_mday >> birth.tm_mon >> birth.tm_year;
        cin >> dead.tm_mday >> dead.tm_mon >> dead.tm_year;

        birth.tm_year += 18;

        if (cmp_data(birth, dead) >= 0) // birthday18 >= deadday
            continue;

        arr[size].time = birth;
        arr[size].live = true;
        size++;

        birth.tm_year += (80 - 18);
        arr[size].live = false;

        if (cmp_data(birth, dead) > 0) {
            arr[size].time = dead;
        } else {
            arr[size].time = birth;
        }
        --arr[size].time.tm_mday;
        size++;
    }

    qsort(arr, size, sizeof(struct man_time), cmp);

    int res = 0;
    int max_size = 0;

    for (int i = 0; i < size; ++i) {
        if (arr[i].live)
            res++;
        else
            res--;
        if (res > max_size)
            max_size = res;
    }

    cout << max_size << endl;

    return 0;
}


