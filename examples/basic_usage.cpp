#include <logger/logger.hpp>

int main() {
    // 1. Логирование в консоль (по умолчанию)
    SHOW_INFO("Main", "Запуск приложения (консоль)");
    SHOW_DEBUG("Main", "Это сообщение не видно, так как уровень INFO");

    logger::Logger::setLevel(logger::Level::DEBUG);
    SHOW_DEBUG("Main", "Теперь DEBUG виден");

    // 2. Переключение в файл
    logger::Logger::setOutputFile("app.log");
    SHOW_INFO("File", "Эта запись пойдет в файл app.log");
    SHOW_ERROR("DB", "Ошибка подключения к БД (тоже в файл)");

    // 3. Возврат в консоль
    logger::Logger::setOutputConsole();
    SHOW_INFO("Main", "Снова пишем в консоль");

    return 0;
}
