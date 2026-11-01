#include <fstream>
#include <iostream>

const int MAX_STATIC_SIZE = 10000;
const int MAX_DYNAMIC_SIZE = 1000000;

namespace ivanova {
  bool readMatrix(const char *filename, int &rows, int &cols, int *data, int max_size)
  {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
      return false;
    }
    fin >> rows >> cols;
    if (!fin) {
      return false;
    }
    if (rows < 0 || cols < 0) {
      return false;
    }
    if (rows * cols > max_size) {
      return false;
    }
    const int total = rows * cols;
    for (int i = 0; i < total; i++) {
      fin >> data[i];
      if (fin.fail()) {
        return false;
      }
      if (fin.eof() && i < total - 1) {
        return false;
      }
    }

    return true;
  }

  int findColumn(const int *data, int rows, int cols)
  {
    if (rows <= 0 || cols <= 0) {
      return 0;
    }
    int best_col = 1;
    int best_len = 0;

    for (int j = 0; j < cols; j++) {
      int cur_len = 1;
      int maxln_col = 1;
      for (int i = 1; i < rows; i++) {
        const int cur = data[i * cols + j];
        const int prev = data[(i - 1) * cols + j];
        if (cur == prev) {
          cur_len++;
          if (cur_len > maxln_col) {
            maxln_col = cur_len;
          }
        } else {
          cur_len = 1;
        }
      }
      if (maxln_col > best_len) {
        best_len = maxln_col;
        best_col = j + 1;
      }
    }
    return best_col;
  }
}

int main(int argc, char *argv[])
{
  using namespace ivanova;

  if (argc != 4) {
    std::cerr << "Error: need 3 arguments\n";
    return 1;
  }

  if (argv[1][0] != '1' && argv[1][0] != '2') {
    std::cerr << "Error: first argument must be 1 or 2\n";
    return 1;
  }
  if (argv[1][1] != '\0') {
    std::cerr << "Error: first argument must be 1 or 2\n";
    return 1;
  }
  const int mode = argv[1][0] - '0';

  const char *const input_file = argv[2];
  const char *const output_file = argv[3];

  int rows = 0;
  int cols = 0;

  if (mode == 1) {
    int data[MAX_STATIC_SIZE];
    if (!readMatrix(input_file, rows, cols, data, MAX_STATIC_SIZE)) {
      std::cerr << "Error: cannot read matrix\n";
      return 2;
    }
    const int result = findColumn(data, rows, cols);

    std::ofstream fout(output_file);
    if (!fout.is_open()) {
      std::cerr << "Error: cannot open output file\n";
      return 2;
    }
    fout << rows << " " << cols << " " << result << "\n";
  } else {
    static int temp[MAX_DYNAMIC_SIZE];

    if (!readMatrix(input_file, rows, cols, temp, MAX_DYNAMIC_SIZE)) {
      std::cerr << "Error: cannot read matrix\n";
      return 2;
    }

    const int total = rows * cols;
    int *data = nullptr;
    if (total > 0) {
      data = new int[total];
    }

    for (int i = 0; i < total; i++) {
      data[i] = temp[i];
    }

    const int result = findColumn(data, rows, cols);

    std::ofstream fout(output_file);
    if (!fout.is_open()) {
      std::cerr << "Error: cannot open output file\n";
      if (data != nullptr) {
        delete[] data;
      }
      return 2;
    }
    fout << rows << " " << cols << " " << result << "\n";

    if (data != nullptr) {
      delete[] data;
    }
  }

  return 0;
}
