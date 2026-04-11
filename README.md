# Slog 
### High-Performance Async Logger for C++20

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build](https://img.shields.io/badge/build-xmake-orange.svg)](https://xmake.io)
[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20macos-lightgrey.svg)]()

---

# English

## Overview

Slog is a lightweight asynchronous logging library built for real-time C++ applications — game engines, simulations, embedded systems. Logging never blocks your main thread.

## Architecture

```
Main Thread                    Worker Thread
──────────────────             ──────────────────────────
LOG_ERROR("msg")  ──push──►  queue  ──pop──►  terminalLog
                                            └──►  fileLog
                                            └──►  yourLog
```

Producer-consumer model. The main thread formats the message and pushes it into a thread-safe queue. A dedicated background worker drains the queue and dispatches to all registered output systems.

## Quick Start

### 1. Clone

```bash
git clone https://github.com/grechka731/Slog.git
cd Slog
```

### 2. Build

```bash
xmake              # build library + demo binary
xmake run SlogApp  # run demo
```

### 3. As submodule

```bash
git submodule add https://github.com/grechka731/Slog.git external/Slog
```

`xmake.lua` of your project:

```lua
includes("external/Slog")

target("MyApp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_deps("Slog")
```

## Usage

### Basic example

```cpp
#include "logger.h"
#include "terminalLog.h"
#include "fileLog.h"

int main() {
    logger.addSystem(new terminalLog);
    logger.addSystem(new fileLog("app.log"));

    LOG_DEBUG("loading assets...");       // stripped in release build
    LOG_INFO("engine initialized");       // stripped in release build
    LOG_WARN("low memory: 512mb free");
    LOG_ERROR("shader compile failed");
    LOG_CRITICAL("gpu device lost");
}
```

Output:
```
[2026-04-12 14:32:01] [DEBUG  ] [main.cpp:8]  [main] - loading assets...
[2026-04-12 14:32:01] [INFO   ] [main.cpp:9]  [main] - engine initialized
[2026-04-12 14:32:01] [WARNING] [main.cpp:10] [main] - low memory: 512mb free
[2026-04-12 14:32:01] [ERROR  ] [main.cpp:11] [main] - shader compile failed
[2026-04-12 14:32:01] [CRITICAL][main.cpp:12] [main] - gpu device lost
```

## Log Levels

| Level | Macro | Release build | Use case |
|---|---|---|---|
| `DEBUG` | `LOG_DEBUG(msg)` | stripped | Verbose dev info |
| `INFO` | `LOG_INFO(msg)` | stripped | General events |
| `WARNING` | `LOG_WARN(msg)` | included | Degraded state |
| `ERROR` | `LOG_ERROR(msg)` | included | Recoverable failure |
| `CRITICAL` | `LOG_CRITICAL(msg)` | included | Fatal, stop execution |

## Runtime Terminal Styles

Each log level has an independent `Style` that can be changed at any point while the program is running.

### Style struct

```cpp
struct Style {
    uint8_t r, g, b;       // text color RGB, range 0-255
    uint8_t bgR, bgG, bgB; // background color RGB, range 0-255
    bool bg = false;       // enable background (off by default)
};
```

| Field | Type | Default | Description |
|---|---|---|---|
| `r, g, b` | `uint8_t` | `255,255,255` | Text color (RGB) |
| `bgR, bgG, bgB` | `uint8_t` | `0,0,0` | Background color (RGB) |
| `bg` | `bool` | `false` | Must be `true` for background to render |

### Changing styles at runtime

Keep a pointer to `terminalLog` before passing it to `addSystem`. Call `setStyle` at any point — changes apply on the next log call.

```cpp
int main() {
    auto* term = new terminalLog;
    logger.addSystem(term);
    logger.addSystem(new fileLog("app.log"));

    LOG_INFO("default green");
    LOG_WARN("default yellow");

    // orange WARNING
    term->setStyle(WARNING, {255, 140, 0,  0, 0, 0,  false});

    // white text on dark red background for CRITICAL
    term->setStyle(CRITICAL, {255, 255, 255,  160, 0, 0,  true});

    // dim grey DEBUG
    term->setStyle(DEBUG, {120, 120, 120,  0, 0, 0,  false});

    LOG_WARN("now orange");
    LOG_CRITICAL("white on red");
    LOG_DEBUG("dim grey");
}
```

### Style constructor layout

```cpp
Style {r, g, b, bgR, bgG, bgB, bg}
//     │  │  │   │    │    │    └─ bool: enable background
//     │  │  │   └────┴────┴───── background RGB
//     └──┴──┴──────────────────── foreground RGB
```

Color examples:

```cpp
{255, 0,   0,    0,  0,   0,  false}  // red text
{255, 255, 255,  0,  80, 180, true}   // white on blue
{255, 220, 0,    0,  0,   0,  false}  // yellow text
{180, 255, 180,  0,  40,  0,  true}   // green on dark green
```

### Default styles

```cpp
DEBUG    → {150, 150, 150,  0, 0, 0,  false}  // grey
INFO     → {100, 200, 100,  0, 0, 0,  false}  // green
WARNING  → {255, 200,   0,  0, 0, 0,  false}  // yellow
ERROR    → {255, 100, 100,  0, 0, 0,  false}  // light red
CRITICAL → {255, 255, 255,  180, 0, 0, true}  // white on red
```

## Custom Output System

Implement `logSystem` to add any output target:

```cpp
#include "logSystem.h"

class udpLog : public logSystem {
public:
    udpLog(const char* host, int port) { /* setup socket */ }

    void addLog(const char* msg, const LogLevel level) override {
        // send msg over UDP
    }
};

logger.addSystem(new udpLog("127.0.0.1", 9000));
logger.addSystem(new terminalLog);
logger.addSystem(new fileLog("app.log"));
```

## xmake.lua

```lua
add_rules("mode.debug", "mode.release")

target("Slog")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    if is_mode("release") then
        add_defines("NDEBUG")
    end

target("SlogApp")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("Slog")
    add_includedirs("include")
```

---
---

# Русский

## Обзор

Slog — легковесный асинхронный логгер для C++ приложений реального времени: игровых движков, симуляций, встраиваемых систем. Логирование никогда не блокирует основной поток.

## Архитектура

```
Основной поток                 Рабочий поток
──────────────────             ──────────────────────────
LOG_ERROR("msg")  ──push──►  queue  ──pop──►  terminalLog
                                            └──►  fileLog
                                            └──►  yourLog
```

Модель производитель-потребитель. Основной поток форматирует сообщение и кладёт его в потокобезопасную очередь. Фоновый воркер вычитывает очередь и отправляет во все зарегистрированные системы вывода.

## Быстрый старт

### 1. Клонирование

```bash
git clone https://github.com/grechka731/Slog.git
cd Slog
```

### 2. Сборка

```bash
xmake              # собрать библиотеку + демо
xmake run SlogApp  # запустить демо
```

### 3. Как сабмодуль

```bash
git submodule add https://github.com/grechka731/Slog.git external/Slog
```

`xmake.lua` вашего проекта:

```lua
includes("external/Slog")

target("MyApp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_deps("Slog")
```

## Использование

### Базовый пример

```cpp
#include "logger.h"
#include "terminalLog.h"
#include "fileLog.h"

int main() {
    logger.addSystem(new terminalLog);        // вывод в терминал
    logger.addSystem(new fileLog("app.log")); // запись в файл

    LOG_DEBUG("загрузка ассетов...");         // вырезается в release
    LOG_INFO("движок инициализирован");       // вырезается в release
    LOG_WARN("мало памяти: 512мб");
    LOG_ERROR("ошибка компиляции шейдера");
    LOG_CRITICAL("потеряно GPU устройство");
}
```

Вывод:
```
[2026-04-12 14:32:01] [DEBUG  ] [main.cpp:8]  [main] - загрузка ассетов...
[2026-04-12 14:32:01] [INFO   ] [main.cpp:9]  [main] - движок инициализирован
[2026-04-12 14:32:01] [WARNING] [main.cpp:10] [main] - мало памяти: 512мб
[2026-04-12 14:32:01] [ERROR  ] [main.cpp:11] [main] - ошибка компиляции шейдера
[2026-04-12 14:32:01] [CRITICAL][main.cpp:12] [main] - потеряно GPU устройство
```

## Уровни логирования

| Уровень | Макрос | Release сборка | Назначение |
|---|---|---|---|
| `DEBUG` | `LOG_DEBUG(msg)` | вырезается | Подробная отладка |
| `INFO` | `LOG_INFO(msg)` | вырезается | Общие события |
| `WARNING` | `LOG_WARN(msg)` | включается | Деградация состояния |
| `ERROR` | `LOG_ERROR(msg)` | включается | Восстановимая ошибка |
| `CRITICAL` | `LOG_CRITICAL(msg)` | включается | Фатально, стоп |

## Стили терминала в рантайме

Каждый уровень логирования имеет независимый `Style`, который можно менять в любой момент работы программы.

### Структура Style

```cpp
struct Style {
    uint8_t r, g, b;       // цвет текста RGB, диапазон 0-255
    uint8_t bgR, bgG, bgB; // цвет фона RGB, диапазон 0-255
    bool bg = false;       // включить фон (по умолчанию выключен)
};
```

Описание полей:

| Поле | Тип | По умолчанию | Описание |
|---|---|---|---|
| `r, g, b` | `uint8_t` | `255,255,255` | Цвет текста (RGB) |
| `bgR, bgG, bgB` | `uint8_t` | `0,0,0` | Цвет фона (RGB) |
| `bg` | `bool` | `false` | Включить фон — если `false`, поля bgR/bgG/bgB игнорируются |

### Как менять стили в рантайме

Сохрани указатель на `terminalLog` до передачи в `addSystem`. Затем вызывай `setStyle` в любой момент — изменения применяются при следующем вызове лога.

```cpp
int main() {
    auto* term = new terminalLog;
    logger.addSystem(term);
    logger.addSystem(new fileLog("app.log"));

    // дефолтные стили уже установлены, логи работают сразу
    LOG_INFO("зелёный по умолчанию");
    LOG_WARN("жёлтый по умолчанию");

    // WARNING → оранжевый
    term->setStyle(WARNING, {255, 140, 0,  0, 0, 0,  false});

    // CRITICAL → белый текст на тёмно-красном фоне
    term->setStyle(CRITICAL, {255, 255, 255,  160, 0, 0,  true});

    // DEBUG → приглушённый серый
    term->setStyle(DEBUG, {120, 120, 120,  0, 0, 0,  false});

    LOG_WARN("теперь оранжевый");
    LOG_CRITICAL("белый на красном фоне");
    LOG_DEBUG("приглушённый серый");
}
```

### Формат конструктора Style

```cpp
Style {r, g, b, bgR, bgG, bgB, bg}
//     │  │  │   │    │    │    └─ bool: включить фон
//     │  │  │   └────┴────┴───── RGB фона
//     └──┴──┴──────────────────── RGB текста
```

Примеры цветов:

```cpp
{255, 0,   0,    0,  0,   0,  false}  // красный текст
{255, 255, 255,  0,  80, 180, true}   // белый на синем фоне
{255, 220, 0,    0,  0,   0,  false}  // жёлтый текст
{180, 255, 180,  0,  40,  0,  true}   // зелёный на тёмно-зелёном фоне
```

### Дефолтные стили

```cpp
DEBUG    → {150, 150, 150,  0, 0, 0,  false}  // серый
INFO     → {100, 200, 100,  0, 0, 0,  false}  // зелёный
WARNING  → {255, 200,   0,  0, 0, 0,  false}  // жёлтый
ERROR    → {255, 100, 100,  0, 0, 0,  false}  // светло-красный
CRITICAL → {255, 255, 255,  180, 0, 0, true}  // белый на красном
```

## Своя система вывода

Реализуй интерфейс `logSystem` чтобы добавить любой канал вывода:

```cpp
#include "logSystem.h"

class udpLog : public logSystem {
public:
    udpLog(const char* host, int port) { /* настройка сокета */ }

    void addLog(const char* msg, const LogLevel level) override {
        // отправить msg по UDP
    }
};

logger.addSystem(new udpLog("127.0.0.1", 9000));
logger.addSystem(new terminalLog);
logger.addSystem(new fileLog("app.log"));
```

## xmake.lua

```lua
add_rules("mode.debug", "mode.release")

target("Slog")
    set_kind("static")
    add_files("src/*.cpp")
    add_includedirs("include", {public = true})
    if is_mode("release") then
        add_defines("NDEBUG")
    end

target("SlogApp")
    set_kind("binary")
    add_files("main.cpp")
    add_deps("Slog")
    add_includedirs("include")
```

---

## License / Лицензия

MIT © 2026 [grechka](https://github.com/grechka731)
