# Language

## Содержание
- [Аннотация](#аннотация)
- [Frontend](#Frontend)
- [Midlend](#Midlend)
- [Backend](#Backend)
- [Используемые файлы](#Используемые-файлы)
- [Синтаксис](#Синтаксис)
- [Программы](#Программы)
- [Как собирать](#Как-собирать)

---

## Аннотация

***Мой футбольный язык программирования*** (Language) - программа, которая содержит в себе 3 подпрограммы: *Frontend*, *Midlend*, *Backend*.
Выполняяет простейшие программы.

Рассмотрим поподробнее каждую из программ.

## Frontend
- Данная подпрограмма считавыет данные из произвольного текстого файла, в котором содежится программа, написанная на моем языке, и с помощью токенизатора создает ликсемы, в которых содержатся числа, переменные, стандартные функции, оператооры, ключевые слова и условные операторы.
- Далее с помощью рекурсивного спуска (алгоритм синтсаксического анализа) происходит проверка текста программы на синтаксические ошибки, создаюся узлы для дерева, в которых хранится информация: тип элемента, его стандартное условное обозначение и свзязь его с потомками.
- Также вo *Frontend* включена программа *Build_graphviz*, которая сроит дерево в и записывет его узлы и свзяи в файл с расширением *.dot*


## Midlend
- Данная подпрограмма упрощает созданное в *Frontend* дерево с помощью двух фунций *simplifier_conv_of_const* (светка костант), которая считает поддеревья, которые содержат лишь числа и математические оперции, и функция *remove_neutral_elements* (удаление нейтральных элементов), удаляет ненужные поддеревья, которые содержат уиножение на 0, прибавление 0, умножение на 1.
- Программа в случае сохранаения поддерева копирует его.
- После выполнения всех действий дерево печтатеся в скобочном формате в файл *file_output.txt*, в котором хранится информация о каждом узле:
    - его тип
    - его условное обозначение

**Пример:**
```c
  { #2# #*# { #0# #2# }{ #1# #Messi# }}
```

**Таблица типов данных в дереве программы:**
```c
  enum Class_type
  {
      T_NUM     =  0,   // 234, -15
      T_VAR     =  1,   // Messi, x
      T_OP      =  2,   // +, /, ^
      T_OP_LONG =  3,   // sin, sqrt, exp
      T_KEY_W   =  4,   // =, ;
      T_CBR_O   =  5,   // {
      T_CBR_C   =  6,   // }
      T_FUNC    =  7,   // in, out
      T_IF_     =  8,   // if
      T_SIGN    =  9,   // ==, >=
      T_WHILE   =  10,  // while
      DEFUALT   = -1,   // null
  };
```

## Backend
- Данная подпрограмма состоит из трех блоков:
    - *input_output.cpp*
    - *translator.cpp*
    - *processor*
- Подпрограмма *input_output.cpp* считывает инфориацию из файла *file_output.txt* и создает по нему дерево. Алгоритм реализован с помощью стека (вместо рекурсии).
Созданное дерево схоже с даревом из *Frontend*
- Подпрограмма *translator* на основе созданного дерева создает ассемблерный код. Алгоритм основан на рекурсии.
    - если встречатся число, то создается команда *push число*
    - если встречатся переменная, то происходит проверка на ее инициализацию а затем печатаеся команда *push [ переменная ]*
    - если это услованя оператор, то снчала обходться поддерево с условием, затем поддерево с телом. для проверки условия используются условные джампы, например *jbe* (<=), *je* (==), *ja* (>) и тд.
    - если встречатся оператор, то печатася его ассемблерная команда, например *add*, *sqrt* и тд.
- Программа *processor* состоит из двух блоков: *Assembler* и *СPU*
    - *Assembler* считывает передавеаемый ему ассемблерный код и создает байткод
    - *CPU* cчитывает созданный ассемблером байткод и декодирует команды и последовательно выпоняет их или пропускает некоторые в случае условного джампа.

## Используемые файлы:
- *Frontend*
    - src
        - *input_output.cpp*  // содержаться функции распечатки дерева
        - *main.cpp*
        - *pacer.cpp*         // токенизатор и алгоритм синтаксического анализа
    - include
        - *parcer.h*
    - library
        - *commands.h*
        - *library_of_defines.h* // библиотека макросов
        - *errors.h*
- *Midlend*
    - src
        - *simplifier.cpp*       // 2 функции упрощения дерева
    - include
        - *simplifier.h*
- *Backend*
    - src
        - *input_iutput.cpp*     // функции считывания дерева и его печати
        - *main.cpp*
        - *translator.cpp*       // функция создания ассемблерного кода
    - include
        - *backend.h*
    - library
        - *commands.h*
        - *library_of_defines.h*
        - *error.h*
    - processor
        - *Stack*
        - Assembler
            - src:     *assembler.cpp*
            - include: *assembler.h*
        - CPU
            - src:     *CPU.h*
            - include: *CPU.h*
        - Library
            - *DSL.h*
            - *commands.h*

## Синтаксис

***Условия***
- алгоритм синтаксического анализа

```c

  G         ::= equation '\0'
  equation  ::= assign ( ';'|'}' assign )++
  assign    ::= var /*переменная*/ '=' E | var /*функция*/ '(' var')' | var /*условный оператор*/ condition body
  condition ::= '(' E 'SIGN' E ')'
  body      ::= '{' equation '}'
  E         ::= T ( '+'|'-' T )++
  T         ::= P ( '*'|'/' P )++
  P         ::= '(' E ')' | '(' E ')' '^' pow | /*op_long|var*/ F | N
  pow       ::= '(' E ')'
  F         ::= 'op_long' P | 'variable'
  var       ::= 'variable' | 'if' | 'while' | 'func' /*in|out*/
  N         ::= 'num'

```

***Футбольный Синтаксис***

```c

  VAR           //  if
  Extra_time    //  while
  match_start   //  {
  match_end     //  }
  score         //  =
  Kick_off      //  in()  - функция ввода с клавиитауры
  Change        //  out() - фцнкция вывода

```

Знаки сравнения
```c

  red_cards     //  <
  yellow_cards  //  >
  assists       //  ==
  goals         //  <=
  ball_touch    //  >=
  loose         //  !=

```

## Программы

- Для добавления новой программы, написанной на моем языке, стоит сначала изучить синтаксис, затем создать текстовый файл в папке *text_files* и добавить путь до этого файла в *Makefile* в переменную *program*.

### Пример программы **equation_solver**

```c
  Kick_off (Messi);
  Kick_off (Neymar);
  Kick_off (Kante);

  anyNum score 888;
  noRoots score -111;

  VAR (Messi assists 0)   // a == 0
  match_start
      VAR (Kante assists 0)  // c == 0
      match_start
          VAR (Neymar assists 0)  // b == 0
          match_start
              Golovin score anyNum;
          match_end

          VAR (Neymar loose 0)  // b != 0
          match_start
              Golovin score 0;
          match_end
      match_end

      VAR (Kante loose 0)  // c != 0
      match_start
          Golovin score 0 - Kante / Neymar;
      match_end

      Change (Golovin);
  match_end

  VAR (Messi loose 0)  // a != 0
  match_start
      Dybala score Neymar * Neymar - 4 * Messi * Kante;  // d = b*b - 4ac

      VAR (Dybala assists 0)  // d == 0
      match_start
          Golovin score 0 - Neymar / (2 * Messi);
          Change (Golovin);
      match_end

      VAR (Dybala red_cards 0)  // d < 0
      match_start
          Golovin score noRoots;
          Change (Golovin);
      match_end

      VAR (Dybala yellow_cards 0)  // d > 0
      match_start
          Harry_Kane score sqrt(Dybala);
          Golovin score (Harry_Kane - Neymar) / 2;
          Mbape score 0 - (Harry_Kane + Neymar) / 2;
          Change (Golovin);
          Change (Mbape);
      match_end
  match_end

```

### Пример программы **factorial**

```c
  Kick_off (Neymar);  // ввод с клавиатуры

  undef score -111;
  Salah score 1;

  VAR (Neymar goals 1)  // если номер меньше 1
  match_start

      VAR (Neymar red_cards 0)  // если номер отрицательный
      match_start
          Salah score undef;
          Change (Salah);
      match_end

      VAR (Neymar ball_touch 0) // если номер 1 или 0
      match_start
          Salah score 1;
          Change (Salah);
      match_end

  match_end

  VAR (Neymar yellow_cards 1)
  match_start

      Extra_time (Neymar yellow_cards 0)  // цикл while
      match_start
          Salah score Salah * Neymar;
          Neymar score Neymar - 1;
      match_end

      Change (Salah);

  match_end

```
## Как собирать

- Сборка осуществляется с помощью *Makefile*

```c

  CC = g++
  program = text_files\factorial.txt  // относительный путь до программы

  all: frontend backend cpu asm
  	 Frontend\frontend.exe $(program)
  	 Backend\backend.exe text_files\file_output.txt text_files\asm_file.txt
  	 Backend\processor\Assembler\assembler.exe  text_files\asm_file.txt  Backend\processor\Assembler\res_ass.txt
  	 Backend\processor\CPU\cpu.exe  Backend\processor\Assembler\res_ass.txt  text_files\result_file.txt

  // Frontend
  front: frontend
  	 Frontend\frontend.exe $(program)

  frontend: Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp
  	 $(CC) -o Frontend\frontend Frontend\src\main.cpp Frontend\src\parcer.cpp Frontend\src\input_output.cpp Frontend\src\simplifier.cpp

  // Backend
  back: backend
  	 Backend\backend.exe text_files\file_output.txt text_files\asm_file.txt

  backend: Backend\src\main.cpp Backend\src\input_output.cpp Backend\stack\src\stack.cpp Backend\src\translator.cpp
  	 $(CC) -o Backend\backend Backend\src\main.cpp Backend\src\input_output.cpp Backend\stack\src\stack.cpp Backend\src\translator.cpp

  proc: asm cpu
  	 Backend\processor\Assembler\assembler.exe  text_files\asm_file.txt  Backend\processor\Assembler\res_ass.txt
  	 Backend\processor\CPU\cpu.exe  Backend\processor\Assembler\res_ass.txt  text_files\result_file.txt

```

