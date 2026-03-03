#include <stdio.h>

int soma_vetor(int *v, int n) {
	int soma = 0;
	int *ptr = v;
	for (int i = 0; i < n; i++) {
		soma += *(ptr + i);
	}
	return soma;
}

int main() {
	int numeros[] = {8, 9, 1, 2, 6};
	int total = soma_vetor(numeros, 5);
	printf("Soma = %d\n", total);
	return 0;
}
