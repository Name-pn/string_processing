#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define SEP '\0'
#define ALPHABET 255

//Нативный алгоритм поиска образа subs в строке s фиксированной асимтотики О(n*m), и записи вхождений образа в массив a. Возвращает число вхождений образа
int native_pattern(const char * const s, const char * const subs, int * const a);

//Оптимальный нативный алгоритм поиска образа subs в строке s, и записи вхождений образа в массив a. Возвращает число вхождений образа
int optim_pattern(const char * const s, const char * const subs, int * const a);

//z-функция для поиска подстроки subs в s. Вхождения записываются в b
int zf_pattern(const char * const s, const char * const subs, int * const a);

//Алгоритм Рабина-Карпа с модулем q = 2^64 и p = 213. s строка, subs искомый в s образ, a массив с позиями вхождения. Возвращает число вхождений.
int fast_rk(const char * const s, const char * const subs, int * const a);

//Классический алгоритм Рабина-Карпа с изменяющимся модулем q = 2^64 и p = 2. s строка, subs искомый в s образ, a массив с позиями вхождения. Возвращает число вхождений.
int classic_rk(const char * const s, const char * const subs, int * const a);

//Алгоритм КМП с улучшенной префикс функцией. Находит вхождения образа subs в s и записывает позиции вхождения в массив b. Возвращает число вхождений
int smart_kmp(const char * const s, const char * const subs, int * const a);

//Алгоритм Бойера-Мура-Хорспула. Возвращает число вхождений образа subs в s и записывает их в массив a
int basic_bmh(const char * const s, const char * const subs, int * const b);

//Банальный алгоритм определения количества палиндромов в строке s сложности О(n^3)
long long native_palindroms(const char * const s);

//Улучшенный банальный алгоритм поиска палиндромов в строке s сложности О(n^2)
long long fast_native_pal(const char * const s);

//Хеш алгоритм определения числа палиндромов в строке сложности О(n*log (n))
long long hash_pal(const char * const s);

//Алгоритм Манакера для нахождения количества палиндромов в строке сложности О(n)
long long manacker(const char * const s);
