#include <stdio.h>

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main() {
	int x = 10, y = 20;
	printf("Antes: x = %d, y = %d\n", x, y);
	swap(&x, &y);
	printf("Depois: x = %d, y = %d\n", x, y);
	return 0;
}
