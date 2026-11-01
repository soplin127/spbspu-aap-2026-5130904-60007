#include <cstddef>
#include <iostream>
#include <limits>

namespace yilman {
  class IncSeq {
  public:
    void operator()(int a);
    size_t operator()() const;
    bool overflow() const;

  private:
    int prev_ = 0;
    bool has_prev_ = false;
    bool overflow_ = false;
    size_t count_ = 0;
  };
}

void yilman::IncSeq::operator()(int a)
{
  if (has_prev_ && a > prev_) {
    if (count_ == std::numeric_limits< size_t >::max()) {
      overflow_ = true;
    } else {
      ++count_;
    }
  }
  prev_ = a;
  has_prev_ = true;
}

size_t yilman::IncSeq::operator()() const
{
  return count_;
}

bool yilman::IncSeq::overflow() const
{
  return overflow_;
}

int main()
{
  yilman::IncSeq inc_seq;
  int a = 0;
  std::cin >> a;
  while (std::cin && a != 0) {
    inc_seq(a);
    std::cin >> a;
  }
  if (!std::cin) {
    std::cerr << "Error: input is not a sequence\n";
    return 1;
  }
  if (inc_seq.overflow()) {
    std::cerr << "Error: INC-SEQ overflow\n";
    return 2;
  }
  std::cout << inc_seq() << "\n";
  return 0;
}
