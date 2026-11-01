#include <cstddef>
#include <fstream>
#include <iostream>
#include <new>

#define ERROR_BAD_ARGS 1
#define ERROR_BAD_DATA 2
#define FIXED_CAPACITY 10000

namespace himich
{
  using Elem = long long;

  int parseMode(const char * s)
  {
    if ((s[0] == '1' || s[0] == '2') && (s[1] == '\0'))
    {
      return s[0] - '0';
    }
    return 0;
  }

  bool readElements(std::istream & in, Elem * data, std::size_t n)
  {
    for (std::size_t i{0}; i < n; ++i)
    {
      int v{0};
      if (!(in >> v))
      {
        return false;
      }
      data[i] = v;
    }
    in >> std::ws;
    return in.eof();
  }

  void transform(Elem * m, std::size_t rows, std::size_t cols)
  {
    if ((rows == 0) || (cols == 0))
    {
      return;
    }
    long long top{0};
    long long bottom{static_cast< long long >(rows) - 1};
    long long left{0};
    long long right{static_cast< long long >(cols) - 1};
    Elem k{1};

    while ((top <= bottom) && (left <= right))
    {
      for (long long i{top}; i <= bottom; ++i)
      {
        m[i * cols + left] += k++;
      }
      ++left;
      if (left > right)
      {
        break;
      }

      for (long long j{left}; j <= right; ++j)
      {
        m[bottom * cols + j] += k++;
      }
      --bottom;
      if (top > bottom)
      {
        break;
      }

      for (long long i{bottom}; i >= top; --i)
      {
        m[i * cols + right] += k++;
      }
      --right;
      if (left > right)
      {
        break;
      }

      for (long long j{right}; j >= left; --j)
      {
        m[top * cols + j] += k++;
      }
      ++top;
    }
  }

  void print(std::ostream & out, const Elem * m, std::size_t rows, std::size_t cols)
  {
    out << rows << ' ' << cols;
    for (std::size_t i{0}; i < rows * cols; ++i)
    {
      out << ' ' << m[i];
    }
    out << '\n';
  }

  int process(std::istream & in, const char * out_name, Elem * data, std::size_t rows, std::size_t cols)
  {
    if (!readElements(in, data, rows * cols))
    {
      std::cerr << "Incorrect matrix" << '\n';
      return ERROR_BAD_DATA;
    }
    transform(data, rows, cols);

    std::ofstream out(out_name);
    if (!out)
    {
      std::cerr << "Cannot open output file" << '\n';
      return ERROR_BAD_DATA;
    }
    print(out, data, rows, cols);
    return 0;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: ./lab num input output" << '\n';
    return ERROR_BAD_ARGS;
  }

  const int mode{himich::parseMode(argv[1])};
  if (mode == 0)
  {
    std::cerr << "First parameter must be 1 or 2" << '\n';
    return ERROR_BAD_ARGS;
  }

  std::ifstream in(argv[2]);
  if (!in)
  {
    std::cerr << "Cannot open input file" << '\n';
    return ERROR_BAD_DATA;
  }

  int r{0};
  int c{0};
  if (!(in >> r >> c) || (r < 0) || (c < 0))
  {
    std::cerr << "Incorrect matrix" << '\n';
    return ERROR_BAD_DATA;
  }
  const std::size_t rows{static_cast< std::size_t >(r)};
  const std::size_t cols{static_cast< std::size_t >(c)};
  const std::size_t total{rows * cols};

  if (mode == 1)
  {
    if (total > FIXED_CAPACITY)
    {
      std::cerr << "Matrix is too large" << '\n';
      return ERROR_BAD_DATA;
    }
    himich::Elem data[FIXED_CAPACITY] = {};
    return himich::process(in, argv[3], data, rows, cols);
  }

  himich::Elem * const data{new (std::nothrow) himich::Elem[total]};
  if (data == nullptr)
  {
    std::cerr << "Not enough memory" << '\n';
    return ERROR_BAD_DATA;
  }
  const int code{himich::process(in, argv[3], data, rows, cols)};
  delete[] data;
  return code;
}
