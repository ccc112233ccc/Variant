#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

template <typename T, typename... Ts>
struct MaxSize {
  constexpr static size_t size = sizeof(T) > MaxSize<Ts...>::size
                                     ? sizeof(T)
                                     : MaxSize<Ts...>::size;
};

template <typename T>
struct MaxSize<T> {
  constexpr static size_t size = sizeof(T);
};

template <int idx, typename T, typename... Ts>
struct Type_element {
  using Type = typename Type_element<idx - 1, Ts...>::Type;
};
template <typename T, typename... Ts>
struct Type_element<0, T, Ts...> {
  using Type = T;
};

template <typename... Ts>
struct Variant {
  char data[MaxSize<Ts...>::size];
  template <typename T>
  void operator=(T&& t) {
    using rm_ref = typename remove_reference<T>::type;
    memset(data, 0, sizeof(data));
    new (data) rm_ref(forward<T>(t));
  }
  void operator=(const char* s) {
    memset(data, 0, sizeof(data));
    new (data) string(s);
  }

  template <int idx>
  auto& get() {
    return *reinterpret_cast<Type_element<idx, Ts...>::Type*>(data);
  }
};

template <typename T>
void destroy(char* data) {
  reinterpret_cast<T*>(data)->~T();
}

int main() {
  Variant<int, double, string> v;
  v = 10;
  cout << v.get<0>() << endl;
  v = 3.14;
  cout << v.get<1>() << endl;
  v = "hello";
  cout << v.get<2>() << endl;
  return 0;
}