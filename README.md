# Примитивный HTTP-сервер

Простой HTTP-сервер с модульной архитектурой для обработки базовых запросов.

## Основные возможности

1) **User-Agent** - получение информации о клиенте  
2) **Работа с файлами** - создание, чтение и запись  
3) **Echo-запросы** - возврат полученных данных  
4) **Модульная архитектура** - лёгкое добавление новых функций  

## Быстрый старт

### Сборка проекта
```bash
mkdir build && cd build
cmake ..
make
```

### Запуск сервера
Базовый запуск:
```bash
./server
```

С указанием рабочей директории:
```bash
./server --directory /path/to/files/
```

## Разработка новых функций

### 1. Добавление новой команды

Добавьте enum-значение в `Command.h`:
```cpp
enum class Command {
    // существующие команды
    NEW_COMMAND  ///< Описание функциональности команды
};
```

**Формат имени команды в запросе:**
```
[RequestType]_UPPER_CASE_NAME
```
Где:
- `RequestType` (опционально) - GET/POST
- `UPPER_CASE_NAME` - имя команды в верхнем регистре с подчёркиваниями вместо дефисов
(так как используется функционал `magic_enum.hpp` для более удобной конвертации команд в строку)
**Примеры допустимых команд:**
```
GET_USER_STATUS
POST_UPDATE_SETTINGS
```

### 2. Создание билдера ответов

Реализуйте класс-обработчик:
```cpp
class NewResponseBuilder : public ResponseBuilder {
public:
    HttpResponse build(const HttpRequest& request,
                      const std::optional<std::string>& dir = std::nullopt) override 
    {
        // Ваша реализация обработки запроса
        HttpResponse response;
        response.setStatus(HttpStatus::OK);
        response.setBody("Response data");
        return response;
    }
};
```

### 3. Регистрация обработчика

Добавьте фабричную функцию в `ResponseFactory`:
```cpp
factory.registerBuilder(
    Command::NEW_COMMAND,
    []() { return std::make_unique<NewResponseBuilder>(); }
);
```