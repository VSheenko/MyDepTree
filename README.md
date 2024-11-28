# Задание
Разработать инструмент командной строки для визуализации графа зависимостей, включая транзитивные зависимости. Сторонние программы или библиотеки для получения зависимостей использовать нельзя.

Зависимости определяются для файла-пакета языка Python (pip). Для описания графа зависимостей используется представление PlantUML. Визуализатор должен выводить результат на экран в виде графического
изображения графа.

Ключами командной строки задаются:
- Путь к программе для визуализации графов.
- Путь к анализируемому пакету.
- Максимальная глубина анализа зависимостей.



Установка зависимостей

- [curl](https://curl.se/libcurl/) - libcurl — бесплатная и простая в использовании библиотека передачи URL-адресов на стороне клиента.
- [libzippp](https://github.com/ctabin/libzippp) - libzippp — это простая базовая оболочка C++ вокруг библиотеки libzip. Это портативная и простая в использовании библиотека для работы с ZIP-файлами.
- [nlohmann/json](https://github.com/nlohmann/json) - JSON для современного C++.
- [Google Test](https://github.com/google/googletest) - это популярная библиотека для модульного тестирования C++ приложений.
  Установите их через vcpkg:
```bash
vcpkg install curl libzippp gtest nlohmann-json
```

# Сборка
Сборка осуществляется как для обычного проекта CMake
## Требования
- OS: Windows x64
- Компилятор: GCC
- CMake 3.28+
- vcpkg для управления зависимостями

## Зависимости
Установка vcpkg:
```bash
git clone https://github.com/microsoft/vcpkg.git
```
Перейдите в директорию vcpkg и выполните скрипт установки
```bash
cd vcpkg
.\bootstrap-vcpkg.bat # Для Windows
```
# Структура проекта
```
MyDepTree/
    ├── DepTrer                    # Классы, представляющие дерево зависимости пакетов
        ├── DepTree.cpp            # и его узлов
        ├── DepTree.h
        ├── PkgNode.cpp
        └── PkgNode.h
    ├── PlantUML
        └──  start.bat             # Скрипт для запуска утилиты plantuml.jar     
    ├── test
        ├── PlatUmlDepTest.cpp
    ├── utils
        ├── AdditionalFileLoader.h # Загрузка PlantUml и JDK
        ├── Indicators.h           # Индикатор активности и прогрессбар
        ├── Indicators.cpp
        ├── InternetWorker.cpp
        ├── InternetWorker.h
        ├── JsonManager.cpp
        ├── JsonManager.h
        └── ZipWorker.h            # Распаковка скачанных архивов
    ├── CMakeLists.txt
    ├── appsettings.json
    └── main.cpp
```

Структура файла конфигураций ```appsettings.json```, предназначенного для выбора ресурсов для скачивания нужных файлов.

```json
{
  "URL_JDK": "https://download.java.net/java/GA/jdk23.0.1/c28985cbf10d4e648e4004050f8781aa/11/GPL/openjdk-23.0.1_windows-x64_bin.zip",
  "URL_PLANTUML": "https://github.com/plantuml/plantuml/releases/download/v1.2024.7/plantuml-mit-1.2024.7.jar"
}
```

# Пример использования

```bash
MyDepTree <pkg_dir> <dep>
<pkg_dir> - путь к директории python пакета
<dep>     - глубина анализа зависимостей
```
В случае отсутствия необходимых файлов (plantuml.jar и jdk), приложение начнет автоматическое их скачивание. После чего, проанализирует зависимости пакета.
Само изображение дерева будет сохранено в директорию ```./PlantUml/```


Вывод приложения:
![doc/img/s1](https://github.com/VSheenko/MyDepTree/blob/master/doc/img/s1.png)
Изображение дерева:
![doc/img/s2](https://github.com/VSheenko/MyDepTree/blob/master/doc/img/s2.png)
# Тесты
![doc/img/s3](https://github.com/VSheenko/MyDepTree/blob/master/doc/img/s3.png)




