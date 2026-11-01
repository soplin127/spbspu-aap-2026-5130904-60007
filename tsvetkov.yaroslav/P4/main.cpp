#include <iostream>
#include <cstddef>
#include <new>
#include <cctype>

namespace tsvetkov {
  constexpr std::size_t capacity = 16;
  constexpr std::size_t coef = 2;
  constexpr int memerr = 1;
  constexpr int argerr = 2;
  constexpr int inputerr = 3;
  char* readline(std::size_t& length);
  char* removeLatin(const char* src, char* dst) noexcept;
}

int main()
{
  std::size_t len = 0;
  char* input = nullptr;
  try {
    input = tsvetkov::readline(len);
  } catch (const std::bad_alloc&) {
    std::cerr << "Not enough memory\n";
    return tsvetkov::memerr;
  }
  if (len == 0 && std::cin.eof()) {
    delete[] input;
    std::cerr << "No input\n";
    return tsvetkov::inputerr;
  }

  char* result = nullptr;
  try {
    result = new char[len + 1];
  } catch (const std::bad_alloc&) {
    delete[] input;
    std::cerr << "Not enough memory\n";
    return tsvetkov::memerr;
  }

  if (tsvetkov::removeLatin(input, result) == nullptr) {
    delete[] result;
    delete[] input;
    std::cerr << "Invalid argument\n";
    return tsvetkov::argerr;
  }
  std::cout << result << "\n";

  delete[] result;
  delete[] input;
  return 0;
}

char* tsvetkov::readline(std::size_t& length)
{
  std::size_t buf_capacity = capacity;
  std::size_t size = 0;
  char* buf = new char[buf_capacity];

  char c = 0;
  while (std::cin.get(c) && c != '\n') {
    if (size + 1 == buf_capacity) {
      char* new_buf = nullptr;
      try {
        new_buf = new char[buf_capacity * coef];
      } catch (const std::bad_alloc&) {
        delete[] buf;
        throw;
      }
      for (std::size_t i = 0; i < size; ++i) {
        new_buf[i] = buf[i];
      }
      delete[] buf;
      buf = new_buf;
      buf_capacity *= coef;
    }
    buf[size] = c;
    ++size;
  }
  buf[size] = '\0';
  length = size;
  return buf;
}

char* tsvetkov::removeLatin(const char* src, char* dst) noexcept
{
  if (src == nullptr || dst == nullptr) {
    return nullptr;
  }
  std::size_t j = 0;
  for (std::size_t i = 0; src[i] != '\0'; ++i) {
    if (!std::isalpha(static_cast< unsigned char >(src[i]))) {
      dst[j] = src[i];
      ++j;
    }
  }
  dst[j] = '\0';
  return dst;
}
