#include <cctype>
#include <new>
#include <iostream>
#include <cstddef>
#include <string>

namespace shibaev
{
  const std::size_t alphabet_size = 26;
  const int error_code = 1;

  std::size_t distinctLetters(const char *str);
}

int main()
{
  std::string *source = nullptr;

  try {
    source = new std::string;
    if (!(std::getline(std::cin, *source))) {
      std::cerr << "Invalid input\n";
      delete source;
      return shibaev::error_code;
    }
  } catch (const std::bad_alloc &) {
    std::cerr << "Memory allocation failed\n";
    delete source;
    return shibaev::error_code;
  }

  const std::size_t count = shibaev::distinctLetters (source->c_str());

  std::cout << count << '\n';
  std::cout.flush();

  if (!std::cout) {
    std::cerr << "Output error\n";
    delete source;
    return shibaev::error_code;
  }

  delete source;
  return 0;
}

std::size_t shibaev::distinctLetters(const char *str)
{
  bool letters[shibaev::alphabet_size]{};
  std::size_t i = 0;

  while (str[i] != '\0') {
    const char c = static_cast<char>(std::tolower(static_cast<unsigned char>(str[i])));
    if ((c >= 'a') && (c <= 'z')) {
      letters[c - 'a'] = true;
    }
    i++;
  }

  std::size_t count = 0;
  for (i = 0; i < shibaev::alphabet_size; i++) {
    if (letters[i]) {
      count++;
    }
  }

  return count;
}