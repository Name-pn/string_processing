#include "alg.h"

//Возвращает длину строки s
static int strlen(char* s) {
	char* ps = s;
	while (*s != SEP)
		++s;
	return(s - ps);
}

int native_pattern(char* s, char* subs, int* a) {
	int i = 0;
	int k = 0;
	int n = strlen(subs);
	int ns = strlen(s) - n;
	while (i <= ns) {
		size_t j;
		int f = 1;
		for (j = 0; j < n; j++) {
			if (subs[j] != s[i + j])
				f = 0;
		}
		if (f) {
			a[k] = i;
			++k;
		}
		++i;
	}
	return (k);
}

int optim_pattern(char* s, char* subs, int* a) {
	int i = 0;
	int k = 0;
	int n = strlen(subs);
	int ns = strlen(s) - n;
	while (i <= ns) {
		int j = 0;
		while (s[i + j] == subs[j] && subs[j] != SEP) {
			++j;
		}
		if (j == n) {
			a[k] = i;
			++k;
		}
		++i;
	}
	return (k);
}


//Возвращает число символов в строке s совпадающих с i1 и i2 позиций
static int cmp(char* s, int i1, int i2) {
	int r = 0;
	char* pi1, * pi2;
	pi1 = &(s[i1]);
	pi2 = &(s[i2]);
	while (*pi1 == *pi2 && *pi1 != 0 && *pi2 != 0) {
		++pi1;
		++pi2;
		++r;
	}
	return(r);
}

//Записывает в a z-функцию строки s
static void block(char* s, int* a) {
	int n = strlen(s);
	int l = -1;
	int r = -1;
	a[0] = 0;
	for (int i = 1; i < n; i++) {
		if (r < i) {
			a[i] = cmp(s, i, 0);
			if (a[i] > 0) {
				r = a[i] + i - 1;
				l = i;
			}
		}
		else {
			int k = i - l;
			if (a[k] < r - i + 1) {
				a[i] = a[k];
			}
			else {
				int t = cmp(s, r + 1 - i, r + 1);
				a[i] = r - i + 1;
				l = i;
				if (t > 0) {
					a[i] += t;
					r = i + a[i] - 1;
				}
			}
		}
	}
}

//Специальная конкатенация строк s1 и s2
static void strcat(char* s1, char* s2) {
	while (*s1 != SEP)
		++s1;
	while (*s2 != '\0') {
		*s1 = *s2;
		++s1;
		++s2;
	}
	*s1 = *s2;
}

int zf_pattern(char* s, char* subs, int* b) {
	char* buf = (char*)calloc(100000100, 1);
	int ns = strlen(subs);
	char ts[3] = "•\n";
	strcat(buf, subs);
	strcat(buf, ts);
	strcat(buf, s);
	int n = strlen(buf);
	int* a = (int*)calloc(n + 1, sizeof(int));
	block(buf, a);
	int k = 0;
	for (size_t i = ns; i < n; i++) {
		int t = a[i];
		if (a[i] == ns) {
			b[k] = i - ns - 2;
			++k;
		}
	}
	free(a);
	free(buf);
	return(k);
}

//Возвращает хешь подстроки строки s с началом в i1, концом в i2 - 1 по модулю 2^64
static long long fhash(char* s, long long i1, long long i2) {
	unsigned long long r = (unsigned char)s[i1];
	for (long long i = i1 + 1; i <= i2; ++i) {
		r = (r * 211);
		r = (r + (unsigned char)s[i]);
	}
	return(r);
}

int fast_rk(char* s, char* subs, int* a) {
	long long nsubs = strlen(subs);
	long long isubs = fhash(subs, 0, nsubs - 1);
	unsigned long long is = fhash(s, 0, nsubs - 1);
	int k = 0;
	unsigned long long deg = 1;
	for (long long i = 1; i <= nsubs; i++) {
		deg = (deg * 211);
	}
	if (is == isubs) {
		a[k] = 0;
		++k;
	}
	long long i = nsubs;
	while (s[i] != SEP) {
		is = ((is * 211) - deg * ((unsigned char)s[i - nsubs]) + (unsigned char)s[i]);
		if (is == isubs) {
			a[k] = i - nsubs + 1;
			++k;
		}
		i++;
	}
	return(k);
}

//Возвращает i наиболее способствующее быстродействию определения позиций подстрок subs в строке s
static long long find_i(char* s, char* subs) {
	long long ns = strlen(s);
	long long nsubs = strlen(subs);
	long long i = (long long)ns * ns * nsubs * 64;
	if (i < 0 || i >= 1 << 54) {
		long long max = ~(unsigned long long)0 >> 10;
		if (i <= max && i > 0)
			printf("Ожидаемая вероятность ложного совпадения простых чисел %f\n", 2.53 / ns);

		i = max;
	}
	return((long long)i);
}

//Если i простое число возвращает 1, иначе 0
static long long is_prime(long long i) {
	long long sqrt_t = (long long)sqrt(i);
	if (i < 10)
		for (long long j = 2; j <= sqrt_t; ++j) {
			if (i % j == 0) {
				return(0);
			}
		}
		else
			for (long long j = 3; j <= sqrt_t; j += 2) {
				if (i % j == 0)
					return(0);
			}
	return(1);
}

//Возвращает модуль для алгоритма Рабина-Карпа
static long long get_module(char* s, char* subs) {
	long long i = find_i(s, subs);
	while (!is_prime(i))
		--i;
	return(i);
}

//Возвращает хешь подстроки строки s с началом в i1, концом в i2 по модулю q
static long long chash(char* s, long long i1, long long i2, long long q) {
	unsigned long long r = (unsigned char)s[i1];
	for (long long i = i1 + 1; i <= i2; ++i) {
		r = (r << 8) % q;
		r = (r + (unsigned char)s[i]);
	}
	return(r);
}


int classic_rk(char* s, char* subs, int* a) {
	long long q = get_module(s, subs);
	long long nsubs = strlen(subs);
	long long isubs = chash(subs, 0, nsubs - 1, q);
	long long is = chash(s, 0, nsubs - 1, q);
	long long k = 0;
	long long deg = 1;
	for (long long i = 1; i <= nsubs; i++) {
		deg = (deg << 8) % q;
	}
	if (is == isubs) {
		a[k] = 0;
		++k;
	}
	long long i = nsubs;
	while (s[i] != '\0') {
		is = ((is << 8) - deg * ((unsigned char)s[i - nsubs]) + (unsigned char)s[i]) % q;
		if (is < 0)
			is += q;
		if (is == isubs) {
			a[k] = i - nsubs + 1;
			++k;
		}
		i++;
	}
	return(k);
}

//Получает массив a граней подстрок s 
static void prefix_arr(int* a, char* s) {
	a[0] = 0;
	int i = 1;
	int j = 0;
	while (s[i] != SEP) {
		if (s[i] == s[j]) {
			a[i] = j + 1;
			++i;
			++j;
		}
		else
			if (j == 0) {
				a[i] = 0;
				i++;
			}
			else
				j = a[j - 1];
	}
}

//Преобразует массив a граней подстрок s в оптимальный для КМП вид
static void prefix_up(int* a, char* s) {
	int n = strlen(s);
	for (int i = 1; i < n - 1; ++i) {
		if (s[a[i] + 1] == s[i + 1])
			a[i] = a[a[i]];
	}
}

int smart_kmp(char* s, char* subs, int* b) {
	int n = strlen(subs);
	int* a = (int*)calloc(n + 1, sizeof(int));
	int i = 0;
	prefix_arr(a, subs);
	prefix_up(a, subs);
	int k = 0;
	int l = 0;
	while (s[k] != SEP) {
		if (s[k] == subs[l]) {
			++k;
			++l;
			if (l == n) {
				b[i] = k - l;
				++i;
			}
		}
		else {
			if (l == 0) {
				++k;
			}
			else
				l = a[l - 1];
		}
	}
	free(a);
	return(i);
}

//Инициализирует массив a размера n числом x
static void ini(int* a, int n, int x) {
	for (size_t i = 0; i < n; i++) {
		a[i] = x;
	}
}

//Получает массив величин сдвигов a для БМХ алгоритма по образу subs
static void bmh_shift_arr(int* a, char* subs) {
	int n = strlen(subs);
	ini(a, ALPHABET, n);
	int i2 = n - 2;
	int i1 = 0;
	int k = 1;
	while (i2 >= i1) {
		if (a[(unsigned char)subs[i2]] == n)
			a[(unsigned char)subs[i2]] = k;
		++k;
		--i2;
	}
}

int basic_bmh(char* s, char* subs, int* b) {
	int a[ALPHABET];
	int n = strlen(subs);
	bmh_shift_arr(a, subs);
	--n;
	int ns = strlen(s);
	int i = n;
	int k = 0; //Размер массива результата
	while (i < ns) {
		int j = n;
		int i1 = 0;
		while (s[i - i1] == subs[j] && i1 <= i) {
			--j;
			++i1;
			if (j < 0) {
				b[k] = i - n;
				++k;
			}
		}
		i += a[(unsigned char)(s[i])];
	}
	return(k);
}

//Возвращает 1 если указатель p1 указывает на начало палиндрома а p2 на его конец, иначе возвращает 0
static int is_palindrom(char* p1, char* p2) {
	while (p1 <= p2) {
		if (*p1 != *p2)
			return(0);
		++p1;
		--p2;
	}
	return(1);
}

long long native_palindroms(char* s) {
	char* fp = &(s[strlen(s) - 1]);
	long long r = 0;
	for (char* left = s; left < fp; ++left) {
		for (char* right = left + 1; right <= fp; ++right) {
			if (is_palindrom(left, right))
				++r;
		}
	}
	return(r);
}

//Возвращает число палиндромов нечетной длины в s
static long long odd_count(char* s) {
	char* fp = &(s[strlen(s) - 1]);
	long long r = 0;
	for (char* middle = s; middle <= fp; ++middle) {
		char* left = middle - 1;
		char* right = middle + 1;
		while (left >= s && right <= fp && *left == *right) {
			++r;
			--left;
			++right;
		}
	}
	return(r);
}

//Возвращает число палиндромов четной длины в s
static long long even_count(char* s) {
	char* fp = &(s[strlen(s) - 1]);
	long long r = 0;
	for (char* middle = s; middle <= fp; ++middle) {
		char* left = middle;
		char* right = middle + 1;
		while (left >= s && right <= fp && *left == *right) {
			++r;
			--left;
			++right;
		}
	}
	return(r);
}

long long fast_native_pal(char* s) {
	return even_count(s) + odd_count(s);
}

//Записывает в массив a хеши префиксов строки s по основанию p
static void hash_prefix_arr(char* s, unsigned long long* a, unsigned long long p) {
	unsigned long long h = 0;
	int i = 0;
	while (*s != SEP) {
		h = h * p + *s;
		a[i++] = h;
		++s;
	}
}

//Записывает в массив a хеши суффиксов строки s по основанию p
static void hash_suffix_arr(char* s, int n, unsigned long long* a, unsigned long long p) {
	unsigned long long h = 0;
	int i = n - 1;
	char* sp = s;
	s += n - 1;
	while (sp <= s) {
		h = h * p + *s;
		a[i--] = h;
		--s;
	}
}

//Записывает в массив a степени числа p до степени n
static void hash_degree_p(unsigned long long* a, int n, unsigned long long p) {
	unsigned long long tp = 1;
	for (int i = 0; i < n; ++i) {
		a[i] = tp;
		tp *= p;
	}
}

//Возвращает значение 1, если с l по r позицию в строке лежит палиндром, для этого использует массив хешей префиксов строки a, суффиксов b, оснований c, иначе возвращает 0
static int is_pal(int l, int r, int n, unsigned long long* a, unsigned long long* b, unsigned long long* c) {
	int pal_size = r - l + 1;
	int before_pal = l - 1;
	int after_pal = r + 1;
	return(a[r] - (before_pal < 0 ? 0 : a[before_pal]) * (pal_size == n ? 0 : c[pal_size]) == b[l] - (after_pal < n ? b[after_pal] : 0) * (pal_size == n ? 0 : c[pal_size]));
}

long long hash_pal(char* s) {
	unsigned long long p = 257;
	int n = strlen(s);
	unsigned long long* a = (unsigned long long*)calloc(n, sizeof(unsigned long long));
	unsigned long long* b = (unsigned long long*)calloc(n, sizeof(unsigned long long));
	unsigned long long* c = (unsigned long long*)calloc(n, sizeof(unsigned long long));
	hash_prefix_arr(s, a, p);
	hash_suffix_arr(s, n, b, p);
	hash_degree_p(c, n, p);
	int l, r, m;
	long long t; // Для хранения числа палиндромов в позиции
	long long res = 0; // Для хранения числа палиндромов в строке
	for (int i = 0, j = n - 1; i < n; ++i, --j) {
		t = 0;
		l = 1;
		r = (i + 1 < n - i ? i + 1 : n - i);
		while (r >= l) {
			m = (l + r) / 2;
			if (is_pal(i - m + 1, i + m - 1, n, a, b, c)) {
				l = m + 1;
				if ((long long)m - 1 > t)
					t = (long long)m - 1;
			}
			else
				r = m - 1;
		}
		res += t;
		t = 0;
		l = 1;
		r = (i < n - i ? i : n - i);
		while (r >= l) {
			m = (l + r) / 2;
			if (is_pal(i - m, i + m - 1, n, a, b, c)) {
				l = m + 1;
				if (m > t)
					t = m;
			}
			else
				r = m - 1;
		}
		res += t;
	}
	free(a);
	free(b);
	free(c);
	return(res);
}

long long manacker(char* s) {
	int n = strlen(s);
	int odl = 0;
	int odr = -1;
	int el = 0;
	int er = -1;
	int* a = (int*)calloc(n, sizeof(int));
	int* b = (int*)calloc(n, sizeof(int));
	long long r = 0;
	for (int i = 0; i < n; ++i) {
		int odk = i > odr ? 1 : (a[odl + odr - i] < odr - i + 1 ? a[odl + odr - i] : odr - i + 1);
		while (i + odk < n && i - odk >= 0 && s[odk + i] == s[i - odk])
			++odk;
		a[i] = odk;
		r += odk - 1;
		if (i + odk - 1 > odr) {
			odl = i - odk + 1;
			odr = i + odk - 1;
		}
		int ek = i > er ? 0 : (b[el + er - i + 1] < er - i + 1 ? b[el + er - i + 1] : er - i + 1);
		while (i + ek < n && i - ek - 1 >= 0 && s[ek + i] == s[i - ek - 1])
			++ek;
		b[i] = ek;
		r += ek;
		if (i + ek - 1 > er) {
			el = i - ek;
			er = i + ek - 1;
		}
	}
	free(a);
	free(b);
	return(r);
}