#include <iostream>
#include <cstdlib>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc < 4) {
    std::cerr << "Not enough arguments" << "\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments" << "\n";
    return 1;
  }
  char* endptr = nullptr;
  const long num = std::strtol(argv[1], &endptr, 10);
  if (*endptr != '\0' || endptr == argv[1]) {
    std::cerr << "First parameter is not a number" << "\n";
    return 1;
  }
  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range" << "\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);

  if (!input.is_open()) {
    std::cerr << "Cannot open input file" << "\n";
    return 2;
  }
  if (!output.is_open()) {
    std::cerr << "Cannot open output file" << "\n";
    return 2;
  }

  int rows = 0;
  int cols = 0;

  if (!(input >> rows >> cols)) {
    std::cerr << "Cannot read matrix dimensions" << "\n";
    return 2;
  }

  if (rows < 0 || cols < 0) {
    std::cerr << "Invalid matrix dimensions" << "\n";
    return 2;
  }
  if (rows == 0 || cols == 0) {
    return 0;
  }

  const int max_size = 100;
  int static_matrix[max_size][max_size];
  int** dynamic_matrix = nullptr;
  int* data = nullptr;

  if (num == 1) {
    if (rows > max_size || cols > max_size) {
      std::cerr << "Matrix is too big for static array" << "\n";
      return 2;
    }
  } else if (num == 2) {
    dynamic_matrix = new int*[rows];
    data = new int[rows * cols];

    for (int i = 0; i < rows; ++i) {
      dynamic_matrix[i] = data + (i * cols);
    }
  }

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int value = 0;
      if (!(input >> value)) {
        std::cerr << "Invalid data" << "\n";
        if (num == 2) {
          delete[] data;
          delete[] dynamic_matrix;
        }
        return 2;
      }
      if (num == 2) {
        dynamic_matrix[i][j] = value;
      } else {
        static_matrix[i][j] = value;
      }
    }
  }
  int max_sum = 0;
  bool is_first = true;

  for (int diff = -cols; diff <= rows; ++diff) {
    if (diff != 0) {
      int current_sum = 0;

      for (int i = 0; i < rows; ++i) {
        const int j = i - diff;

        if (j >= 0 && j < cols) {
          if (num == 1) {
            current_sum += static_matrix[i][j];
          } else {
            current_sum += dynamic_matrix[i][j];
          }
        }
      }
      if (is_first) {
        max_sum = current_sum;
        is_first = false;
      } else if (current_sum > max_sum) {
        max_sum = current_sum;
      }
    }
  }
  output << max_sum << "\n";

  if (num == 2) {
    delete[] data;
    delete[] dynamic_matrix;
  }
  return 0;
}
