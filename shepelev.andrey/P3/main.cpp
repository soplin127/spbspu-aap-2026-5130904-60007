#include <iostream>
#include <fstream>
#include <string>

constexpr int max_elements = 10000;

void processMatrix(int* a, const int rows, const int cols)
{
  const int count = rows * cols;
  int step = 1;
  int top = 0;
  int bottom = rows - 1;
  int left = 0;
  int right = cols - 1;

  while (step <= count && top <= bottom && left <= right)
  {
    for (int i = bottom; i >= top && step <= count; i--)
    {
      a[i * cols + left] -= step;
      step++;
    }
    left++;

    for (int j = left; j <= right && step <= count && top <= bottom; j++)
    {
      a[top * cols + j] -= step;
      step++;
    }
    top++;

    for (int i = top; i <= bottom && step <= count && left <= right; i++)
    {
      a[i * cols + right] -= step;
      step++;
    }
    right--;

    for (int j = right; j >= left && step <= count && top <= bottom; j--)
    {
      a[bottom * cols + j] -= step;
      step++;
    }
    bottom--;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Error: expected 3 arguments\n";
    return 1;
  }

  const std::string mode = argv[1];

  if (mode != "1" && mode != "2")
  {
    std::cerr << "Error: num must be 1 or 2\n";
    return 1;
  }

  std::ifstream file(argv[2]);

  if (!file.is_open())
  {
    std::cerr << "Error: cannot open input file\n";
    return 2;
  }

  int rows = 0;
  int cols = 0;

  if (!(file >> rows >> cols))
  {
    std::cerr << "Error: invalid matrix dimensions\n";
    return 2;
  }

  if (rows < 0 || cols < 0)
  {
    std::cerr << "Error: invalid matrix dimensions\n";
    return 2;
  }

  if (rows > max_elements || cols > max_elements)
  {
    std::cerr << "Error: invalid matrix dimensions\n";
    return 2;
  }

  if (rows != 0 && cols > max_elements / rows)
  {
    std::cerr << "Error: invalid matrix dimensions\n";
    return 2;
  }

  std::ofstream output(argv[3]);

  if (!output.is_open())
  {
    std::cerr << "Error: cannot open output file\n";
    return 2;
  }

  if (mode == "1")
  {
    int a[max_elements];

    for (int i = 0; i < rows * cols; i++)
    {
      if (!(file >> a[i]))
      {
        std::cerr << "Error: invalid matrix element\n";
        return 2;
      }
    }

    if (rows > 0 && cols > 0)
    {
      processMatrix(a, rows, cols);
    }

    output << rows << ' ' << cols << '\n';

    for (int i = 0; i < rows * cols; i++)
    {
      output << a[i] << ' ';
    }

    output << '\n';
  }
  else
  {
    int* const a = new int[rows * cols];

    for (int i = 0; i < rows * cols; i++)
    {
      if (!(file >> a[i]))
      {
        delete[] a;
        std::cerr << "Error: invalid matrix element\n";
        return 2;
      }
    }

    if (rows > 0 && cols > 0)
    {
      processMatrix(a, rows, cols);
    }

    output << rows << ' ' << cols << '\n';

    for (int i = 0; i < rows * cols; i++)
    {
      output << a[i] << ' ';
    }

    output << '\n';

    delete[] a;
  }

  if (!output)
  {
    std::cerr << "Error: cannot write to output file\n";
    return 2;
  }

  return 0;
}
