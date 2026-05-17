# KR1

Минимальный C++ проект для практического задания.

## Структура

- `hello.h` - объявление функции `printHelloWorld(std::ostream&)`.
- `hello.cpp` - определение функции.
- `main.cpp` - точка входа программы.
- `Makefile` - сборка проекта через `make`.

## Сборка и запуск

```sh
make
make run
```

При ручной сборке через GCC/MinGW:

```sh
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp hello.cpp -o hello
./hello
```

Ожидаемый вывод:

```text
Hello, World!
```
