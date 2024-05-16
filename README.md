# Variant

## Description

一个仿std::variant的C++17实现。variant是一个能存储多种类型的值的类型，它的行为类似于union，但是它能够安全地处理构造和析构。

## Usage

```cpp
#include "Variant.h"

using namespace std;
using ccutil::Variant;

int main() {
    Variant<int, double, string> v;
    v = 1;
    cout << get<int>(v) << endl;
    v = 3.14;
    cout << get<double>(v) << endl;
    v = "hello";
    cout << get<string>(v) << endl;
    return 0;
}
```