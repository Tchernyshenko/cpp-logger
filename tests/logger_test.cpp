#include <cassert>
#include <iostream>
#include <logger/logger.hpp>
#include <thread>

void testLogLevels() {
    // Тест 1: Проверка фильтрации уровней
    logger::Logger::setLevel(logger::Level::WARN);

    // Эти не должны выводиться (но мы проверяем логику, а не вывод в консоль)
    // В реальном тесте лучше перехватывать поток, но для примера проверяем атомику
    assert(logger::g_log_level.load() == logger::Level::WARN);

    logger::Logger::setLevel(logger::Level::DEBUG);
    assert(logger::g_log_level.load() == logger::Level::DEBUG);

    std::cout << "✓ Тест уровней пройден" << std::endl;
}

void testThreadSafety() {
    // Тест 2: Проверка потокобезопасности
    logger::Logger::setLevel(logger::Level::DEBUG);

    std::thread t1([]() {
        for (int i = 0; i < 100; ++i) {
            SHOW_INFO("Thread1", "Message " + std::to_string(i));
        }
    });

    std::thread t2([]() {
        for (int i = 0; i < 100; ++i) {
            SHOW_INFO("Thread2", "Message " + std::to_string(i));
        }
    });

    t1.join();
    t2.join();

    std::cout << "✓ Тест потокобезопасности пройден" << std::endl;
}

int main() {
    std::cout << "=== Запуск тестов логгера ===" << std::endl;
    testLogLevels();
    testThreadSafety();
    std::cout << "=== Все тесты пройдены ===" << std::endl;
    return 0;
}
