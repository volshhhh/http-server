# Примитивный HTTP-сервер

Простой HTTP-сервер, реализующий базовые функции для работы с запросами.

## Основные возможности

1) **User-Agent** - получение информации о клиенте  
2) **Работа с файлами** - создание, запись и чтение файлов  
3) **Echo-запросы** - возврат полученных данных  
4) **Модульная архитектура** - легко добавить новые функции  

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

Для работы с файлами укажите директорию:
```bash
./server --directory /path/to/files/
```

## Разработка новых функций

Чтобы добавить новую функциональность:

1. Добавьте новую команду, с которой будет ассоциироваться ваша функция, в `Command.h`:
   ```cpp
   enum class Command {
       // существующие команды
       NEW_COMMAND  // ваша новая команда
   };
   ```

2. Создайте билдер ответов (наследуйтесь от `ResponseBuilder`):
   ```cpp
   class NewResponseBuilder : public ResponseBuilder {
   public:
       Response build(const HttpRequest& request) override {
           // ваша реализация
       }
   };
   ```

3. Зарегистрируйте билдер в `ResponseFactory`:
   ```cpp
   registerBuilder(Command::NEW_COMMAND,
                  []() { return std::make_unique<NewResponseBuilder>(); });
   ```