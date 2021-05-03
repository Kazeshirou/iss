#include <fmt/core.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include "sorts.h"

void generate_file(const std::string& filename, size_t bytes) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        fmt::print("Не удалось открыть файл {} для записи!\n", filename);
        exit(1);
    }

    size_t random_number;
    size_t n = bytes / sizeof(random_number);

    for (size_t i = 0; i < n; i++) {
        random_number = rand() % 100000;
        file.write(reinterpret_cast<const char*>(&random_number),
                   sizeof(random_number));
    }
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
    std::string input = "bin/input";
    size_t      size{15};
    fmt::print("Введите размер генерируемого файла (кол-во size_t):\n");
    std::cin >> size;
    generate_file(input, size * 8);
    std::size_t devices{5};
    fmt::print("Введите количество устройств:\n");
    std::cin >> devices;
    extended_sort_fibonacci(input, "bin/output", devices);
    return 0;
}