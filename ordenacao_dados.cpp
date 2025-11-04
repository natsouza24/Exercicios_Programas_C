#include <stdio.h>
#include <stdlib.h>

// ALGORITMOS DE ORDENAÇÃO

// Bubble Sort
void bubbleSort(int *v, int n) {
    int i, j, aux;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (v[j] > v[j + 1]) {
                aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
}

// Insertion Sor
void insertionSort(int *v, int n) {
    int i, j, chave;
    for (i = 1; i < n; i++) {
        chave = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// Selection Sort
void selectionSort(int *v, int n) {
    int i, j, min, aux;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (v[j] < v[min]) min = j;
        aux = v[i];
        v[i] = v[min];
        v[min] = aux;
    }
}

// Merge Sort
void merge(int *v, int inicio, int meio, int fim) {
    int i = inicio, j = meio + 1, k = 0;
    int *aux = (int*) malloc((fim - inicio + 1) * sizeof(int));
    while (i <= meio && j <= fim) {
        if (v[i] <= v[j]) aux[k++] = v[i++];
        else aux[k++] = v[j++];
    }
    while (i <= meio) aux[k++] = v[i++];
    while (j <= fim) aux[k++] = v[j++];
    for (i = inicio, k = 0; i <= fim; i++, k++) v[i] = aux[k];
    free(aux);
}

void mergeSort(int *v, int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSort(v, inicio, meio);
        mergeSort(v, meio + 1, fim);
        merge(v, inicio, meio, fim);
    }
}

// Quick Sort
void quickSort(int *v, int inicio, int fim) {
    int i = inicio, j = fim, pivo = v[(inicio + fim) / 2], aux;
    while (i <= j) {
        while (v[i] < pivo) i++;
        while (v[j] > pivo) j--;
        if (i <= j) {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
            i++;
            j--;
        }
    }
    if (inicio < j) quickSort(v, inicio, j);
    if (i < fim) quickSort(v, i, fim);
}


// FUNÇÕES DE ARQUIVO

// Criar Arquivo
void criarArquivo(const char *nome) {
    FILE *fp = fopen(nome, "w");
    if (!fp) {
        printf("Erro ao criar o arquivo!\n");
        return;
    }

    int n, valor;
    printf("Quantos numeros deseja gravar? ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Digite o numero %d: ", i + 1);
        scanf("%d", &valor);
        fprintf(fp, "%d\n", valor);
    }

    fclose(fp);
    printf("Arquivo '%s' criado com sucesso!\n", nome);
}

// Ler Arquivo
int lerArquivo(const char *nome, int *v) {
    FILE *fp = fopen(nome, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo '%s'!\n", nome);
        return 0;
    }

    int i = 0;
    while (fscanf(fp, "%d", &v[i]) != EOF) i++;
    fclose(fp);
    return i;
}

// Salvar Arquivo
void salvarArquivo(const char *nome, int *v, int n) {
    FILE *fp = fopen(nome, "w");
    if (!fp) {
        printf("Erro ao abrir o arquivo '%s'!\n", nome);
        return;
    }

    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", v[i]);

    fclose(fp);
    printf("Arquivo '%s' salvo com sucesso!\n", nome);
}

// Exibir Arquivo
void exibirArquivo(const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo '%s'!\n", nome);
        return;
    }

    int valor;
    printf("\nConteudo do arquivo '%s':\n", nome);
    while (fscanf(fp, "%d", &valor) != EOF)
        printf("%d ", valor);
    printf("\n");
    fclose(fp);
}

// Alterar Arquivo
void alterarValor(const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (!fp) {
        printf("Arquivo não encontrado!\n");
        return;
    }

    int v[100], n = 0;
    while (fscanf(fp, "%d", &v[n]) != EOF) n++;
    fclose(fp);

    int pos, novo;
    printf("Qual posicao (1 a %d) deseja alterar? ", n);
    scanf("%d", &pos);
    if (pos < 1 || pos > n) {
        printf("Posição invalida!\n");
        return;
    }

    printf("Digite o novo valor: ");
    scanf("%d", &novo);
    v[pos - 1] = novo;

    salvarArquivo(nome, v, n);
    printf("Valor alterado e arquivo atualizado!\n");
}


// MENU PRINCIPAL
int main() {
    int opcao, n, v[100];
    const char *arquivoEntrada = "dados.txt";
    const char *arquivoOrdenado = "ordenado.txt";

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Criar arquivo com dados desordenados\n");
        printf("2 - Ler dados do arquivo\n");
        printf("3 - Ordenar dados\n");
        printf("4 - Alterar valor no arquivo e reordenar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                criarArquivo(arquivoEntrada);
                break;

            case 2:
                exibirArquivo(arquivoEntrada);
                break;

            case 3: {
                n = lerArquivo(arquivoEntrada, v);
                if (n == 0) break;

                int metodo;
                printf("\nEscolha o metodo de ordenacao:\n");
                printf("1 - Bubble Sort\n2 - Insertion Sort\n3 - Selection Sort\n4 - Merge Sort\n5 - Quick Sort\n");
                printf("Opcao: ");
                scanf("%d", &metodo);

                switch (metodo) {
                    case 1: bubbleSort(v, n); break;
                    case 2: insertionSort(v, n); break;
                    case 3: selectionSort(v, n); break;
                    case 4: mergeSort(v, 0, n - 1); break;
                    case 5: quickSort(v, 0, n - 1); break;
                    default: printf("Metodo invalido!\n"); continue;
                }

                salvarArquivo(arquivoOrdenado, v, n);
                printf("\nDados ordenados:\n");
                for (int i = 0; i < n; i++) printf("%d ", v[i]);
                printf("\n");
                break;
            }

            case 4:
                alterarValor(arquivoEntrada);
                printf("Reordenando novamente...\n");
                n = lerArquivo(arquivoEntrada, v);
                quickSort(v, 0, n - 1);
                salvarArquivo(arquivoOrdenado, v, n);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
