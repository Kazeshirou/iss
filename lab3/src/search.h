#pragma once

#include <list>
#include <vector>

#include "statistic.h"

template <class T>
typename std::vector<T>::iterator linear_search(statistic_t&    statistic,
                                                std::vector<T>& data,
                                                const T&        target_value) {
    statistic = statistic_t{};
    for (auto it = data.begin(); it != data.end(); it++) {
        statistic.comparisons++;
        if (*it == target_value) {
            return it;
        }
    }
    return data.end();
}

template <class T>
typename std::vector<T>::iterator binary_search(statistic_t&    statistic,
                                                std::vector<T>& data,
                                                const T&        target_value) {
    statistic  = statistic_t{};
    auto begin = data.begin();
    auto end   = data.end();
    while (true) {
        auto it = begin + (end - begin) / 2;
        statistic.comparisons++;
        if (*it == target_value) {
            return it;
        }
        if ((end - begin) == 1) {
            break;
        }
        statistic.comparisons++;
        if (target_value < *it) {
            end = it;
        } else {
            begin = it;
        }
    }
    return data.end();
}

template <class T>
typename std::vector<T>::iterator hash_search(statistic_t&    statistic,
                                              std::vector<T>& data,
                                              const T&        target_value) {
    statistic  = statistic_t{};
    auto begin = data.begin();
    auto end   = data.end();
    while (true) {
        auto it = begin + (end - begin) / 2;
        statistic.comparisons++;
        if (*it == target_value) {
            return it;
        }
        if ((end - begin) == 1) {
            break;
        }
        statistic.comparisons++;
        if (target_value < *it) {
            end = it;
        } else {
            begin = it;
        }
    }
    return data.end();
}

template <class T>
class Hash_table {
public:
    Hash_table() : data_(256) {}

    Hash_table(const std::vector<T>& vector) : data_(vector.size()) {
        for (auto v : vector) {
            append(v);
        }
    }

    T* find(statistic_t& stats, T target) {
        stats  = statistic_t{};
        auto h = hash(target);
        if (!data_[h].size()) {
            return nullptr;
        }
        stats.collisions += data_[h].size() - 1;
        for (auto& value : data_[h]) {
            stats.comparisons++;
            if (value == target) {
                return &value;
            }
        }
        return nullptr;
    }

    void append(T value) {
        real_size_++;
        data_[hash(value)].push_back(value);
    }

    size_t size() const {
        return real_size_;
    }

private:
    size_t hash(const T& value) {
        return int(value) % data_.size();
    }
    std::vector<std::list<T>> data_;
    size_t                    real_size_{0};
};

template <class T>
Hash_table(const std::vector<T>&) -> Hash_table<T>;