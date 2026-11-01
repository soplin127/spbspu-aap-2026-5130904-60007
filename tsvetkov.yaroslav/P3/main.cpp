#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << "\n";
    return 2;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << "\n";
    return 2;
  }
  char* end = nullptr;
  const long value = std::strtol(argv[1], &end, 10);
  if (end == argv[1] || *end != '\0')
  {
    std::cerr << "First parameter is not a number\n";
    return 2;
  }
  if (std::string(argv[1]) != "1" && std::string(argv[1]) != "2")
  {
    std::cerr << "First parameter is out of range\n";
    return 2;
  }
  std::ifstream file(argv[2]);
  int rows = 0;
  int cols = 0;
  if (!(file >> rows >> cols) || rows < 0 || cols < 0)
  {
    std::cerr << "Incorrect matrix dimensions\n";
    return 2;
  }
  int x = 0;
  int count = 0;
  while (file >> x)
  {
    count++;
  }
  if (count < rows * cols)
  {
    std::cerr << "The matrix does not exist\n";
    return 2;
  }
  file.clear();
  file.seekg(0, std::ios::beg);
  if (value == 1)
  {
    const int mr = 100;
    const int mc = 100;
    int sm[mr][mc];
    std::ifstream file(argv[2]);
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        file >> sm[i][j];
      }
    }
    file.close();
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        const int layer = std::min({i, rows - 1 - i, j, cols - 1 - j}) + 1;
        sm[i][j] += layer;
      }
    }
    std::ofstream out(argv[3]);
    out << rows << " " << cols << " ";
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        out << sm[i][j] << " ";
      }
    }
  }
  else if (value == 2)
  {
    std::ifstream file(argv[2]);
    int** const sm = new int*[rows];
    for (int i = 0; i < rows; ++i)
    {
      sm[i] = new int[cols];
    }
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        file >> sm[i][j];
      }
    }
    file.close();
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        const int layer = std::min({i, rows - 1 - i, j, cols - 1 - j}) + 1;
        sm[i][j] += layer;
      }
    }
    std::ofstream out(argv[3]);
    out << rows << " " << cols << " ";
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
      {
        out << sm[i][j] << " ";
      }
    }
    for (int i = 0; i < rows; ++i)
    {
      delete[] sm[i];
    }
    delete[] sm;
  }
  return 0;
}
