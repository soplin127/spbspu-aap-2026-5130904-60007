#include <cstddef>
#include <fstream>
#include <iostream>
#include <new>

namespace yilman {
  constexpr std::size_t max_fixed_size = 10000;

  bool parseTaskNumber(const char *str, int &num);
  bool readMatrix(std::istream &in, int *data, std::size_t rows, std::size_t cols);
  long long minSumAntiDiagonal(const int *data, std::size_t rows, std::size_t cols);
  bool writeResult(const char *filename, long long result);
}

bool yilman::parseTaskNumber(const char *str, int &num)
{
  if (str == nullptr) {
    return false;
  }
  if (str[0] == '\0' || str[1] != '\0') {
    return false;
  }
  if (str[0] == '1') {
    num = 1;
    return true;
  }
  if (str[0] == '2') {
    num = 2;
    return true;
  }
  return false;
}

bool yilman::readMatrix(std::istream &in, int *data, std::size_t rows, std::size_t cols)
{
  if (data == nullptr) {
    return false;
  }
  for (std::size_t i = 0; i < rows * cols; ++i) {
    in >> data[i];
    if (!in) {
      return false;
    }
  }
  return true;
}

long long yilman::minSumAntiDiagonal(const int *data, std::size_t rows, std::size_t cols)
{
  if (data == nullptr) {
    return 0;
  }
  const std::size_t n = rows < cols ? rows : cols;
  if (n < 2) {
    return 0;
  }
  bool found = false;
  long long min_sum = 0;
  for (std::size_t k = 0; k < 2 * n - 1; ++k) {
    if (k == n - 1) {
      continue;
    }
    long long sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
      if (k >= i && k - i < n) {
        sum += data[i * cols + (k - i)];
      }
    }
    if (!found || sum < min_sum) {
      min_sum = sum;
      found = true;
    }
  }
  return min_sum;
}

bool yilman::writeResult(const char *filename, long long result)
{
  if (filename == nullptr) {
    return false;
  }
  std::ofstream out(filename);
  if (!out) {
    return false;
  }
  out << result << "\n";
  return static_cast< bool >(out);
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  int num = 0;
  if (!yilman::parseTaskNumber(argv[1], num)) {
    std::cerr << "First parameter is out of range or not a number\n";
    return 1;
  }

  std::ifstream in(argv[2]);
  if (!in) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }
  int rows_in = 0;
  int cols_in = 0;
  in >> rows_in >> cols_in;
  if (!in || rows_in < 0 || cols_in < 0) {
    std::cerr << "Input is not a matrix\n";
    return 2;
  }
  const std::size_t rows = static_cast< std::size_t >(rows_in);
  const std::size_t cols = static_cast< std::size_t >(cols_in);

  long long result = 0;
  if (num == 1) {
    if (rows * cols > yilman::max_fixed_size) {
      std::cerr << "Matrix is too big\n";
      return 2;
    }
    int data[yilman::max_fixed_size] = {};
    if (!yilman::readMatrix(in, data, rows, cols)) {
      std::cerr << "Input is not a matrix\n";
      return 2;
    }
    result = yilman::minSumAntiDiagonal(data, rows, cols);
  } else {
    int *data = nullptr;
    try {
      data = new int[rows * cols];
    } catch (const std::bad_alloc &) {
      std::cerr << "Not enough memory\n";
      return 2;
    }
    if (!yilman::readMatrix(in, data, rows, cols)) {
      delete[] data;
      std::cerr << "Input is not a matrix\n";
      return 2;
    }
    result = yilman::minSumAntiDiagonal(data, rows, cols);
    delete[] data;
  }

  if (!yilman::writeResult(argv[3], result)) {
    std::cerr << "Cannot write output file\n";
    return 2;
  }
  return 0;
}
