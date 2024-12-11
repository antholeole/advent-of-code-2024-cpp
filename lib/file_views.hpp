#include <concepts>
#include <string>

namespace {
template <typename T>
concept TIsReader = requires(std::istream &s, T &self) {
  typename T::TRet;
  { T::read(s) } -> std::same_as<typename T::TRet>;
};

template <TIsReader TReader> struct file_istream : private TReader {
  using Self = file_istream<TReader>;

  friend std::istream &operator>>(std::istream &s, Self &fv) {
    fv.v = TReader::read(s);
    return s;
  };

  TReader::TRet &get() { return v; }

private:
  TReader::TRet v;
};

struct line_file_reader {
  using TRet = std::string;

  static TRet read(std::istream &s) {
    std::string str;
    std::getline(s, str);
    return str;
  };
};
static_assert(TIsReader<line_file_reader>);

struct char_file_reader {
  using TRet = char;

  static TRet read(std::istream &s) { return s.get(); };
};
static_assert(TIsReader<char_file_reader>);

}; // namespace

using lsv_file_view = file_istream<line_file_reader>;
using char_file_view = file_istream<char_file_reader>;
