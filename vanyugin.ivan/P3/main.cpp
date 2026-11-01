#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace vanyugin {

  const int expected_argc = 4;
  const int err_args = 1;
  const int err_data = 2;
  const int mode_stack = 1;
  const int mode_dynamic = 2;
  const int max_stack_size = 10000;
  const int min = 2147483647;

  int minsumparalsecdiag(const int *matrix, int rows_num, int columns_num)
  {
    if (rows_num <= 0 || columns_num <= 0) {
      return 0;
    }

    int min_sum = min;
    bool found = false;

    for (int k = 0; k < rows_num + columns_num - 1; ++k) {
      if (rows_num == columns_num && k == rows_num - 1) {
        continue;
      }

      int cnt = 0;
      bool has_elements = false;

      for (int r = 0; r < rows_num; ++r) {
        const int c = k - r;

        if (c >= 0 && c < columns_num) {
          const int linear_index = r * columns_num + c;

          cnt += matrix[linear_index];
          has_elements = true;
        }
      }

      if (has_elements && cnt < min_sum) {
        min_sum = cnt;
        found = true;
      }
    }

    return found ? min_sum : 0;
  }

}

int main(int argc, char *argv[])
{
  if (argc < vanyugin::expected_argc) {
    std::cerr << "Not enough arguments\n";
    return vanyugin::err_args;
  }
  if (argc > vanyugin::expected_argc) {
    std::cerr << "Too many arguments\n";
    return vanyugin::err_args;
  }

  int ter{0};

  try {
    std::size_t char_num{0};
    const std::string arg_str(argv[1]);
    ter = std::stoi(arg_str, &char_num);

    if (arg_str.length() != char_num) {
      std::cerr << "First parameter is not a number\n";
      return vanyugin::err_args;
    }
  }

  catch (const std::exception &) {
    std::cerr << "First parameter is not a number\n";
    return vanyugin::err_args;
  }

  if (ter != vanyugin::mode_stack && ter != vanyugin::mode_dynamic) {
    std::cerr << "First parameter is out of range\n";
    return vanyugin::err_args;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file\n";
    return vanyugin::err_data;
  }

  int rows_num{0};
  int columns_num{0};
  if (!(input >> rows_num >> columns_num)) {
    std::cerr << "Invalid matrix dimensions\n";
    return vanyugin::err_data;
  }

  if (rows_num < 0 || columns_num < 0) {
    std::cerr << "Invalid matrix dimensions\n";
    return vanyugin::err_data;
  }

  std::ofstream out(argv[3]);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    return vanyugin::err_data;
  }

  const int sum = rows_num * columns_num;
  int stack_matrix[vanyugin::max_stack_size];
  int *dynamic_matrix = nullptr;
  int *matrix = nullptr;

  if (ter == vanyugin::mode_stack) {
    if (sum > vanyugin::max_stack_size) {
      std::cerr << "Matrix rxceeds max stack size\n";
      return vanyugin::err_data;
    }
    matrix = stack_matrix;
  } else {
    dynamic_matrix = new int[sum];
    matrix = dynamic_matrix;
  }

  for (int i = 0; i < sum; ++i) {
    if (!(input >> matrix[i])) {
      delete[] dynamic_matrix;
      std::cerr << "Invalid matrix data\n";
      return vanyugin::err_data;
    }
  }

  const int minsum = vanyugin::minsumparalsecdiag(matrix, rows_num, columns_num);
  delete[] dynamic_matrix;

  out << minsum << "\n";
  return 0;
}
