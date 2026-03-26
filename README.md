# C++ Logger

Простой, потокобезопасный, header-only логгер для C++17.

## Возможности
- Уровни логирования (DEBUG, INFO, WARN, ERROR)
- Потокобезопасность (std::mutex)
- Вывод в консоль или в файл
- Микросекундные метки времени
- Легкое подключение через CMake

## Подключение

### Вариант 1: Git Submodule
```bash
git submodule add https://github.com/ВАШ_НИК/cpp-logger.git libs/logger
add_subdirectory(libs/logger)
target_link_libraries(your_target PRIVATE cpp_logger)
```

### Вариант 2: FetchContent
```bash
include(FetchContent)
FetchContent_Declare(
  cpp_logger
  GIT_REPOSITORY https://github.com/ВАШ_НИК/cpp-logger.git
  GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(cpp_logger)
```

## Использование
```bash
#include <logger/logger.hpp>

SHOW_INFO("App", "Hello World");
logger::Logger::set_output_file("log.txt");
SHOW_ERROR("Net", "Connection lost");
```

