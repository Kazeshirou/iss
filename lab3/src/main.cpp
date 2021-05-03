#include <fmt/core.h>
#include <ctime>
#include <iostream>
#include <random>

#include "search.h"
#include "sorts.h"


template <class T>
std::vector<T> generate_vector(size_t size) {
    std::vector<int> vector(size);
    for (auto& el : vector) {
        el = rand();
    }
    return vector;
}


template <class T>
void print_vector(const std::vector<T>& data) {
    for (const auto& el : data) {
        std::cout << el << "\t";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::srand(std::time(nullptr));

    size_t size   = 1025;
    auto   vector = generate_vector<int>(size);
    quick_sort(vector);

    std::cout << "Исходный массив:\n";
    print_vector(vector);
    std::cout << "\n";

    std::vector<int> targets = {vector[0], vector[size / 2], vector[size - 1],
                                vector[size - 1] + 5};
    std::vector<std::string> targets_labels = {
        "первый элемент", "средний элемент", "последний элемент",
        "элемент, которого нет в массиве"};
    statistic_t stats;
    std::cout << "Линейный поиск:\n";
    for (size_t i{0}; i < targets.size(); i++) {
        auto res = linear_search(stats, vector, targets[i]);
        if (res != vector.end()) {
            fmt::print(
                "\tИщется {}({}): найденный элемент - {}, сравнений - {}\n",
                targets_labels[i], targets[i], *res, stats.comparisons);
        } else {
            fmt::print("\tИщется {}({}): элемент не найден, сравнений - {}\n",
                       targets_labels[i], targets[i], stats.comparisons);
        }
    }
    float comparisons_av{0};
    for (auto target : vector) {
        auto res = linear_search(stats, vector, target);
        if ((res == vector.end()) || (*res != target)) {
            std::cout << "\tНе найден элемент, представленный в массиве!\n";
            exit(1);
        }
        comparisons_av += stats.comparisons;
    }
    comparisons_av /= float(size);
    fmt::print("\tСреднее количество сравнений - {}\n\n", comparisons_av);

    std::cout << "Бинарный поиск:\n";
    for (size_t i{0}; i < targets.size(); i++) {
        auto res = binary_search(stats, vector, targets[i]);
        if (res != vector.end()) {
            fmt::print(
                "\tИщется {}({}): найденный элемент - {}, сравнений - {}\n",
                targets_labels[i], targets[i], *res, stats.comparisons);
        } else {
            fmt::print("\tИщется {}({}): элемент не найден, сравнений - {}\n",
                       targets_labels[i], targets[i], stats.comparisons);
        }
    }
    comparisons_av = 0;
    for (auto target : vector) {
        auto res = binary_search(stats, vector, target);
        if ((res == vector.end()) || (*res != target)) {
            std::cout << "\tНе найден элемент, представленный в массиве!\n";
            exit(1);
        }
        comparisons_av += stats.comparisons;
    }
    comparisons_av /= float(size);
    fmt::print("\tСреднее количество сравнений - {}\n\n", comparisons_av);

    std::cout << "Поиск в хэш-таблице:\n";
    Hash_table hash_table(vector);
    for (size_t i{0}; i < targets.size(); i++) {
        auto res = hash_table.find(stats, targets[i]);
        if (res) {
            fmt::print("\tИщется {}({}): найденный элемент - {}, сравнений - "
                       "{}, коллизий - {}\n",
                       targets_labels[i], targets[i], *res, stats.comparisons,
                       stats.collisions);
        } else {
            fmt::print("\tИщется {}({}): элемент не найден, сравнений - {}, "
                       "коллизий - {}\n",
                       targets_labels[i], targets[i], stats.comparisons,
                       stats.collisions);
        }
    }
    comparisons_av      = 0;
    float collisions_av = 0;
    for (auto target : vector) {
        auto res = hash_table.find(stats, target);
        if (!res || (*res != target)) {
            std::cout << "\tНе найден элемент, представленный в массиве!\n";
            exit(1);
        }
        comparisons_av += stats.comparisons;
        collisions_av += stats.collisions;
    }
    comparisons_av /= float(size);
    collisions_av /= float(size);
    fmt::print("\tСреднее количество сравнений - {}, среднее количество "
               "коллизий - {}\n\n",
               comparisons_av, collisions_av);

    return 0;
}