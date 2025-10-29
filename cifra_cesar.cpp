#include <stdio.h>

int main() {
    char normal[100];
    char criptografado[100];
    int i;
    int chave = 3; // Deslocamento fixo da Cifra de César

    printf("Digite uma palavra: ");
    fgets(normal, sizeof(normal), stdin);

    for (i = 0; normal[i] != '\0'; i++) {
        char c = normal[i];

        if (c >= 'a' && c <= 'z') {  // Se for letra minúscula
            criptografado[i] = ((c - 'a' + chave) % 26) + 'a';
        }
        else if (c >= 'A' && c <= 'Z') { // Se for letra maiúscula
            criptografado[i] = ((c - 'A' + chave) % 26) + 'A';
        }
        else {
            criptografado[i] = c;
        }
    }

    criptografado[i] = '\0'; // Final da string

    printf("\nPalavra original: %s", normal);
    printf("Palavra criptografada: %s\n", criptografado);

    return 0;
}
