# Объяснение контрольной работы 5

Проект: **тайм-трекер** - консольное C++ приложение для учета рабочего времени по задачам.

Главная идея пятой работы: контейнеризировать проект с помощью **Docker**. То есть сделать так, чтобы приложение собиралось, тестировалось и запускалось в чистом Linux-окружении, независимо от настроек компьютера пользователя.

## Что сказать в начале

В этой работе я подготовил Docker-образ для проекта. Внутри контейнера устанавливаются все зависимости, выполняется сборка через CMake, запускаются тесты Catch2, а в финальный образ попадает только готовое приложение.

Также в приложение добавлен режим запуска через аргументы командной строки. Это нужно, потому что контейнер удобнее запускать командой, например:

```sh
docker run --rm time-tracker-kr5 --help
docker run --rm time-tracker-kr5 add-task "Docker demo"
docker run --rm time-tracker-kr5 report
```

Без аргументов приложение по-прежнему может запускать интерактивное меню.

## Файловая структура

```text
KR5/
├── .dockerignore
├── Dockerfile
├── CMakeLists.txt
├── README.md
├── TEST_SCENARIOS.md
├── EXPLANATION.md
├── report.html
├── report.pdf
├── srs.md
├── include/
│   ├── ConsoleUI.h
│   ├── ReportService.h
│   ├── Storage.h
│   ├── Task.h
│   ├── TaskManager.h
│   ├── TimeSession.h
│   └── TimeTracker.h
├── src/
│   ├── ConsoleUI.cpp
│   ├── main.cpp
│   ├── ReportService.cpp
│   ├── Storage.cpp
│   ├── Task.cpp
│   ├── TaskManager.cpp
│   ├── TimeSession.cpp
│   └── TimeTracker.cpp
└── tests/
    ├── CMakeLists.txt
    ├── test_console_ui.cpp
    ├── test_report_service.cpp
    ├── test_storage.cpp
    ├── test_task.cpp
    ├── test_task_manager.cpp
    ├── test_time_session.cpp
    ├── test_time_tracker.cpp
    └── scenarios/
        ├── scenario_basic_workflow.cpp
        ├── scenario_persistence.cpp
        └── scenario_reporting.cpp
```

## Для чего нужны основные файлы

`Dockerfile` - главный файл пятой работы. Он описывает, как собрать Docker-образ.

`.dockerignore` - список файлов и папок, которые не нужно отправлять в Docker-контекст: локальные сборки, временные файлы, исполняемые файлы, локальные CSV-данные.

`CMakeLists.txt` - сборка C++ проекта и подключение тестов.

`tests/CMakeLists.txt` - подключение Catch2 и сборка unit-тестов.

`src/main.cpp` - точка входа. В пятой работе здесь добавлена обработка аргументов командной строки.

`README.md` - команды сборки Docker-образа и запуска контейнера.

`report.html` и `report.pdf` - отчет по работе.

## Что такое Docker в этой работе

Docker нужен, чтобы создать одинаковое окружение для сборки и запуска.

Без Docker результат зависит от компьютера: установлен ли CMake, есть ли компилятор, какая версия библиотек.

С Docker все зависимости описаны в `Dockerfile`. Поэтому проект можно собрать в чистом Linux-окружении одной командой:

```sh
docker build -t time-tracker-kr5 .
```

## Что такое многостадийная сборка

В `Dockerfile` используется две стадии:

```text
build
runtime
```

### Стадия `build`

На стадии сборки используется образ:

```dockerfile
FROM ubuntu:24.04 AS build
```

Здесь устанавливаются зависимости:

- `ca-certificates`;
- `cmake`;
- `g++`;
- `git`;
- `make`.

Они нужны для сборки проекта и загрузки Catch2.

Потом выполняются команды:

```dockerfile
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel \
    && ctest --test-dir build --output-on-failure
```

Что происходит:

1. CMake настраивает проект.
2. Проект собирается.
3. Запускаются тесты.

Если тесты не проходят, Docker-образ не соберется.

### Стадия `runtime`

На стадии запуска используется новый чистый образ:

```dockerfile
FROM ubuntu:24.04 AS runtime
```

В него копируется только готовый исполняемый файл:

```dockerfile
COPY --from=build /app/build/time_tracker /usr/local/bin/time_tracker
```

Папка сборки, исходники и временные файлы в финальный образ не попадают.

Что сказать: это и есть смысл многостадийной сборки. В первой стадии есть инструменты сборки, а во второй остается только то, что нужно для запуска.

## Зачем нужен `.dockerignore`

`.dockerignore` уменьшает Docker-контекст и не отправляет в сборку лишние файлы.

В него добавлены:

- `.git`;
- папки `build`;
- временные CMake-файлы;
- объектные файлы;
- `.exe`, `.dll`;
- локальные CSV-файлы из `data`.

Что сказать: это делает сборку чище и не добавляет в образ мусор с локального компьютера.

## Как запускается контейнер

Сборка образа:

```sh
docker build -t time-tracker-kr5 .
```

Интерактивный запуск:

```sh
docker run --rm -it time-tracker-kr5
```

Так как в Dockerfile указано:

```dockerfile
ENTRYPOINT ["time_tracker"]
```

контейнер запускает приложение `time_tracker`. Если не передать аргументы, программа открывает интерактивное меню.

Демонстрационный запуск выполняется отдельной командой:

```sh
docker run --rm time-tracker-kr5 --demo
```

Это демонстрационный режим: он показывает, что приложение внутри контейнера работает.

## Передача аргументов командной строки

В задании требовалось организовать передачу аргументов командной строки.

Для этого в `src/main.cpp` добавлены команды:

```text
time_tracker                         interactive menu
time_tracker --demo                  run container demo
time_tracker [--data DIR] add-task TITLE [DESCRIPTION]
time_tracker [--data DIR] list-tasks
time_tracker [--data DIR] archive-task ID
time_tracker [--data DIR] delete-task ID
time_tracker [--data DIR] start ID
time_tracker [--data DIR] stop
time_tracker [--data DIR] history
time_tracker [--data DIR] report
```

Примеры Docker-запуска:

```sh
docker run --rm time-tracker-kr5 --help
docker run --rm time-tracker-kr5 add-task "Write report" "Prepare Docker section"
docker run --rm time-tracker-kr5 list-tasks
docker run --rm time-tracker-kr5 report
```

Что сказать: после имени образа можно передать аргументы, и Docker передаст их программе `time_tracker`.

## Что изменилось в `main.cpp`

В предыдущих работах `main.cpp` сразу запускал интерактивное меню.

В пятой работе добавлена обработка аргументов:

- `--help` - показать справку;
- `--demo` - выполнить демонстрацию для контейнера;
- `add-task` - создать задачу;
- `list-tasks` - вывести список задач;
- `archive-task` - архивировать задачу;
- `delete-task` - удалить задачу и связанные с ней рабочие сессии;
- `start` - запустить таймер;
- `stop` - остановить таймер;
- `history` - показать историю сессий;
- `report` - показать отчет;
- `--data DIR` - указать другой каталог данных.

При этом старое поведение сохранено: если аргументов нет, создается `ConsoleUI` и запускается меню.

## Хранение данных в контейнере

По умолчанию приложение хранит данные в:

```text
/app/data
```

Но контейнеры обычно временные. Если запустить контейнер с `--rm`, данные исчезнут после завершения.

Чтобы сохранить данные между запусками, используется Docker volume:

```sh
docker volume create time-tracker-data
docker run --rm -v time-tracker-data:/app/data time-tracker-kr5 add-task "Docker demo"
docker run --rm -v time-tracker-data:/app/data time-tracker-kr5 list-tasks
```

Что сказать: volume подключает постоянное хранилище к папке `/app/data` внутри контейнера.

## Тесты внутри Docker

Тесты запускаются прямо во время сборки образа:

```sh
ctest --test-dir build --output-on-failure
```

Это важно, потому что проверяется не только локальная сборка на компьютере, а сборка в чистом Linux-окружении.

Если тесты упадут, строка `RUN` в Dockerfile завершится ошибкой, и финальный runtime-образ не будет создан.

Что сказать: это гарантирует, что в образ попадет только версия проекта, которая успешно собрана и протестирована.

## Почему финальный образ не содержит временных файлов

Временные файлы сборки остаются на стадии `build`.

В стадию `runtime` копируется только:

```text
/usr/local/bin/time_tracker
```

и создается папка:

```text
/app/data
```

То есть в финальном образе нет:

- исходников;
- CMake-кэша;
- объектных файлов;
- папки `build`;
- скачанного Catch2.

Что сказать: это уменьшает размер образа и соответствует требованию задания.

## Пользователь внутри контейнера

В Dockerfile создается отдельный пользователь:

```dockerfile
useradd --create-home --shell /usr/sbin/nologin appuser
```

Потом контейнер запускается от него:

```dockerfile
USER appuser
```

Что сказать: это хорошая практика, потому что приложение не запускается от root-пользователя.

## Как данные проходят при запуске команды

Пример:

```sh
docker run --rm time-tracker-kr5 add-task "Docker demo"
```

Последовательность:

1. Docker запускает контейнер.
2. `ENTRYPOINT` вызывает `time_tracker`.
3. Аргументы `add-task "Docker demo"` передаются в программу.
4. `main.cpp` разбирает аргументы.
5. Создается `Storage` для папки `/app/data`.
6. Загружаются существующие задачи.
7. `TaskManager::addTask` создает новую задачу.
8. `Storage::saveTasks` сохраняет данные.
9. Контейнер завершает работу.

## Что показать преподавателю

1. Открыть `KR5/Dockerfile`.
2. Показать две стадии: `build` и `runtime`.
3. Объяснить установку зависимостей в build-стадии.
4. Показать строку с `cmake`, `cmake --build` и `ctest`.
5. Показать `COPY --from=build`, где в runtime копируется только бинарник.
6. Показать `ENTRYPOINT`.
7. Открыть `.dockerignore` и объяснить, зачем он нужен.
8. Открыть `src/main.cpp` и показать команды CLI.
9. Запустить:

```sh
docker build -t time-tracker-kr5 .
docker run --rm -it time-tracker-kr5
docker run --rm time-tracker-kr5 --help
```

## Короткое объяснение всей работы

Можно сказать так:

> В пятой контрольной я контейнеризировал проект через Docker. В Dockerfile используется многостадийная сборка: на стадии `build` устанавливаются CMake, компилятор и Git, затем проект собирается и запускаются тесты Catch2. На стадии `runtime` остается только готовый исполняемый файл. Также я добавил запуск приложения через аргументы командной строки, чтобы контейнер можно было использовать командами `add-task`, `list-tasks`, `delete-task`, `report` и другими. Без аргументов контейнер открывает интерактивное меню, а для быстрой проверки есть режим `--demo`.

## Возможные вопросы и ответы

**Что такое Dockerfile?**

Dockerfile - это инструкция для Docker, как собрать образ: какой базовый образ взять, какие зависимости поставить, какие файлы скопировать и какую команду запускать.

**Зачем нужна многостадийная сборка?**

Чтобы разделить среду сборки и среду запуска. В первой стадии есть компилятор и CMake, а во второй только готовая программа.

**Почему тесты запускаются при `docker build`?**

Чтобы не создать финальный образ, если проект не собирается или тесты не проходят.

**Что делает `ENTRYPOINT`?**

Он задает основную команду контейнера. В данном случае контейнер всегда запускает `time_tracker`.

**Как передать другую команду в контейнер?**

Нужно написать ее после имени образа:

```sh
docker run --rm time-tracker-kr5 --help
```

**Почему нужен `.dockerignore`?**

Чтобы не копировать в Docker-контекст лишние файлы: локальные сборки, временные файлы, `.git`, `.exe`, CSV-данные.

**Где хранятся данные внутри контейнера?**

В папке `/app/data`.

**Как сохранить данные между запусками?**

Подключить volume:

```sh
docker run --rm -v time-tracker-data:/app/data time-tracker-kr5 list-tasks
```

**Почему контейнер запускается не от root?**

Это более безопасная практика. Для приложения создается отдельный пользователь `appuser`.
