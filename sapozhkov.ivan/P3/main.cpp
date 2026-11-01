#include <iostream>
#include <fstream>

namespace sapozhkov
{
  int findLongestSeries(const int* matrix, int rows, int cols)
  {
    int current_series = 1;
    int max_best_in_row = 1;
    int best_of_best = 0;
    int rows_best = 0;
    if (rows == 0)
    {
      return rows_best;
    }
    else
    {
      for (int i = 0; i < rows; ++i)
      {
        current_series = 1;
        max_best_in_row = 1;
        for (int j = 0; j < (cols - 1); ++j)
        {
          if (matrix[i * cols + j] == matrix[i * cols + j + 1])
          {
            ++current_series;
            if (max_best_in_row < current_series)
            {
              max_best_in_row = current_series;
            }
          }
          else
          {
            current_series = 1;
          }
        }
        if (max_best_in_row > best_of_best)
        {
          best_of_best = max_best_in_row;
          rows_best = i;
        }
      }
      return rows_best + 1;
    }
  }
}

int main(int argc, char** argv)
{
  const int argc_count = 4;
  int num = 0;
  const int max_elemets = 10000;
  if (argc < argc_count)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > argc_count)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }
  if (argv[1][0] == '1' && argv[1][1] == '\0')
  {
    num = 1;
  }
  else if (argv[1][0] == '2' && argv[1][1] == '\0')
  {
    num = 2;
  }
  else if (argv[1][1] != '\0')
  {
    std::cerr << "First parameter is not a number" << '\n';
    return 1;
  }
  else
  {
    std::cerr << "First parameter is out of range" << '\n';
    return 1;
  }
  std::ifstream input(argv[2]);
  int rows = 0;
  int cols = 0;
  input >> rows >> cols;
  if (!input)
  {
    std::cerr << "Can't read matrix" << '\n';
    return 2;
  }
  if (rows < 0 || cols < 0)
  {
    std::cerr << "Rows and Cols must be non-negative" << '\n';
    return 2;
  }
  if ((rows == 0 && cols > 0) || (rows > 0 && cols == 0))
  {
    std::cerr << "Incorrect input" << '\n';
    return 2;
  }
  int fixed[max_elemets];
  int* matrix = nullptr;
  if (num == 1)
  {
    if ((rows * cols) > max_elemets)
    {
      std::cerr << "Too many entries" << '\n';
      return 2;
    }
    matrix = fixed;
  }
  else
  {
    matrix = new int[rows * cols];
  }
  for (int i = 0; i < rows * cols; ++i)
  {
    input >> matrix[i];
    if (!input)
    {
      std::cerr << "Incorrect input" << '\n';
      if (num == 2)
      {
        delete[] matrix;
      }
      return 2;
    }
  }
  std::ofstream output(argv[3]);
  output << sapozhkov::findLongestSeries(matrix, rows, cols) << '\n';
  if (num == 2)
  {
    delete[] matrix;
  }
  return 0;
}
