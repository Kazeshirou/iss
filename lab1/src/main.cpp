#include <iostream>
#include <random>

#include "sorts.h"

template <class T>
void print_vector(const std::vector<T>& data) {
    for (const auto& el : data) {
        std::cout << el << "\t";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<size_t>      sizes = {40, 20, 10};
    std::vector<sort_t<int>> sorts = {
        {"Сортировка пузырьком", &bubble_sort<int>},
        {"Сортировка вставками", &insert_sort<int>},
        {"Быстрая сортировка", &quick_sort<int>},
    };

    std::vector<int> common_vector(sizes[0]);
    for (auto& el : common_vector) {
        el = rand() % 1000;
    }

    std::cout << "Исходный массив:\n";
    print_vector(common_vector);
    std::cout << "\n\n";

    statistic_t res;
    for (size_t i = 0; i < sizes.size(); i++) {
        std::vector<int> cur_size_vector;
        std::cout << "------ Сортировка массивов размера " << sizes[i]
                  << std::endl;
        for (size_t j = 0; j < sizes[i]; j++) {
            cur_size_vector.push_back(common_vector[j]);
        }
        for (size_t j = 0; j < sorts.size(); j++) {
            std::cout << "--- " << sorts[j].name << std::endl;
            std::vector<int> current_vector = cur_size_vector;
            std::cout << "Элементы в случайном порядке: ";
            res = sorts[j].alg(current_vector);
            print_vector(current_vector);
            std::cout << "сравнений " << res.comparisons << ", перестановок "
                      << res.transposition << std::endl;

            std::cout << "Элементы упорядочены (прямой порядок): ";
            res = sorts[j].alg(current_vector);
            std::cout << "сравнений " << res.comparisons << ", перестановок "
                      << res.transposition << std::endl;
            print_vector(current_vector);

            for (auto& el : current_vector) {
                el *= -1;
            }
            std::cout << "Элементы упорядочены (обратный порядок): ";
            res = sorts[j].alg(current_vector);
            std::cout << "сравнений " << res.comparisons << ", перестановок "
                      << res.transposition << std::endl;
            print_vector(current_vector);
        }
        std::cout << std::endl;
    }

    return 0;
}