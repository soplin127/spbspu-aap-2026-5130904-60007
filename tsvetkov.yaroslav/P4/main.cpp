#include <iostream>
#include <cstddef>
#include <new>
#include <cctype>

namespace tsvetkov {
  std::size_t capacity = 16;
  constexpr std::size_t coef = 2;
  constexpr int memerr = 1;
  constexpr int argerr = 2;
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
  std::size_t size = 0;
  char* buf = new char[capacity];

  char c = 0;
  while (std::cin.get(c) && c != '\n') {
    if (size + 1 == capacity) {
      char* newBuf = nullptr;
      try {
        newBuf = new char[capacity * coef];
      } catch (const std::bad_alloc&) {
        delete[] buf;
        throw;
      }
      for (std::size_t i = 0; i < size; ++i) {
        newBuf[i] = buf[i];
      }
      delete[] buf;
      buf = newBuf;
      capacity *= coef;
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
