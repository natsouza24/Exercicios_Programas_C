#include <stdio.h>      // I/O padrão (printf, scanf, FILE)
#include <stdlib.h>     // alocação, exit
#include <string.h>     // manipulação de strings (strcpy, strstr, strcmp)
#include <strings.h>    // para strcasecmp (comparação case-insensitive em POSIX)

#define MAX 100         // capacidade máxima do vetor de produtos

// Estrutura que representa um produto
typedef struct {
    int id;                     // identificador único do produto
    char nome[50];              // nome do produto (máx. 49 chars + '\0')
    char categoria[30];         // categoria do produto (máx. 29 chars + '\0')
    float preco;                // preço do produto
    int quantidade;             // quantidade em estoque
} Produto;


/* ---------- Funções Auxiliares ---------- */

// Salvar Arquivo: Grava todos os produtos no arquivo "produtos.txt" em formato pipe-separated
void salvarArquivo(Produto produtos[], int n) {
    FILE *f = fopen("produtos.txt", "w");     // abre (ou cria) o arquivo em modo escrita
    if (!f) {                                 // se não conseguiu abrir
        printf("Erro ao abrir arquivo para salvar.\n"); // avisa
        return;                               // retorna sem salvar
    }
    for (int i = 0; i < n; i++) {             // percorre todos os produtos
        fprintf(f, "%d|%s|%s|%.2f|%d\n",      // escreve uma linha com id|nome|categoria|preco|quantidade
                produtos[i].id, produtos[i].nome,
                produtos[i].categoria, produtos[i].preco,
                produtos[i].quantidade);
    }
    fclose(f);                                // fecha o arquivo
}

// Carregar Arquivo: Lê o arquivo "produtos.txt" e preenche o vetor produtos[], retorna quantidade lida
int carregarArquivo(Produto produtos[]) {
    FILE *f = fopen("produtos.txt", "r");     // abre o arquivo em modo leitura
    if (!f) return 0;                         // se não existe ou não abre, retorna zero (nenhum produto)
    int n = 0;                                // contador de produtos lidos
    // enquanto a leitura de uma linha no formato esperado retornar 5 campos, incrementa n
    while (fscanf(f, "%d|%49[^|]|%29[^|]|%f|%d\n",
                  &produtos[n].id, produtos[n].nome, produtos[n].categoria,
                  &produtos[n].preco, &produtos[n].quantidade) == 5) {
        n++;                                  // incrementa contador quando a linha foi lida corretamente
        if (n >= MAX) break;                  // protecção para não exceder o vetor
    }
    fclose(f);                                // fecha o arquivo
    return n;                                 // retorna a quantidade de produtos carregados
}

// Existe Nome: Verifica se já existe produto com mesmo nome (case-insensitive)
int existeNome(Produto produtos[], int n, char nome[]) {
    for (int i = 0; i < n; i++) {             // percorre o vetor
        if (strcasecmp(produtos[i].nome, nome) == 0) // compara sem diferenciar maiúscula ou minúscula
            return 1;                         // retorna 1 se existe duplicata
    }
    return 0;                                 // retorna 0 se não encontrou
}


/* Funções Principais */

// Listar: Exibe produtos em formato de tabela formatada
void listar(Produto produtos[], int n) {
    if (n == 0) {                             		// se não há produtos
        printf("\nNenhum produto cadastrado!\n"); 	// informa usuário
        return;                               		// encerra a função
    }

    // cabeçalho da tabela
    printf("\n====================================================================\n");
    printf("| %-4s | %-20s | %-15s | %-8s | %-6s |\n",
           "ID", "Nome", "Categoria", "Preco", "Qtde"); // títulos das colunas
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {             // percorre os produtos para imprimir linhas
        // imprime cada produto com alinhamento nas colunas
        printf("| %-4d | %-20s | %-15s | %-8.2f | %-6d |\n",
               produtos[i].id, produtos[i].nome, produtos[i].categoria,
               produtos[i].preco, produtos[i].quantidade);
    }

    printf("====================================================================\n"); // rodapé da tabela
}

// Inserir: Lê dados do usuário e adiciona novo produto ao vetor e salva no arquivo
void inserir(Produto produtos[], int *n) {
    Produto novo;                             // variável temporária para novo produto
    // gera ID automático: se não há produtos, começa em 1; caso contrário, pega último id + 1
    novo.id = (*n == 0) ? 1 : produtos[*n - 1].id + 1;

    printf("Nome: ");                         // pede nome
    scanf(" %[^\n]", novo.nome);              // lê linha (inclui espaços) até '\n'
    if (existeNome(produtos, *n, novo.nome)) { // verifica duplicata
        printf("Produto duplicado!\n");       // avisa e cancela inserção
        return;
    }
    printf("Categoria: ");                    // pede categoria
    scanf(" %[^\n]", novo.categoria);         // lê categoria
    printf("Preco: ");                        // pede preço
    scanf("%f", &novo.preco);                 // lê float
    printf("Quantidade: ");                   // pede quantidade
    scanf("%d", &novo.quantidade);            // lê inteiro

    produtos[*n] = novo;                      // insere no final do vetor
    (*n)++;                                   // incrementa contador
    salvarArquivo(produtos, *n);              // salva alterações no arquivo
    printf("Produto cadastrado!\n");          // confirma inserção
}

// Buscar: Procura substring no nome e exibe produtos correspondentes
void buscar(Produto produtos[], int n) {
    char termo[50];                            // buffer para termo de busca
    printf("Digite o nome: ");                 // solicita termo
    scanf(" %[^\n]", termo);                   // lê termo com espaços
    int achou = 0;                             // flag se encontrou algum resultado
    for (int i = 0; i < n; i++) {              // percorre todos os produtos
        if (strstr(produtos[i].nome, termo) != NULL) { // busca substring sensível a case
            // imprime produto encontrado
            printf("%d | %s | %s | %.2f | %d\n",
                   produtos[i].id, produtos[i].nome,
                   produtos[i].categoria, produtos[i].preco,
                   produtos[i].quantidade);
            achou = 1;                        // marca que achou pelo menos um
        }
    }
    if (!achou) printf("Nenhum produto encontrado!\n"); // se não encontrou, informa
}

// Alterar: Modifica dados de um produto identificado pelo ID e salva no arquivo
void alterar(Produto produtos[], int n) {
    int id, encontrado = 0;                    // id procurado e flag de encontrado
    printf("Digite o ID do produto que deseja alterar: "); // pede ID
    scanf("%d", &id);                          // lê ID

    for (int i = 0; i < n; i++) {              // procura produto com o ID informado
        if (produtos[i].id == id) {            // se encontrou
            encontrado = 1;                    // marca encontrado
            printf("Novo nome (%s): ", produtos[i].nome); // pede novo nome (mostra atual)
            scanf(" %[^\n]", produtos[i].nome); // lê novo nome (substitui)
            printf("Nova categoria (%s): ", produtos[i].categoria); // pede categoria
            scanf(" %[^\n]", produtos[i].categoria); // lê nova categoria
            printf("Novo preco (%.2f): ", produtos[i].preco); // pede novo preço
            scanf("%f", &produtos[i].preco);   // lê preço
            printf("Nova quantidade (%d): ", produtos[i].quantidade); // pede nova qtd
            scanf("%d", &produtos[i].quantidade); // lê quantidade
            salvarArquivo(produtos, n);        // salva alterações no arquivo
            printf("Produto alterado com sucesso!\n"); // confirma alteração
            break;                             // sai do laço, já alterou
        }
    }

    if (!encontrado)                            // se não encontrou nenhum produto com esse ID
        printf("Produto com ID %d nao encontrado!\n", id); // informa usuário
}

// Excluir: Remove produto por ID, desloca vetor e salva arquivo
void excluir(Produto produtos[], int *n) {
    int id, pos = -1;                          // id a excluir e posição encontrada
    printf("Digite o ID para excluir: ");      // pede ID
    scanf("%d", &id);                          // lê ID
    for (int i = 0; i < *n; i++) {             // procura posição do ID
        if (produtos[i].id == id) {
            pos = i;                           // grava posição
            break;                             // sai do laço
        }
    }
    if (pos == -1) {                           // se não encontrou
        printf("ID nao encontrado!\n");        // informa usuário
        return;                                // cancela exclusão
    }
    for (int i = pos; i < *n - 1; i++) {       // desloca itens à esquerda para "fechar" gap
        produtos[i] = produtos[i + 1];         // copia o próximo sobre o atual
    }
    (*n)--;                                    // reduz contador de produtos
    salvarArquivo(produtos, *n);               // salva nova lista no arquivo
    printf("Produto excluido!\n");             // confirma exclusão
}


/* Ordenações (Bubble Sort) */

// Ordena vetor por nome em ordem alfabética A->Z (case-insensitive)
void ordenarNomeAZ(Produto produtos[], int n) {
    for (int i = 0; i < n - 1; i++) {          								// laço externo: n-1 passagens
        for (int j = 0; j < n - i - 1; j++) {  								// laço interno: compara adjacentes
            if (strcasecmp(produtos[j].nome, produtos[j + 1].nome) > 0) { 	// se j > j+1
                Produto tmp = produtos[j];     								// troca usando variável temporária
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
        }
    }
}

// Ordena vetor por nome em ordem alfabética Z->A
void ordenarNomeZA(Produto produtos[], int n) {
    for (int i = 0; i < n - 1; i++) {          								// mesma lógica do anterior
        for (int j = 0; j < n - i - 1; j++) {
            if (strcasecmp(produtos[j].nome, produtos[j + 1].nome) < 0) { 	// inverte condição
                Produto tmp = produtos[j];
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
        }
    }
}

// Ordena por preço menor -> maior
void ordenarPrecoCresc(Produto produtos[], int n) {
    for (int i = 0; i < n - 1; i++) {          // laços padrão do Bubble Sort
        for (int j = 0; j < n - i - 1; j++) {
            if (produtos[j].preco > produtos[j + 1].preco) { // se preço atual > próximo
                Produto tmp = produtos[j];   // troca
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
        }
    }
}

// Ordena por preço maior -> menor
void ordenarPrecoDesc(Produto produtos[], int n) {
    for (int i = 0; i < n - 1; i++) {          // laços do Bubble Sort
        for (int j = 0; j < n - i - 1; j++) {
            if (produtos[j].preco < produtos[j + 1].preco) { // condição invertida
                Produto tmp = produtos[j];   // troca elementos
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = tmp;
            }
        }
    }
}


/* Exportar CSV (vírgula) */

/* Grava arquivo "produtos.csv" com separador de vírgula */
void exportarCSV(Produto produtos[], int n) {
    FILE *f = fopen("produtos.csv", "w");      	// abre/cria CSV
    if (!f) {                                 	// checa erro na abertura
        printf("Erro ao criar produtos.csv\n"); // avisa se falha
        return;                                	// aborta exportação
    }
    fprintf(f, "ID,Nome,Categoria,Preco,Quantidade\n"); // cabeçalho CSV
    for (int i = 0; i < n; i++) {              			// escreve cada produto em uma linha CSV
        fprintf(f, "%d,%s,%s,%.2f,%d\n",
                produtos[i].id, produtos[i].nome,
                produtos[i].categoria, produtos[i].preco,
                produtos[i].quantidade);
    }
    fclose(f);                                 			// fecha CSV
    printf("Dados exportados para produtos.csv!\n"); 	// confirma exportação
}


/* Menu Principal */

int main() {
    Produto produtos[MAX];                     	// vetor estático com capacidade MAX
    int n = carregarArquivo(produtos);         	// carrega produtos do arquivo e recebe quantidade
    int opcao;                                 	// variável para opção do menu

    do {                                       	// loop principal do menu (repete até opção 0)
        printf("\n===== MENU =====\n");        	// cabeçalho do menu
        printf("1 - Listar produtos\n");       	// opção 1: listar
        printf("2 - Inserir novo produto\n");  	// opção 2: inserir
        printf("3 - Buscar por nome\n");       	// opção 3: buscar
        printf("4 - Alterar produto\n");       	// opção 4: alterar
        printf("5 - Excluir produto\n");       	// opção 5: excluir
        printf("6 - Ordenar por nome (A-Z)\n"); // opção 6: ordenar nome A-Z
        printf("7 - Ordenar por nome (Z-A)\n"); // opção 7: ordenar nome Z-A
        printf("8 - Ordenar por preco (menor->maior)\n"); // opção 8: ordenar preço crescente
        printf("9 - Ordenar por preco (maior->menor)\n"); // opção 9: ordenar preço decrescente
        printf("10 - Exportar para CSV\n");    	// opção 10: exportar CSV
        printf("0 - Sair\n");                  	// opção 0: sair
        printf("Opcao: ");                     	// pede escolha
        scanf("%d", &opcao);                   // lê escolha

        switch (opcao) {                       // trata cada opção do menu
            case 1:
                listar(produtos, n);           // lista produtos (formato tabela)
                break;
            case 2:
                inserir(produtos, &n);        // insere novo produto (salva arquivo)
                break;
            case 3:
                buscar(produtos, n);          // busca por nome e mostra resultados
                break;
            case 4:
                alterar(produtos, n);         // altera produto por ID (salva arquivo)
                break;
            case 5:
                excluir(produtos, &n);        // exclui produto por ID (salva arquivo)
                break;
            case 6:
                if (n == 0) {                 // verifica se há produtos para ordenar
                    printf("Nenhum produto para ordenar.\n");
                    break;
                }
                // cria cópia temporária em memória para ordenar sem salvar no arquivo
                {
                    Produto copia[MAX];       // vetor cópia (tamanho suficiente)
                    for (int i = 0; i < n; i++) copia[i] = produtos[i]; // copia elementos
                    ordenarNomeAZ(copia, n);  // ordena a cópia por nome A->Z
                    printf("\nOrdenado por nome (A-Z) - exibido apenas na tela (arquivo mantido):\n");
                    listar(copia, n);         // exibe a cópia ordenada (arquivo intacto)
                }
                break;
            case 7:
                if (n == 0) { printf("Nenhum produto para ordenar.\n"); break; }
                {
                    Produto copia[MAX]; for (int i = 0; i < n; i++) copia[i] = produtos[i];
                    ordenarNomeZA(copia, n);  // ordena a cópia por nome Z->A
                    printf("\nOrdenado por nome (Z-A) - exibido apenas na tela (arquivo mantido):\n");
                    listar(copia, n);         // mostra cópia
                }
                break;
            case 8:
                if (n == 0) { printf("Nenhum produto para ordenar.\n"); break; }
                {
                    Produto copia[MAX]; for (int i = 0; i < n; i++) copia[i] = produtos[i];
                    ordenarPrecoCresc(copia, n); // ordena cópia por preço crescente
                    printf("\nOrdenado por preco (menor->maior) - exibido apenas na tela (arquivo mantido):\n");
                    listar(copia, n);           // mostra cópia ordenada
                }
                break;
            case 9:
                if (n == 0) { printf("Nenhum produto para ordenar.\n"); break; }
                {
                    Produto copia[MAX]; for (int i = 0; i < n; i++) copia[i] = produtos[i];
                    ordenarPrecoDesc(copia, n); // ordena cópia por preço decrescente
                    printf("\nOrdenado por preco (maior->menor) - exibido apenas na tela (arquivo mantido):\n");
                    listar(copia, n);           // mostra cópia ordenada
                }
                break;
            case 10:
                exportarCSV(produtos, n);     // exporta todos os dados para produtos.csv
                break;
            case 0:
                printf("Saindo...\n");        // mensagem de saída
                break;
            default:
                printf("Opcao invalida!\n");  // tratamento para opção desconhecida
        }
    } while (opcao != 0);                     // repete até o usuário escolher 0

    return 0;                                 // encerra o programa com código 0
}
