# Red-Black-Tree

Реализация алгоритма структуры хранения данных в форме красно-черного бинарного дерева (Red Black Tree, RBT).

В проекте представлены исходные библиотеки с реализацией структуры данных RBT, а также драйвером, демонстрирующим работу со структурой данных.

Реализация выполнена на языке C. Запуск драйвера осуществляется make-командой из файла Makefile:

~$ make all

Проверка на ошибки памяти выполнена с помощью Valgrind-3.14.0 и LibVEX. Для запуска проверки на утечку памяти требуется выполнить make-команду из файла Makefile:

~$ make main-valgrind
