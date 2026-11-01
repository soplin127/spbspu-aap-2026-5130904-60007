#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace chinikov
{
  constexpr size_t max_static_size = 10000;
  int cntSedElements(const size_t& rows, const size_t& columns, const int* matrix)
  {
    int counter = 0;
    for (size_t row = 0; row < rows; row++)
    {
      for (size_t column = 0; column < columns; column++)
      {
        bool min_in_row = true;
        for (size_t row_min = 0; row_min < columns; row_min++)
        {
          if (matrix[row * columns + row_min] < matrix[row * columns + column])
          {
            min_in_row = false;
            break;
          }
        }
        if (!min_in_row)
        {
          continue;
        }
        bool max_in_column = true;
        for (size_t column_max = 0; column_max < rows; column_max++)
        {
          if (matrix[column_max * columns + column] > matrix[row * columns + column])
          {
            max_in_column = false;
            break;
          }
        }
        if (max_in_column)
        {
          counter += 1;
        }
      }
    }
    return counter;
  }
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }
  try
  {
    if (std::stoi(argv[1]) < 1 || std::stoi(argv[1]) > 2)
    {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }
  }
  catch (const std::invalid_argument&)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  catch (const std::out_of_range&)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  int rows_num = 0;
  int columns_num = 0;
  const int arg1 = std::stoi(argv[1]);
  int static_matrix[chinikov::max_static_size];
  int* matrix = nullptr;
  std::ifstream file(argv[2]);
  if (!(file.is_open()))
  {
    std::cerr << "Can not open file" << '\n';
    return 2;
  }
  if (!(file >> rows_num >> columns_num))
  {
    std::cerr << "Incorrect data" << '\n';
    return 2;
  }
  if (rows_num < 0 || columns_num < 0)
  {
    std::cerr << "Negative size" << '\n';
    return 2;
  }
  if (arg1 == 1 && static_cast< size_t >(rows_num * columns_num) > chinikov::max_static_size)
  {
    std::cerr << "Stack overflow" << '\n';
    return 2;
  }

  if (arg1 == 1)
  {
    matrix = static_matrix;
  }
  else
  {
    matrix = new int[rows_num * columns_num];
  }

  for (int i = 0; i < rows_num * columns_num; i++)
  {
    if (!(file >> matrix[i]))
    {
      if (arg1 == 2)
      {
        delete[] matrix;
      }
      std::cerr << "Incorrect data in matrix" << '\n';
      return 2;
    }
  }
  std::ofstream out(argv[3]);
  if (!(out.is_open()))
  {
    if (arg1 == 2)
    {
      delete[] matrix;
    }
    std::cerr << "Can not open output" << '\n';
    return 1;
  }
  const int count = chinikov::cntSedElements(rows_num, columns_num, matrix);
  out << count << '\n';

  if (arg1 == 2)
  {
    delete[] matrix;
  }
  return 0;
}
