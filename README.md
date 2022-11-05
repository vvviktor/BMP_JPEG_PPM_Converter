# BMP_JPEG_PPM_Converter

**Программа преобразования формата изображения.**

* Программа считывает изображение форматов JPEG, BMP, PPM из файла и сохраняет его в файле любого из этих
  форматов.
* Использование `./imgconv <исходный файл> <результирующий файл>`
* Для сборки проекта нужно использовать скрипт CMakeLists.txt из папки ImgConverter. В файле CMakeLists.txt
  находящемся в папке ImgLib необходимо указать актуальный путь к папке libjpeg из комплекта сборки.
* Возможна доработка интерфейса и функционала с добавлением возможности применения фильтров и операций поворота,
  отражения изображения.
