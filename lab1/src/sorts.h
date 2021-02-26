#pragma once

#include <functional>
#include <vector>

#include "statistic.h"

template <class T>
struct sort_t {
    const char*                                 name;
    std::function<statistic_t(std::vector<T>&)> alg;
};

template <class T>
statistic_t bubble_sort(std::vector<T>& data) {
    statistic_t stat;
    size_t      size = data.size();
    T           tmp;
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            stat.comparisons++;
            if (data[j] > data[j + 1]) {
                stat.transposition++;
                tmp         = data[j];
                data[j]     = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }
    return stat;
}

template <class T>
statistic_t insert_sort(std::vector<T>& data) {
    statistic_t stat;

    T tmp;
    for (size_t i = 1; i < data.size(); ++i) {
        stat.comparisons++;
        for (size_t j = i; j != 0 && data[j] < data[j - 1]; --j) {
            stat.comparisons++;
            stat.transposition++;
            tmp         = data[j - 1];
            data[j - 1] = data[j];
            data[j]     = tmp;
        }
    }
    return stat;
}

template <class T>
statistic_t quick_sort(std::vector<T>& data) {
    statistic_t stat;

    auto partition = [&stat, &data](size_t low, size_t high) {
        size_t i     = low;
        size_t j     = high - 1;
        T      pivot = data[(low + high) / 2];
        T      tmp;
        while (true) {
            if (i >= j) {
                break;
            }
            stat.comparisons++;
            while ((i < high - 1) && (data[i] < pivot)) {
                i++;
                stat.comparisons++;
            }

            if (i >= j) {
                break;
            }
            stat.comparisons++;
            while ((j > low) && (data[j] > pivot)) {
                j--;
                stat.comparisons++;
            }
            if (i > j) {
                break;
            }
            stat.transposition++;
            tmp     = data[i];
            data[i] = data[j];
            data[j] = tmp;
            i++;
        }
        return i;
    };

    std::function<void(size_t, size_t)> sort =
        [&stat, &data, &partition, &sort](size_t low, size_t high) -> void {
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

    return stat;
}
