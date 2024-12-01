#include <string>

struct lsv_file_view {
  std::string line;

  friend std::istream &operator>>(std::istream &s, lsv_file_view &fv) {
    std::getline(s, fv.line);
    ++fv.line_n;
    return s;
  };

private:
  size_t line_n;
};
