# Практическое занятие №4

Проект: **Тайм-трекер** - консольное C++ приложение для учета рабочего времени по задачам.

## Структура

- `include/` - публичные заголовочные файлы модулей.
- `src/` - исходные файлы реализации.
- `tests/` - unit-тесты Catch2 и отдельные тестовые сценарии.
- `CMakeLists.txt` - конфигурация сборки CMake.
- `report.html` - отчет по практическому занятию №4.
- `report.pdf` - PDF-версия отчета.
- `TEST_SCENARIOS.md` - список тестовых сценариев.

## Модули

- `Task`, `TimeSession` - доменные модели.
- `TaskManager` - создание, поиск и архивирование задач.
- `TimeTracker` - запуск и остановка рабочих сессий.
- `Storage` - сохранение и загрузка данных из локальных CSV-файлов.
- `ReportService` - расчет суммарного времени по задачам.
- `ConsoleUI` - консольное меню пользователя.

## Сборка

```sh
cmake -S . -B build
cmake --build build
```

При первой конфигурации CMake загрузит Catch2 через `FetchContent`.

## Запуск

```sh
cd build
./time_tracker
```

На Windows исполняемый файл может называться `time_tracker.exe`.

## Запуск тестов

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Unit-тесты собираются в `build/tests/unit_tests`.

Отдельные сценарные мини-программы собираются в `build/tests/scenarios`:

```sh
./tests/scenarios/scenario_basic_workflow
./tests/scenarios/scenario_persistence
./tests/scenarios/scenario_reporting
```

## Сторонние библиотеки

Для unit-тестирования используется Catch2. Основное приложение использует только стандартную библиотеку C++17.
