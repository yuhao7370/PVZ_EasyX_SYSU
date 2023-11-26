#include <chrono>

long long get_ms(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch().count();
    return (long long)(value);
}

void fillArray(int arr[], int size, int type = 0) {
    if(type == 1){
        const int counts[] = {8, 4, 4};
        const int values[] = {1, 2, 3};

        int index = 0;
        for (int i = 0; i < 3; ++i) {
            std::fill_n(arr + index, counts[i], values[i]);
            index += counts[i];
        }

        std::random_shuffle(arr, arr + size);
    }
    else{
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 10;
        }
        std::random_shuffle(arr, arr + size);
    }
}  