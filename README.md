# StankinScheduleEditor2
*StankinScheduleEditor2* - не официальный редактор и генератор еженедельного расписания для МГТУ "СТАНКИН".

Данный репозиторий является продолжением старого [StankinScheduleEditor](https://github.com/Nikololoshka/StankinScheduleEditor),
но с улучшенной произодительностью при работе и исправлением многих ошибок.

Цель создания проекта - это возможность перевода расписаний из PDF в JSON с последующей генерации альтернативного (еженедельного, с дополнениями и т.п.) 
расписания или использование получаемых JSON в других программах (например, при создании расписания для Google Calendar).

## Возможности
- [x] Парсинг расписаний из PDF;
- [x] Создание и редактирование расписаний;
- [ ] Генерация расписания (еженедельного и общего).

## Установка
Для сборки необходим компилятор, поддерживаем стандарт C++ 17, библиотека Qt5 (Core, Widgets и Gui), TesseractAPI, Poppler-Cpp и OpenCV4.

Для Tesseract необходимо скачать отдельно rustraineddata для русского языка.

### Windows
Установка всех зависимостей возможна через Vcpkg (или другой менеджер пакетов).
Также, вместо Vcpkg, библиотеку Qt5 можно подключить используя уже устанавленый Qt с QtCreator.

```cmd 
vcpkg install tesseract:x64-windows 
vcpkg install opencv4:x64-windows
vcpkg install qt5-base:x64-windows
vcpkg install poppler:x64-windows
vcpkg integrate install
```

### Linux 
```shell 
sudo apt-get install qt5-default
sudo apt-get install python3-opencv
sudo apt-get install libtesseract-dev
sudo apt-get install libpoppler-dev
```
  
