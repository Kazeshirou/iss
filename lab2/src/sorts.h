#pragma once

#include <functional>
#include <vector>

template <class T>
void quick_sort(std::vector<T>& data) {
    auto partition = [&data](size_t low, size_t high) {
        size_t i     = low;
        size_t j     = high - 1;
        T      pivot = data[(low + high) / 2];
        T      tmp;
        while (true) {
            if (i >= j) {
                break;
            }
            while ((i < high - 1) && (data[i] < pivot)) {
                i++;
            }

            if (i >= j) {
                break;
            }
            while ((j > low) && (data[j] > pivot)) {
                j--;
            }
            if (i > j) {
                break;
            }
            tmp     = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
        }
        return i;
    };

    std::function<void(size_t, size_t)> sort =
        [&data, &partition, &sort](size_t low, size_t high) -> void {
        if (high <= low) {
            return;
        }
        if (high - low == 1) {
            return;
        }
        size_t j = partition(low, high);
        sort(low, j);
        sort(j, high);
    };

    sort(0, data.size());
}
