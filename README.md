
# 🚀 Slog: High-Performance Async Logger

[![C++ Version](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build Tool](https://img.shields.io/badge/build-xmake-orange.svg)](https://xmake.io)

**Slog** — это легковесный асинхронный логгер, созданный для минимизации задержек в игровых движках (RICA Engine).

Slog: High-Performance Asynchronous Logging Framework for C++. Overview. Slog is a lightweight, asynchronous logging library designed for high-performance C++ applications, specifically game engines. By decoupling the logging invocation from the I/O operations, Slog ensures minimal impact on the main execution thread's frame rate (FPS).

Architecture. The framework is built upon an asynchronous producer-consumer model. The producer, being the main application thread, formats the log string and pushes a task into a thread-safe queue. The consumer is a dedicated background worker thread that waits for new entries using condition variables and executes the I/O operations sequentially.

Key Features. The library provides asynchronous I/O where logging operations are performed on a secondary thread to prevent main thread blocking. It features zero-overhead in release mode; using preprocessor macros, DEBUG and INFO levels are completely stripped from the binary in Release builds, eliminating function call overhead. The extensible system uses an abstract interface that allows for easy implementation of custom output targets. Thread safety is managed via std::mutex and std::condition_variable.

Integration. To integrate Slog as a submodule in your project, include it in your xmake.lua by adding includes("external/Slog") and then add_deps("Slog") to your target. By replacing std::endl with "\n" and utilizing a background worker, Slog avoids frequent disk flushes, maintaining peak performance even during intensive logging periods.

Slog: Асинхронный движок логирования. Техническое описание. Slog - это высокопроизводительный фреймворк логирования на C++, разработанный для минимизации задержек в системах реального времени. Он использует асинхронную архитектуру для выноса операций ввода-вывода за пределы критического пути выполнения программы.

Архитектура. Фреймворк построен на базе асинхронной модели производитель-потребитель. Основной поток приложения формирует строку лога и помещает задачу в потокобезопасную очередь. Выделенный фоновый рабочий поток ожидает появления новых записей с помощью переменных состояния и последовательно выполняет операции вывода в терминал или файл.

Ключевые особенности. Асинхронная обработка реализует модель с использованием std::condition_variable и std::mutex для исключения блокировок основного потока. Оптимизация при компиляции использует макросы препроцессора для полного удаления уровней DEBUG и INFO в релизных сборках. Абстрактный интерфейс позволяет создавать модульные системы вывода, а сама библиотека реализована на стандарте C++20 без внешних зависимостей.

Интеграция. Добавьте библиотеку как сабмодуль и подключите в xmake.lua через команды includes и add_deps. Благодаря отказу от принудительного сброса буфера на диск при каждой записи, Slog сохраняет пиковую производительность игрового движка даже при очень интенсивном потоке логов.
