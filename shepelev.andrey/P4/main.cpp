#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <new>

namespace shepelev
{
  constexpr std::size_t initial_capacity = 16;
  constexpr std::size_t growth_factor = 2;
  constexpr int memory_error_code = 1;
  constexpr int input_error_code = 2;

  char* convertToLower(char* dest, const char* src) noexcept;
  char* readLine();
}

int main()
{
  char* const source = shepelev::readLine();

  if (source == nullptr)
  {
    std::cerr << "Error: cannot allocate memory for input string\n";
    return shepelev::memory_error_code;
  }

  if (source[0] == '\0' && std::cin.eof())
  {
    delete[] source;
    std::cerr << "Error: no input string\n";
    return shepelev::input_error_code;
  }

  char* const result = new (std::nothrow) char[std::strlen(source) + 1];

  if (result == nullptr)
  {
    delete[] source;
    std::cerr << "Error: cannot allocate memory for result string\n";
    return shepelev::memory_error_code;
  }

  shepelev::convertToLower(result, source);

  std::cout << result << '\n';

  delete[] result;
  delete[] source;

  return 0;
}

char* shepelev::convertToLower(char* dest, const char* src) noexcept
{
  if (dest == nullptr || src == nullptr)
  {
    return nullptr;
  }

  std::size_t i = 0;

  while (src[i] != '\0')
  {
    const unsigned char ch = static_cast< unsigned char >(src[i]);

    if (std::isupper(ch))
    {
      dest[i] = static_cast< char >(std::tolower(ch));
    }
    else
    {
      dest[i] = src[i];
    }

    i++;
  }

  dest[i] = '\0';
  return dest;
}

char* shepelev::readLine()
{
  std::size_t capacity = initial_capacity;
  std::size_t size = 0;
  char* buffer = new (std::nothrow) char[capacity];

  if (buffer == nullptr)
  {
    return nullptr;
  }

  char ch = '\0';

  while (std::cin.get(ch) && ch != '\n')
  {
    if (size + 1 >= capacity)
    {
      const std::size_t new_capacity = capacity * growth_factor;
      char* const bigger = new (std::nothrow) char[new_capacity];

      if (bigger == nullptr)
      {
        delete[] buffer;
        return nullptr;
      }

      for (std::size_t i = 0; i < size; i++)
      {
        bigger[i] = buffer[i];
      }

      delete[] buffer;
      buffer = bigger;
      capacity = new_capacity;
    }

    buffer[size] = ch;
    size++;
  }

  buffer[size] = '\0';
  return buffer;
}
