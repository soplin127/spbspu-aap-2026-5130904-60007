#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>

namespace devret
{
  const size_t max_static_size = 10000;

  bool readElements(std::istream &in, int *data, size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      if (!(in >> data[i]))
      {
        return false;
      }
    }
    return true;
  }

  size_t countRows(const int *data, size_t rows, size_t cols)
  {
    size_t result = 0;
    for (size_t i = 0; i < rows; ++i)
    {
      bool has_equal = false;
      for (size_t j = 1; j < cols && !has_equal; ++j)
      {
        has_equal = (data[i * cols + j] == data[i * cols + j - 1]);
      }
      if (!has_equal)
      {
        ++result;
      }
    }
    return result;
  }

  bool writeResult(const char *path, size_t result)
  {
    std::ofstream out(path);
    out << result << "\n";
    return static_cast< bool >(out);
  }
}

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " num input output\n";
    return 1;
  }

  char *end = nullptr;
  const long mode = std::strtol(argv[1], &end, 10);
  if (end == argv[1] || *end != '\0' || (mode != 1 && mode != 2))
  {
    std::cerr << "Error: num must be 1 or 2\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Error: cannot open input file\n";
    return 2;
  }

  long long rows = 0;
  long long cols = 0;
  if (!(input >> rows >> cols) || rows < 0 || cols < 0)
  {
    std::cerr << "Error: invalid matrix size\n";
    return 2;
  }

  const size_t r = static_cast< size_t >(rows);
  const size_t c = static_cast< size_t >(cols);
  if (r != 0 && c > devret::max_static_size / r && mode == 1)
  {
    std::cerr << "Error: matrix is too large\n";
    return 2;
  }
  const size_t count = r * c;

  size_t result = 0;
  if (mode == 1)
  {
    int matrix[devret::max_static_size] = {};
    if (!devret::readElements(input, matrix, count))
    {
      std::cerr << "Error: invalid matrix elements\n";
      return 2;
    }
    result = devret::countRows(matrix, r, c);
  }
  else
  {
    int *matrix = nullptr;
    try
    {
      matrix = new int[count];
    }
    catch (const std::bad_alloc &)
    {
      std::cerr << "Error: not enough memory\n";
      return 2;
    }
    if (!devret::readElements(input, matrix, count))
    {
      delete[] matrix;
      std::cerr << "Error: invalid matrix elements\n";
      return 2;
    }
    result = devret::countRows(matrix, r, c);
    delete[] matrix;
  }

  if (!devret::writeResult(argv[3], result))
  {
    std::cerr << "Error: cannot write output file\n";
    return 2;
  }
  return 0;
}
