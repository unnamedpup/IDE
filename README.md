
# IDE for JSON-language

<a href="https://github.com/unnamedpup/IDE/actions/workflows/c-cpp.yml"><img src="https://github.com/unnamedpup/IDE/actions/workflows/c-cpp.yml/badge.svg?branch=master" alt="Build Status"></a>

Текстовый редактор для нового языка программирования основанного на JSON.
Реализован на языке программирования C++ при помощи библиотеки ncursesw.

## Список команд нового языка программирования

1.  "ввод" - ввести значение и положить его во внутренний стек.
2.  "вывод" - взять значение из стека и вывести его.
3.  "взять" имеет два варианта:
    - если команда - это структура, и в ней есть поле с ключом <<что>>, содержимое этого поля кладется в стек.
    - если нет, то из стека берется ссылка, и вместо нее кладется то, на что она ссылается. 
4.  "положить" - берет из стека ссылку и значение и записывает значение по указанной ссылке.
5.  число  положить его в стек данных.
6.  объект - взять у него значение атрибута "имя" и выполнить как команду (весь объект команды передается функции, выполняющей эту команду, как единственный параметр, по ссылке).
7.  строка - выполнить команду с таким именем, в том числе: арифметическая, логическая, операция сравнения - взять из стека нужное количество чисел, вычислить выражение и положить результат в стек.
8.  "метка" - такая команда должна быть объектом с дополнительным полем с ключом "метка", в котором записана строка - имя метки. Как команда, ничего не делает и служит только для того, чтобы пометить определенное место в последовательности команд.
9.  "переход" -  такая команда должна быть объектом с дополнительным полем с ключом "куда", в котором записана строка - имя метки. Как команда, вызывает передачу управления на метку с указанным именем.
10. "если" - такая команда должна быть объектом с дополнительными полями с ключами "да" и "нет", в которых записаны строки - имена меток. Как команда, берет значение из стека и вызывает передачу управления на метку с именем под ключом "да", если взятое из стека значение отличается от 0, и "нет" в противном случае. Поле с ключом "нет" может отсутствовать, в этом случае, если снятое из стека значение равно 0, управление, как обычно, переходит на следующую по порядку команду.

Пример:

```
[
    "ввод",
    {"имя": "взять", "что": {"режим":"регистр", "имя":"а"}},
    "положить",
    "ввод",
    {"имя": "взять", "что": {"режим":"регистр", "имя":"б"}},
    "положить",
    {"имя": "взять", "что": {"режим":"регистр", "имя":"а"}},
    "взять",
    {"имя": "взять", "что": {"режим":"регистр", "имя":"б"}},
    "взять",
    "*",
    "вывод"
]
```

В примере выше представлена запись программы, которая вычисляет произведение двух введенных чисел.

## Запуск и тестирование

#### Запуск 

1. Убедитесь, что у вас установлен CMake и компилятор C++.
2. Клонируйте репозиторий
3. Перейдите в директорию с проектом: `cd IDE`
4. Создайте директорию для сборки: `mkdir build && cd build`
5. Соберите проект: `cmake .. && make`
6. Запустите проект из директории IDE: `./build/json-IDE`
   
#### Тестирование

1. Убедитесь, что вы создали директорию для сборки проекта: `mkdir build && cd build`
2. Убедитесь, что вы уже собрали проект: `cmake .. && make`
3. Запустите проект  из директории json-IDE: `./build/json-IDE`
4. Откройте еще одно окно терминала
5. Перейдите в /build : `cd IDE/build`
6. Запустите тесты: `./tests/tests`

## Использование

Этот редактор представляет собой приложение с текстовым пользовательским интерфейсом (TUI), взаимодействие с которым осуществляется с помощью клавиатуры.

### Навигация и управление:

- Для переключения между основными окнами используется клавиша ESC.
- Для выбора функции в панели управления используется клавиша ENTER.

### Рекомендации по работе с файлами.

Для более удобной работы с файлами рекомендуется хранить их в той же директории, что и исполняемый файл редактора. В противном случае, необходимо указывать полный путь к файлу при открытии.
