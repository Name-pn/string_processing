#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "alg.h"
#define STRING_SIZE 100000035
#define PAT_RES
#define STANDART_STREAM "stdin"
#undef PA //Вывод результатов поиска образа 

//Вывод массива a размера n
void write_arr(int* a, size_t k) {
	int* ps = a;
	for (size_t i = 0; i < k; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");
}

//Ввод строки s и образа subs из потока stream
void read_pattern(char* s, char* subs, FILE* stream) {
	fgets(s, 1000010000, stream);
	fgets(subs, 1000002, stream);
	char* ps = subs; 
	while (*ps != '\n' && *ps != '\0')
		++ps;
	*ps = '\0';
}

//Решение задачи поиска образа subs в строке s разными алгоритмами, и вывод времени работы каждого из них
void test_pattern(char* s, char* subs) {
	int* a = (int*)calloc(100000003, sizeof(int));
	clock_t begin = clock();
	int k1 = native_pattern(s, subs, a);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение нативного О(n*m): %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k1);
#endif

	begin = clock();
	int k = optim_pattern(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа\n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение ускоренного нативного: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif
	begin = clock();
	k = zf_pattern(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа \n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение z-функции для поиска образа: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif	

	begin = clock();
	k = fast_rk(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа \n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение ускоренного алгоритма Рабина-Карпа для поиска образа: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif

	begin = clock();
	k = classic_rk(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа \n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение классического алгоритма Рабина-Карпа для поиска образа: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif
	begin = clock();
	k = smart_kmp(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа \n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение алгоритма КМП для поиска образа: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif

	begin = clock();
	k = basic_bmh(s, subs, a);
	end = clock();
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций поиска образа \n");
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение алгоритма Бойера-Мура-Хорспула для поиска образа: %lf\n", time_spent);
#ifdef PAT_RES
	write_arr(a, k);
#endif
	free(a);
}

//Решение задачи определения количества палиндромов в строке s разными способами алгоритмами и вывод времени работы каждого
void test_palindroms(char* s) {
	clock_t begin = clock();
	int k1 = 0;//native_palindroms(s);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Время на выполнение нативного О(n^3): %lf\n", time_spent);
	printf("%i\n", k1);

	begin = clock();
	int k = fast_native_pal(s);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций определения количества палиндромов\n");
	printf("Время на выполнение ускоренного нативного О(n^2): %lf\n", time_spent);
	printf("%i\n", k);

	begin = clock();
	k = hash_pal(s);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций определения количества палиндромов (Хеш алгоритм)\n");
	printf("Время на выполнение хеш-алгоритма О(n*log n): %lf\n", time_spent);
	printf("%i\n", k);

	begin = clock();
	k = manacker(s);
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	if (k != k1)
		fprintf(stderr, "Обнаружено расхождение значений функций определения количества палиндромов\n");
	printf("Время на выполнение алгоритма Манакера: %lf\n", time_spent);
	printf("%i\n", k);
}

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	printf("Какую задачу будем тестировать? Палиндромы 0, образ 1, изменение потока 2, выход 3\n");
	int f;
	scanf("%i", &f);
	getchar();
	FILE* stream = stdin;
	char f_name[255];
	char* s = (char*)calloc(STRING_SIZE, 1);
	char* subs = (char*)calloc(10001, 1);
	while (f != 3) {
		switch (f) {
		case 0:
			fgets(s, STRING_SIZE, stream);
			test_palindroms(s);

			if (stream != stdin)
				fseek(stream, 0, SEEK_SET);
			break;
		case 1:
			read_pattern(s, subs, stream);
			test_pattern(s, subs);
			if (stream != stdin)
				fseek(stream, 0, SEEK_SET);
			break;
		case 2:
			printf("Введите имя файла для обработки\n");
			scanf("%s", f_name);
			if (f_name[0] == 's' && f_name[1] == 't' && f_name[2] == 'd' && f_name[3] == 'i' && f_name[4] == 'n') {
				stream = stdin;
			}
			stream = fopen(f_name, "r");
			if (stream == NULL)
				fprintf(stderr, "Файл не найден\n");
		}
		scanf("%i", &f);
		getchar();
	}
	free(s);
	free(subs);
	fclose(stream);
	return(0);
}