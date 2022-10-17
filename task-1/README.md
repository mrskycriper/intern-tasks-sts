# Задание
Разработать модуль ядра Linux, который реализует драйвер символьного устройства my_dummy_device. При подключении и отключении устройства драйвер должен выводить сообщения в dmesg. Разработать модуль ядра устройства, который при загрузке регистрирует my_dummy_device и при выгрузке удаляет его. Драйвера должны быть совместимы с ядром ОС Ubuntu 18.04
# Установка
1. Собрать модуль из исходного кода командой `make`
2. Установить модуль командой `sudo insmod my_dummy_device.ko`
# Удаление
1. Отключить модуль командой `sudo rmmod my_dummy_device`
# Использование
После подключения модуля в псевдофайловой системе `/dev` появится устройство `my_dummy_device`. При обращении к устройству мы будем получать бесконечный поток символов из строки "Uninteresting generic peace of text.\n"
# dmesg
1. После подключения модуля в журнале появятся записи:
  - my_dummy_device module loaded with major device number <major_number>
  - /dev/my_dummy_device created
2. В случае появления ошибок при подключении модуля могут быть получены предупреждения:
  - Could not register device on major number <major_number>
  - class_create() failed: <PTR_ERR>
  - device_create() failed: <PTR_ERR>
3. При отключении модуля в журняле появятся записи:
  - /dev/my_dummy_device removed
  - my_dummy_device module unloaded
# Тестирование
В приложеном Makefile имеется test позволяющий проверить появление сообщений в системном журнале. Для запуска теста достаточно выполнить команду `make test`.
