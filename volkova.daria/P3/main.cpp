#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

namespace volkova
{
  const double tenths_degree = 10.0;
  void printTenths(std::ostream& out, double v)
  {
    long long t = std::llround(v * tenths_degree);
    if (t < 0)
    {
      out << '-';
      t = -t;
    }
    out << t / 10;
    if (t % 10 != 0)
    {
      out << '.' << t % 10;
    }
  }
}

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Wrong number of arguments\n";
    return 1;
  }

  int num = 0;
  if (std::strcmp(argv[1], "1") == 0)
  {
    num = 1;
  }
  else if (std::strcmp(argv[1], "2") == 0)
  {
    num = 2;
  }
  else
  {
    std::cerr << "First parameter must be 1 or 2\n";
    return 1;
  }

  std::ifstream in(argv[2]);
  int rows = 0;
  int cols = 0;
  if (!(in >> rows >> cols) || rows < 0 || cols < 0)
  {
    std::cerr << "Input is not a matrix\n";
    return 2;
  }
  const long long total = static_cast< long long >(rows) * cols;

  const int stack_capacity = 10000;
  int stack_arr[stack_capacity];
  int* dyn_arr = nullptr;
  int* m = stack_arr;
  if (num == 2)
  {
    dyn_arr = new int[total];
    m = dyn_arr;
  }
  else if (total > stack_capacity)
  {
    std::cerr << "Matrix is too large\n";
    return 2;
  }

  for (long long i = 0; i < total; ++i)
  {
    if (!(in >> m[i]))
    {
      std::cerr << "Input is not a matrix\n";
      delete[] dyn_arr;
      return 2;
    }
  }
  char extra = 0;
  if (in >> extra)
  {
    std::cerr << "Input is not a matrix\n";
    delete[] dyn_arr;
    return 2;
  }

  std::ofstream out(argv[3]);
  out << rows << ' ' << cols;
  for (int r = 0; r < rows; ++r)
  {
    for (int c = 0; c < cols; ++c)
    {
      long long sum = 0;
      int cnt = 0;
      for (int dr = -1; dr <= 1; ++dr)
      {
        for (int dc = -1; dc <= 1; ++dc)
        {
          const int nr = r + dr;
          const int nc = c + dc;
          const bool is_self = (dr == 0 && dc == 0);
          const bool inside = nr >= 0 && nr < rows && nc >= 0 && nc < cols;
          if (!is_self && inside)
          {
            sum += m[nr * cols + nc];
            ++cnt;
          }
        }
      }
      const double value = (cnt == 0) ? m[r * cols + c] : static_cast< double >(sum) / cnt;
      out << ' ';
      volkova::printTenths(out, value);
    }
  }
  out << '\n';

  delete[] dyn_arr;
  return 0;
}
