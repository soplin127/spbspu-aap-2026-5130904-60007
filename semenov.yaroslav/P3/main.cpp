#include <iostream>
#include <cstring>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "wrong number of arguments\n";
    return 1;
  }
  std::cout << "Mode: " << argv[1] << '\n';
  std::cout << "Input: " << argv[2] << '\n';
  std::cout << "Output: " << argv[3] << '\n';

  if (std::strcmp(argv[1], "1") != 0 && std::strcmp(argv[1], "2") != 0)
  {
    std::cerr << "Invalid mode\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  int rows = 0;
  int cols = 0;
  input >> rows >> cols;
  if (input.fail())
  {
    std::cerr << "Invalid input file format\n";
    return 2;
  }
  std::cout << rows << " " << cols << '\n';

  const int capacity = 10000;
  int fixed_arr[capacity];
  int* arr = fixed_arr;
  if (std::strcmp(argv[1], "2") == 0)
  {
    arr = new int[rows * cols];
  }
  int top = 0;
  int bottom = rows - 1;
  int left = 0;
  int right = cols - 1;
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      input >> arr[i * cols + j];
      if (input.fail())
      {
        std::cerr << "Invalid input file format\n";
        if (std::strcmp(argv[1], "2") == 0)
        {
          delete[] arr;
        }
        return 2;
      }
    }
  }
  int subtrahend = 1;
  for (; (bottom >= top) && (right >= left);)
  {
    for (int i = bottom; i >= top; --i)
    {
      arr[i * cols + left] -= subtrahend;
      ++subtrahend;
    }
    ++left;
    for (int i = left; (i <= right) && (top <= bottom); ++i)
    {
      arr[top * cols + i] -= subtrahend;
      ++subtrahend;
    }
    ++top;
    for (int i = top; (i <= bottom) && (left <= right); ++i)
    {
      arr[i * cols + right] -= subtrahend;
      ++subtrahend;
    }
    --right;
    for (int i = right; (i >= left) && (top <= bottom); --i)
    {
      arr[bottom * cols + i] -= subtrahend;
      ++subtrahend;
    }
    --bottom;
  }
  std::ofstream output(argv[3]);
  if (!output)
  {
    std::cerr << "Failed to open output file\n";
    if (std::strcmp(argv[1], "2") == 0)
    {
      delete[] arr;
    }
    return 2;
  }

  output << rows << ' ' << cols;
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      output << ' ' << arr[i * cols + j];
    }
  }
  if (std::strcmp(argv[1], "2") == 0)
  {
    delete[] arr;
  }
  return 0;
}
