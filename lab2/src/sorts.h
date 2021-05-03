#pragma once

#include <functional>
#include <vector>

#include "statistic.h"

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

size_t print_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        fmt::print("Не удалось открыть файл {} для чтения!\n", filename);
        exit(1);
    }

    size_t random_number;
    size_t size{0};

    fmt::print("Файл {}:\n", filename);
    file.read(reinterpret_cast<char*>(&random_number), sizeof(random_number));
    while (!file.eof()) {
        if (size < 20) {
            fmt::print("{} ", random_number);
        }
        ++size;
        file.read(reinterpret_cast<char*>(&random_number),
                  sizeof(random_number));
    }
    if (size >= 20) {
        fmt::print(" ... ");
    }
    fmt::print("\n");
    return size;
}

struct distribution_t {
    std::vector<size_t> distribution;
    std::vector<size_t> empty_distribution;
};

distribution_t fibonacci_distribution(statistic_t& stat, size_t elements_count,
                                      size_t devices_count) {
    distribution_t distr{std::vector<size_t>(devices_count, 0),
                         std::vector<size_t>(devices_count, 0)};
    size_t         input_devices = devices_count - 1;
    distr.distribution[0]        = 1;
    size_t sum{0};
    while (true) {
        sum = 0;
        for (size_t i{0}; i < input_devices; i++) {
            sum += distr.distribution[i];
        }
        if (sum >= elements_count) {
            break;
        }
        size_t old0 = distr.distribution[0];
        size_t i{0};
        for (; i < input_devices - 1; i++) {
            distr.distribution[i] = distr.distribution[i + 1] + old0;
        }
        distr.distribution[i] = old0;
    }
    stat.empty_sequences = sum - elements_count;
    size_t empty         = stat.empty_sequences / input_devices;
    size_t empty_ext     = stat.empty_sequences % input_devices;
    for (size_t i = 0; i < input_devices; i++) {
        if (i < empty_ext) {
            distr.empty_distribution[i] = empty + 1;
        } else {
            distr.empty_distribution[i] = empty;
        }
    }
    return distr;
}

struct work_file_t {
    std::string         name;
    std::vector<size_t> seqs;
};

std::vector<work_file_t> distribute_input_to_work_files(
    const std::string input, const distribution_t& distr) {
    std::vector<work_file_t> files(distr.distribution.size());

    std::ifstream file(input, std::ios::binary);
    if (!file) {
        fmt::print("Не удалось открыть файл {} для чтения!\n", input);
        exit(1);
    }

    for (size_t i{0}; i < files.size(); i++) {
        files[i].name = std::string("bin/work_file") + std::to_string(i);
        std::ofstream work(files[i].name, std::ios::binary);
        if (!work.is_open()) {
            fmt::print("Не удалось открыть файл {} для записи!\n",
                       files[i].name);
            exit(1);
        }
        std::vector<size_t> values(distr.distribution[i] -
                                   distr.empty_distribution[i]);
        size_t              val;
        for (size_t j{0}; j < values.size(); j++) {
            file.read(reinterpret_cast<char*>(&val), sizeof(val));
            values[j] = val;
        }
        quick_sort(values);
        size_t j{0};
        for (; j < values.size(); j++) {
            work.write(reinterpret_cast<char*>(&values[j]), sizeof(values[j]));
            files[i].seqs.push_back(1);
        }
        for (; j < distr.distribution[i]; j++) {
            files[i].seqs.push_back(0);
        }
    }

    return files;
}

void merge_work_files(statistic_t& stat, std::vector<work_file_t>& works,
                      size_t& current_output) {
    std::vector<std::fstream> files(works.size());
    size_t                    min_not_nul_seq_count{0};
    size_t                    next_output;
    for (size_t i{0}; i < files.size(); i++) {
        if (i == current_output) {
            files[i].open(works[i].name, std::ios::out | std::ios::binary);
            if (!files[i].is_open()) {
                fmt::print("Не удалось открыть файл {} для записи!\n",
                           works[i].name);
                exit(1);
            }
            continue;
        }
        files[i].open(works[i].name, std::ios::in | std::ios::binary);
        if (!files[i].is_open()) {
            fmt::print("Не удалось открыть файл {} для чтения!\n",
                       works[i].name);
            exit(1);
        }

        size_t work_i_seq_size{works[i].seqs.size()};
        if (work_i_seq_size) {
            if (!min_not_nul_seq_count) {
                min_not_nul_seq_count = work_i_seq_size;
                next_output           = i;
            } else if (work_i_seq_size < min_not_nul_seq_count) {
                min_not_nul_seq_count = work_i_seq_size;
                next_output           = i;
            }
        }
    }

    for (size_t i{0}; i < min_not_nul_seq_count; i++) {
        std::vector<bool>   has_values(works.size(), false);
        std::vector<size_t> current_values(works.size());
        works[current_output].seqs.push_back(0);
        while (true) {
            bool   has_value{false};
            size_t current_min;
            for (size_t j{0}; j < works.size(); j++) {
                if (j == current_output) {
                    continue;
                }
                if (has_values[j]) {
                    has_value   = true;
                    current_min = j;
                    continue;
                }
                if (works[j].seqs[i]) {
                    files[j].read(reinterpret_cast<char*>(&current_values[j]),
                                  sizeof(&current_values[j]));
                    stat.read++;
                    works[j].seqs[i]--;
                    has_value     = true;
                    has_values[j] = true;
                    current_min   = j;
                }
            }
            if (!has_value) {
                break;
            }

            for (size_t j{0}; j < works.size(); j++) {
                if (j == current_output) {
                    continue;
                }
                if (has_values[j] &&
                    (current_values[j] <= current_values[current_min])) {
                    current_min = j;
                    stat.operations++;
                }
            }

            works[current_output].seqs[i]++;
            stat.write++;
            files[current_output].write(
                reinterpret_cast<char*>(&current_values[current_min]),
                sizeof(&current_values[current_min]));
            has_values[current_min] = false;
        }
    }

    for (size_t i{0}; i < works.size(); i++) {
        if (i == current_output) {
            continue;
        }
        works[i].seqs.erase(works[i].seqs.begin(),
                            works[i].seqs.begin() + min_not_nul_seq_count);
        if (i == next_output) {
            continue;
        }
        if (!works[i].seqs.size()) {
            std::filesystem::remove(works[i].name);
            works.erase(works.begin() + i);
            if (next_output > i) {
                next_output--;
            }
            if (current_output > i) {
                current_output--;
            }
            i--;
            continue;
        }
        std::string   tmp_filename("bin/tmp");
        std::ofstream tmp(tmp_filename, std::ios::binary);
        if (!tmp.is_open()) {
            fmt::print("Не удалось открыть файл {} для записи!\n",
                       tmp_filename);
            exit(1);
        }
        tmp << files[i].rdbuf();
        files[i].close();
        std::filesystem::rename(tmp_filename, works[i].name);
    }
    (void)stat;
    current_output = next_output;
}

void extended_sort_fibonacci(const std::string& input,
                             const std::string& output, size_t devices_count) {
    statistic_t stat;
    auto        size = print_file(input);
    fmt::print("Количество элементов: {}\n", size);
    auto distribution = fibonacci_distribution(stat, size, devices_count);

    auto work_files = distribute_input_to_work_files(input, distribution);
    for (const auto& work : work_files) {
        auto size = print_file(work.name);
        fmt::print("Количество [ненулевых | нулевых | всего] элементов: {} | "
                   "{} | {}\n",
                   size, work.seqs.size() - size, work.seqs.size());
    }

    size_t current_output = work_files.size() - 1;
    fmt::print("\n\n");
    fmt::print("Шаг {}:\n", stat.steps);
    for (const auto& work : work_files) {
        auto size = print_file(work.name);
        fmt::print("{}: ", size);
        for (const auto& seq : work.seqs) {
            fmt::print("[{}] ", seq);
        }
        fmt::print("\n");
    }
    fmt::print("\n");
    while (work_files.size() > 2) {
        merge_work_files(stat, work_files, current_output);
        stat.steps++;
        fmt::print("Шаг {}:\n", stat.steps);
        for (const auto& work : work_files) {
            auto size = print_file(work.name);
            fmt::print("{}: ", size);
            for (const auto& seq : work.seqs) {
                fmt::print("[{}] ", seq);
            }
            fmt::print("\n");
        }
        fmt::print("\n");
    }
    std::filesystem::remove(work_files[current_output].name);
    std::filesystem::rename(work_files[current_output ? 0 : 1].name, output);

    fmt::print("\n");
    size = print_file(output);
    fmt::print("Количество элементов: {}\n", size);
    fmt::print("\nstat.empty_sequences = {}\nstat.steps = {}\nstat.operations "
               "= {}\nstat.read = {}\nstat.write = {}\n\n",
               stat.empty_sequences, stat.steps, stat.operations, stat.read,
               stat.write);
}