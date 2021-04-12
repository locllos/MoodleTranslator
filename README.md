# Оптимизация хеш-таблицы
Суть данного проекта заключалась в оптимизации такой структуры данных, как хеш-таблица. Здесь хеш-таблица использовалась для поиска переводов слов. Также был реализован "дословный переводчик", суть которого заключалась отображении перевода слова с английского на русский, при наведении курсора мышки на его английский вариант.

Пример:

![](https://i.imgur.com/IosgJ2t.png)

## Формальности окончены - вперёд оптимизировать
### Мне не приходят на ум алгоритмы, но хочу, чтобы программа работала быстрее
Я написал свой дословный переводчик, но вот незадача, он слишком медленный, при этом алгоритма умнее с более хорошей асимпотикой не придумаешь, тогда на помощь мне приходят ***ассемблерные вставочки!*** 
### С чего начать
Для начала определим, какая из функций работает медленее всего. С этой целью мы воспользуемся ***valgrind***, а если быть более точным, то инструментов valgrind'a - ***callgrind***. 

#### Но перед этим...
Но перед этим посмотрим, сколько времени работает программа без оптимзаций.
Для тестирования мы много (порядка 2 миллионов) раз проводили запрос типа Find для элементов хеш-таблицы.

Время: ***6.93 секунд***

### Запустили - молодцы
Итак, вот какие функции работают дольше всего:

![](https://i.imgur.com/5LpSiMd.png)

*уточним: HolyPoly функция подсчёта полиномиального хеша

### Какие выводы можно сделать с картиночки
Вперёд оптимизировать HolyPoly и strcasecmp!

### Пора приступать к оптимизации
#### Начнём с функции HolyPoly
Напишем ассемблерный аналог HolyPoly и слинкуем её с нашей прогой

![](https://i.imgur.com/HVZp55T.png)


#### Ну и каков результат

Результат не может не радовать, ведь благодаря тому, что мы написали функцию подсчёта хеша на ассемблере, время работы нашей функции уменьшилось: стало ***6.5 сек***
При этом доля работы этой функции значительно уменьшилась, вместо почти 25% она стала занимать лишь 11.6%

![](https://i.imgur.com/d9cueHJ.png)



### Пойдём оптимизировать strcasecmp

#### Ну и чего мы тут понаписали
А мы решили взять и переписать строки на avx2. И благодаря этому у нас вместо кучи сравнений по одному символы, всё будем происходить за один раз.

![](https://i.imgur.com/0yOm3Sh.png)


#### Ну и что? Ускорило что ли
Да, ускорило.
При этом достаточно значительно: теперь тест проходит за: ***5.36 секунд***


### До и после
Подытожим и посмотрим на сколько быстрее стало выполняться суммарно
* До оптимизации: ~6.93c
* После оптимизации: ~5.36c

Общий прирост: **24%**

### Итог
Добавим для сравнения нашу неоптимизированную программу, скомпилированную с флагом -О3. Она отработала за 6.3 секунды, что медленее нашей оптимизации, использующую AVX инструкции. Наша реализация быстрее -О3 на 15%.

Покажем результаты на графике.
* **Not Optimized** - изначальная версия программы
* **Asm** - версия, где мы переписали функцию подсчёта хеша в ассемблер
* **-O3** - версия, скомилированная с флагом О3
* **Asm + Avx** - версия, c переписанной на ассемблер функцией и с добавлением AVX инструкций
![](https://i.imgur.com/hcMS8tk.png)

Для наглядности отобразим график прироста производительности, то есть на сколько у нас увеличивалась скорость работы программы:
* "Not optimize -> asm" - прирост при переходе от неоптимизированной к версии, с ассебмлерной вставкой
* "asm -> asm + avx" - прирост при добавлении AVX инструкций
* "-O3 -> asm + avx" - прирост относительно -O3
![](https://i.imgur.com/eNpZgDS.png)

Как мы видим, даже -О3 не смог обогнать наши AVX'ики!

Проведя дорогостоящие исследования, можно подытожить и уверенно сказать: ускорили на 24% процента.