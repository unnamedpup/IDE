
# IDE for JSON-language

<a href="https://github.com/unnamedpup/IDE/actions/workflows/c-cpp.yml"><img src="https://github.com/unnamedpup/IDE/actions/workflows/c-cpp.yml/badge.svg?branch=master" alt="Build Status"></a>

Текстовый редактор для нового языка программирования основанного на JSON.
Реализован на языке программирования C++ при помощи библиотеки ncursesw.

## Установка и запуск

1. Убедитесь, что у вас установлен компилятор g++ (GNU Compiler Collection) для C++.
2. Убедитесь, что у вас установлена библиотека ncursesw. 
Скачать ее для Ubuntu или Debian можно следюущим образом:

```
sudo apt update
sudo apt install libncursesw5-dev
```

3. Клонируйте репозиторий.
4. Перейдите в директорию с проектом: ``` cd IDE ```
5. Соберите проект: ``` make ```
6. Запустите проект: ``` ./ide ```

## Использование

Этот редактор представляет собой приложение с текстовым пользовательским интерфейсом (TUI), взаимодействие с которым осуществляется с помощью клавиатуры.

### Навигация и управление:

- Для переключения между основными окнами используется клавиша ESC.
- Для выбора функции в панели управления используется клавиша ENTER.

### Рекомендации по работе с файлами.

Для более удобной работы с файлами рекомендуется хранить их в той же директории, что и исполняемый файл редактора. В противном случае, необходимо указывать полный путь к файлу при открытии.
