#include <stdio.h>
#include <string.h>

void inverte_string(char *str) {
	int len = strlen(str);
	char *inicio = str;
	char *fim = str + len - 1;
	while (inicio < fim) {
		char temp = *inicio;
		*inicio = *fim;
		*fim = temp;
		inicio++;
		fim--;
	}
}

int main() {
	char palavra[] = "casa";
	printf("Antes: %s\n", palavra);
	inverte_string(palavra);
	printf("Depois: %s\n", palavra);
	return 0;
}
