#include <fstream>
#include <iostream>
#include <new>
#include <string>

namespace mukhametdinov {

  int countColsWithoutConsecutive(const int *matrix, int rows, int cols)
  {
    int count = 0;
    for (int j = 0; j < cols; ++j) {
      bool has_consecutive = false;
      for (int i = 0; i < rows - 1; ++i) {
        if (matrix[i * cols + j] == matrix[(i + 1) * cols + j]) {
          has_consecutive = true;
          break;
        }
      }
      if (!has_consecutive) {
        ++count;
      }
    }
    return count;
  }

  int run(int argc, char *argv[])
  {
    const int argc_count = 4;
    const int max_elements = 10000;

    if (argc < argc_count) {
      std::cerr << "Not enough arguments\n";
      return 1;
    }
    if (argc > argc_count) {
      std::cerr << "Too many arguments\n";
      return 1;
    }

    const std::string mode_str = argv[1];
    if (mode_str != "1" && mode_str != "2") {
      std::cerr << "First parameter is not a number\n";
      return 1;
    }
    const int num = std::stoi(mode_str);

    std::ifstream input(argv[2]);
    if (!input) {
      std::cerr << "Input file not found\n";
      return 1;
    }

    int rows = 0;
    int cols = 0;
    input >> rows >> cols;
    if (!input) {
      std::cerr << "Invalid input file format\n";
      return 2;
    }

    if (rows < 0 || cols < 0) {
      std::cerr << "Invalid matrix dimensions\n";
      return 2;
    }
    if ((rows == 0 && cols > 0) || (rows > 0 && cols == 0)) {
      std::cerr << "Invalid matrix dimensions\n";
      return 2;
    }

    int fixed[max_elements];
    int *matrix = nullptr;

    if (num == 1) {
      if (rows * cols > max_elements) {
        std::cerr << "Matrix size exceeds stack limit\n";
        return 2;
      }
      matrix = fixed;
    } else {
      matrix = new (std::nothrow) int[rows * cols];
      if (!matrix) {
        std::cerr << "Memory allocation failed\n";
        return 2;
      }
    }

    for (int i = 0; i < rows * cols; ++i) {
      input >> matrix[i];
      if (!input) {
        std::cerr << "Invalid input file format\n";
        if (num == 2) {
          delete[] matrix;
        }
        return 2;
      }
    }

    char trailing = '\0';
    if (input >> trailing) {
      std::cerr << "Extra data in input file\n";
      if (num == 2) {
        delete[] matrix;
      }
      return 2;
    }

    std::ofstream output(argv[3]);
    if (!output) {
      std::cerr << "Failed to open output file\n";
      if (num == 2) {
        delete[] matrix;
      }
      return 1;
    }

    output << countColsWithoutConsecutive(matrix, rows, cols) << '\n';

    if (num == 2) {
      delete[] matrix;
    }

    return 0;
  }

}

int main(int argc, char *argv[])
{
  return mukhametdinov::run(argc, argv);
}
