# Практическое занятие №5

Проект: **Тайм-трекер** - консольное C++ приложение для учета рабочего времени по задачам.

Тема работы: **контейнеризация проекта с Docker**.

## Что добавлено

- `Dockerfile` - многостадийная сборка проекта в Linux-окружении.
- `.dockerignore` - исключение временных файлов, локальных сборок и данных из Docker-контекста.
- CLI-режим в `src/main.cpp` - запуск основной функции приложения через аргументы командной строки.
- Запуск unit-тестов Catch2 внутри Docker на этапе сборки образа.
- Финальный runtime-образ содержит только исполняемый файл и каталог `data`.

## Сборка локально

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Сборка Docker-образа

Команду нужно выполнять из каталога `KR5`:

```sh
docker build -t time-tracker-kr5 .
```

Во время сборки Docker:

1. устанавливает CMake, компилятор C++ и Git;
2. собирает приложение;
3. загружает Catch2 через CMake `FetchContent`;
4. запускает `ctest`;
5. копирует только готовый бинарный файл в чистый runtime-образ.

## Запуск контейнера

Демонстрационный запуск:

```sh
docker run --rm time-tracker-kr5
```

Передача аргументов командной строки:

```sh
docker run --rm time-tracker-kr5 --help
docker run --rm time-tracker-kr5 add-task "Write report" "Prepare Docker section"
docker run --rm time-tracker-kr5 list-tasks
docker run --rm time-tracker-kr5 report
```

Для сохранения данных между запусками можно подключить volume:

```sh
docker volume create time-tracker-data
docker run --rm -v time-tracker-data:/app/data time-tracker-kr5 add-task "Docker demo"
docker run --rm -v time-tracker-data:/app/data time-tracker-kr5 list-tasks
```

## Команды приложения

```text
time_tracker                         interactive menu
time_tracker --demo                  run container demo
time_tracker [--data DIR] add-task TITLE [DESCRIPTION]
time_tracker [--data DIR] list-tasks
time_tracker [--data DIR] archive-task ID
time_tracker [--data DIR] start ID
time_tracker [--data DIR] stop
time_tracker [--data DIR] history
time_tracker [--data DIR] report
```

Без аргументов приложение запускает старое интерактивное меню. В контейнере по умолчанию выполняется команда `--demo`, чтобы контейнер сразу показывал работу проекта.
