# Условие
Для линейного конгруэнтного генератора чисел реализовать алгоритм вычисления последующего числа по четырем предыдущим. Считать параметр m не более 65535. Например, есть 4 числа : 157; 5054; 25789; 13214. Следующим за этими числами будет число 16605.
# Запуск
1. Собрать исполняемый файл из исходного кода командой `g++ task-3.xpp -o task-3`
2. Запустить исполняемый файл передав в качестве аргументов 4 первых числа из последовательности при помощи команды `./task-3 57 5054 25789 13214`
3. Резульатом работы программы будет список всех возможных продолжений последовательности состоящий из: коэфицентов и следующего числа в последовательности в формате `m=<param.m>\ta=<param.a>\tc=<param.c>\tnext number: <number_5>`
