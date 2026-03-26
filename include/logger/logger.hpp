#pragma once

#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

namespace logger {
enum class Level : int { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

inline std::atomic<Level> g_log_level{Level::INFO};

// Хранилище для потока вывода. По умолчанию указывает на clog (консоль)
inline std::ostream* g_log_stream = &std::clog;
// Умный указатель для хранения файла, если он открыт (чтобы автоматически закрыть)
inline std::unique_ptr<std::ofstream> g_log_file = nullptr;

inline std::string timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto time_t_now = std::chrono::system_clock::to_time_t(now);
    const auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;

    std::tm tm_buf{};
#if defined(_WIN32) || defined(_MSC_VER)
    localtime_s(&tm_buf, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_buf);
#endif
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

class Logger {
public:
    static void setLevel(Level level) {
        g_log_level.store(level);
    }

    // Переключение вывода в файл
    static void setOutputFile(const std::string& filename) {
        std::scoped_lock<std::mutex> lock(config_mutex_);

        // Закрываем старый файл, если был
        g_log_file.reset();
        g_log_file = std::make_unique<std::ofstream>(filename, std::ios::app);

        if (g_log_file->is_open()) {
            g_log_stream = g_log_file.get();
        } else {
            // Если файл не открылся, возвращаем вывод в консоль и кидаем ошибку
            g_log_stream = &std::clog;
            std::cerr << "[LOGGER ERROR] Cannot open file: " << filename
                      << ". Reverting to console." << std::endl;
        }
    }

    // Метод для возврата вывода в консоль
    static void setOutputConsole() {
        std::scoped_lock<std::mutex> lock(config_mutex_);
        g_log_file.reset();
        g_log_stream = &std::clog;
    }

    static void log(const Level level, const std::string& module, const std::string& message) {
        if (level < g_log_level.load()) {
            return;
        }

        std::scoped_lock<std::mutex> lock(cout_mutex_);

        const char* level_str = " ";
        switch (level) {
            case Level::DEBUG:
                level_str = "[DEBG] ";
                break;
            case Level::INFO:
                level_str = "[INFO] ";
                break;
            case Level::WARN:
                level_str = "[WARN] ";
                break;
            case Level::ERROR:
                level_str = "[ERRR] ";
                break;
        }

        // Пишем в выбранный поток (файл или консоль)
        *g_log_stream << "[" << timestamp() << "] " << level_str << "[" << module
                      << "]: " << message << "\n";

        // Важно: если пишем в файл, нужно сбрасывать буфер, чтобы видеть логи сразу
        g_log_stream->flush();
    }

private:
    inline static std::mutex cout_mutex_;
    inline static std::mutex config_mutex_;
};

// Макросы для удобства
#define SHOW_DEBUG(module, msg) logger::Logger::log(logger::Level::DEBUG, module, msg)
#define SHOW_INFO(module, msg) logger::Logger::log(logger::Level::INFO, module, msg)
#define SHOW_WARN(module, msg) logger::Logger::log(logger::Level::WARN, module, msg)
#define SHOW_ERROR(module, msg) logger::Logger::log(logger::Level::ERROR, module, msg)

} // namespace logger
