#include <iostream>
#include <cmath>

using namespace std;

double spaceTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    double space = fabs((x1 - x3) * (y2 - y3) -
            (x2 - x3) * (y1 - y3)) / 2;
    return space;
}

int main() {
    int N;
    cin >> N;

    double res = 0;
    double x1, y1;
    double x2, y2;
    double x3, y3;

    cin >> x1 >> y1;
    cin >> x2 >> y2;

    for (int i = 0; i < N - 2; i++) {
        cin >> x3 >> y3;
        res += spaceTriangle(x1, y1, x2, y2, x3, y3);
        x2 = x3;
        y2 = y3;
    }
    cout << res;
    return 0;
}

