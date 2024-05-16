#include <iostream>
#include <string>

#include "Variant.h"

struct my_struct {
  int a;
  double b;
  std::string c;
};

int main() {
  using namespace std;
  using ccutil::Variant;
  Variant<int, double, my_struct> v;
  v = 10;
  cout << v.get<int>() << endl;
  v = 3.14;
  cout << v.get<double>() << endl;
  v = my_struct{1, 3.14, "hello"};
  cout << v.get<my_struct>().a << " " << v.get<my_struct>().b << " "
       << v.get<my_struct>().c << endl;

  return 0;
}