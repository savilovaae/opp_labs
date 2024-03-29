### Лабораторная работа №2
### «Параллельная реализация решения системы линейных алгебраических уравнений с помощью OpenMP»

#### Замеры времени работы программы при разном количество потоков
_**N = 5 000**_



| threads | first version (for) | second version (section) |
| :-----: | :-----------------: | :----------------------: |
|    1    |      35.306849      |        36.150880         |
|    2    |      18.093532      |        27.816642         |
|    3    |      12.711164      |        24.687546         |
|    4    |      10.191512      |        22.663366         |
|    5    |      8.704375       |        22.230295         |
|    6    |      8.016006       |        22.062122         |
|    7    |      10.150292      |        23.260686         |
|    8    |      9.349595       |        22.690038         |
|    9    |      8.802928       |        22.344414         |
|   10    |      8.030640       |        22.486817         |
|   11    |      7.695964       |        21.370126         |
|   12    |      7.446462       |        21.262252         |



export OMP_NUM_THREADS=10
gcc -fopenmp main.c schedule_program.c -o main.out -lm
./main.out 5000

|  type   |  x   |  result   |
| :-----: | :--: | :-------: |
| dynamic | 500  | 8.082779  |
| static  | 500  | 8.009835  |
| static  |  1   | 8.452317  |
| dynamic |  1   | 33.959640 |
| dynamic | 100  | 8.331790  |
| dynamic | 200  | 8.591072  |
| dynamic | 250  | 8.524620  |
| dynamic | 1000 | 13.732688 |
| dynamic | 5000 | 37.117924 |
| static  | 5000 | 35.164686 |
| static  | 1000 | 10.536714 |
| static  | 250  | 8.893593  |
| static  | 200  | 9.186259  |
| static  | 100  | 8.676748  |

##### Команда для компиляции:
```bash
gcc -fopenmp main.c sequential_program.c parallel_for_program.c parallel_section_program.c -o main.out -lm`
```
##### Команда для задания количества потоков при исполнении программы
```bash
export OMP_NUM_THREADS=12
./main.out 10000
```