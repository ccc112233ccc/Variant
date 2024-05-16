#include <cstring>

namespace ccutil {

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

template <int id, typename U, typename T, typename... Ts>
struct Position {
  constexpr static int pos =
      std::is_same<U, T>::value ? id : Position<id + 1, U, Ts...>::pos;
};
template <int id, typename U, typename T>
struct Position<id, U, T> {
  constexpr static int pos = id;
};

template <typename T>
void destroy(char* data) {
  reinterpret_cast<T*>(data)->~T();
}

template <typename... Ts>
struct Variant {
  int type = -1;
  char data[MaxSize<Ts...>::size];
  using destroy_func_t = void (*)(char*);
  constexpr static destroy_func_t destroy_funcs[] = {destroy<Ts>...};

  Variant() {}
  Variant(const Variant& other) {
    type = other.type;
    memcpy(data, other.data, sizeof(data));
  }

  void operator=(const Variant& other) {
    type = other.type;
    memcpy(data, other.data, sizeof(data));
  }

  template <typename T>
  void operator=(T&& t) {
    if (type != -1) {
      destroy_funcs[type](data);
    }
    type = Position<0, T, Ts...>::pos;
    using rm_ref = typename std::remove_reference<T>::type;
    memset(data, 0, sizeof(data));
    new (data) rm_ref(std::forward<T>(t));
  }
  template <typename T>
  T& get() {
    return *reinterpret_cast<T*>(data);
  }

  template <int idx>
  auto& get() {
    return *reinterpret_cast<typename Type_element<idx, Ts...>::Type*>(data);
  }

  ~Variant() {
    if (type != -1) {
      destroy_funcs[type](data);
    }
  }
};

}  // namespace ccutil