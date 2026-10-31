#include <cstddef>
#include <cstring>
#include <iostream>
#include <new>

namespace sopin {
  int hasRep(const char* str)
  {
    for (; *str != '\0'; ++str) {
      if (std::strchr(str + 1, *str) != nullptr) {
        return 1;
      }
    }
    return 0;
  }

  char* getLine(std::istream& in)
  {
    std::size_t capacity = 16;
    std::size_t size = 0;
    char* str = new char[capacity];
    char c = '\0';
    while (in.get(c) && c != '\n') {
      if (size + 1 == capacity) {
        char* tmp = nullptr;
        try {
          tmp = new char[capacity * 2];
        } catch (const std::bad_alloc&) {
          delete[] str;
          throw;
        }
        std::memcpy(tmp, str, size);
        delete[] str;
        str = tmp;
        capacity *= 2;
      }
      str[size++] = c;
    }
    str[size] = '\0';
    return str;
  }
}

int main()
{
  char* str = nullptr;
  try {
    str = sopin::getLine(std::cin);
  } catch (const std::bad_alloc&) {
    std::cerr << "Not enough memory\n";
    return 1;
  }
  if (str[0] == '\0' && std::cin.eof()) {
    delete[] str;
    std::cerr << "No input\n";
    return 1;
  }
  std::cout << sopin::hasRep(str) << "\n";
  delete[] str;
  return 0;
}
