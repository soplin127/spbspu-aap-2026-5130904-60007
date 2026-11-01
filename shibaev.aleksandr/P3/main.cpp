#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <exception>
#include <sstream>

namespace shibaev {
  const int expected_argc = 4;
  const int argument_error = 1;
  const int data_error = 2;
  const int stack_mode = 1;
  const int dynamic_mode = 2;
  const std::size_t max_stack_size = 10000;
  const std::size_t border_width = 1;
  const std::size_t min_dimension = 3;

  std::size_t countLocalMinima(const int *matrix, std::size_t rows, std::size_t columns);
}

std::size_t shibaev::countLocalMinima(const int *matrix, std::size_t rows, std::size_t columns)
{
  if ((rows < min_dimension) || (columns < min_dimension)) {
    return 0;
  }

  std::size_t count = 0;
  for (std::size_t row = border_width; row < rows - border_width; ++row) {
    for (std::size_t column = border_width; column < columns - border_width; ++column) {
      bool is_minimum = true;
      for (std::size_t i = row - border_width; (i <= row + border_width) && is_minimum; ++i) {
        for (std::size_t j = column - border_width; j <= column + border_width; ++j) {
          if ((i == row) && (j == column)) {
            continue;
          }
          if (matrix[row * columns + column] >= matrix[i * columns + j]) {
            is_minimum = false;
            break;
          }
        }
      }
      if (is_minimum) {
        ++count;
      }
    }
  }
  return count;
}

int main(int argc, char *argv[])
{
  if (argc < shibaev::expected_argc) {
    std::cerr << "Not enough arguments\n";
    return shibaev::argument_error;
  }
  if (argc > shibaev::expected_argc) {
    std::cerr << "Too many arguments\n";
    return shibaev::argument_error;
  }

  int mode = 0;
  std::stringstream argument(argv[1]);
  if (!(argument >> mode) || !argument.eof()) {
    std::cerr << "First parameter is not a number\n";
    return shibaev::argument_error;
  }
  if ((mode != shibaev::stack_mode) && (mode != shibaev::dynamic_mode)) {
    std::cerr << "First parameter is out of range\n";
    return shibaev::argument_error;
  }

  std::ifstream file(argv[2]);
  if (!file.is_open()) {
    std::cerr << "Cannot open input file\n";
    return shibaev::data_error;
  }

  int rows_number = 0;
  int columns_number = 0;
  if (!(file >> rows_number >> columns_number)) {
    std::cerr << "Invalid matrix dimensions\n";
    return shibaev::data_error;
  }
  if ((rows_number < 0) || (columns_number < 0)) {
    std::cerr << "Dimensions cannot be negative\n";
    return shibaev::data_error;
  }

  const std::size_t rows = static_cast< std::size_t >(rows_number);
  const std::size_t columns = static_cast< std::size_t >(columns_number);
  const std::size_t size_limit = static_cast< std::size_t >(-1);
  if ((columns != 0) && (rows > size_limit / sizeof(int) / columns)) {
    std::cerr << "Matrix dimensions are too large\n";
    return shibaev::data_error;
  }
  const std::size_t total = rows * columns;

  int stack_matrix[shibaev::max_stack_size] = {};
  int *dynamic_matrix = nullptr;
  int *matrix = stack_matrix;
  if (mode == shibaev::stack_mode) {
    if (total > shibaev::max_stack_size) {
      std::cerr << "Matrix exceeds max stack size\n";
      return shibaev::data_error;
    }
  } else {
    try {
      dynamic_matrix = new int[total]{};
    } catch (const std::exception &) {
      std::cerr << "Cannot allocate matrix\n";
      return shibaev::data_error;
    }
    matrix = dynamic_matrix;
  }

  for (std::size_t i = 0; i < total; ++i) {
    if (!(file >> matrix[i])) {
      delete[] dynamic_matrix;
      std::cerr << "Invalid matrix data\n";
      return shibaev::data_error;
    }
  }

  std::string extra;
  if (file >> extra) {
    delete[] dynamic_matrix;
    std::cerr << "Extra data in file\n";
    return shibaev::data_error;
  }
  if (!file.eof() || file.bad()) {
    delete[] dynamic_matrix;
    std::cerr << "Cannot read input file\n";
    return shibaev::data_error;
  }

  const std::size_t count = shibaev::countLocalMinima(matrix, rows, columns);
  delete[] dynamic_matrix;

  std::ofstream out(argv[3]);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    return shibaev::data_error;
  }
  out << count << '\n';
  out.close();
  if (!out) {
    std::cerr << "Cannot write output file\n";
    return shibaev::data_error;
  }
  return 0;
}
