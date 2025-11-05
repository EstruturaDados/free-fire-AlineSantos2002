#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Enum para critérios de ordenação
typedef enum {
    POR_NOME,
    POR_TIPO, 
    POR_PRIORIDADE
} CriterioOrdenacao;

// Struct do Item
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade; // 1-5 (nível mestre)
} Item;

// Variáveis globais
Item mochila[MAX_ITENS];
int totalItens = 0;
bool ordenadoPorNome = false;
CriterioOrdenacao criterioAtual = POR_NOME;

// Protótipos das funções
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void buscarItemSequencial();
void ordenarItens();
void buscarItemBinario();
int compararItens(const Item* a, const Item* b, CriterioOrdenacao criterio);
void insertionSort(int* comparacoes);

int main() {
    printf("=== MOCHILA DA ILHA - NIVEL MESTRE ===\n");
    printf("Sistema de gerenciamento de inventario\n\n");
    
    int opcao;
    
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        
        switch(opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItemSequencial();
                break;
            case 5:
                ordenarItens();
                break;
            case 6:
                buscarItemBinario();
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while(opcao != 0);
    
    return 0;
}

void exibirMenu() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1 - Inserir item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Buscar item (sequencial)\n");
    printf("5 - Ordenar itens\n");
    printf("6 - Buscar item (binaria)\n");
    printf("0 - Sair\n");
}

void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("Erro: Mochila cheia! Capacidade maxima: %d itens\n", MAX_ITENS);
        return;
    }
    
    Item novoItem;
    
    printf("\n=== INSERIR NOVO ITEM ===\n");
    
    printf("Nome do item: ");
    fgets(novoItem.nome, TAM_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // Remove \n
    
    printf("Tipo (arma, municao, cura, ferramenta): ");
    fgets(novoItem.tipo, TAM_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;
    
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    
    printf("Prioridade (1-5, onde 5 é mais importante): ");
    scanf("%d", &novoItem.prioridade);
    
    // Validações
    if (novoItem.quantidade <= 0) {
        printf("Erro: Quantidade deve ser positiva!\n");
        return;
    }
    
    if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("Erro: Prioridade deve ser entre 1 e 5!\n");
        return;
    }
    
    mochila[totalItens] = novoItem;
    totalItens++;
    ordenadoPorNome = false; // Lista foi modificada
    
    printf("Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

void removerItem() {
    if (totalItens == 0) {
        printf("Mochila vazia! Nada para remover.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    
    printf("\n=== REMOVER ITEM ===\n");
    printf("Nome do item a remover: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
    
    // Busca sequencial para encontrar o item
    int indice = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        printf("Item '%s' nao encontrado na mochila!\n", nomeBusca);
        return;
    }
    
    // Remove o item movendo os elementos
    printf("Removendo item: %s (%s) - %d unidades\n", 
           mochila[indice].nome, mochila[indice].tipo, mochila[indice].quantidade);
    
    for (int i = indice; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    
    totalItens--;
    ordenadoPorNome = false; // Lista foi modificada
    
    printf("Item removido com sucesso!\n");
}

void listarItens() {
    if (totalItens == 0) {
        printf("Mochila vazia!\n");
        return;
    }
    
    printf("\n=== ITENS NA MOCHILA (%d/%d) ===\n", totalItens, MAX_ITENS);
    printf("%-3s | %-20s | %-15s | %-10s | %s\n", 
           "ID", "Nome", "Tipo", "Quantidade", "Prioridade");
    printf("----|----------------------|-----------------|------------|-----------\n");
    
    for (int i = 0; i < totalItens; i++) {
        printf("%-3d | %-20s | %-15s | %-10d | %d\n", 
               i + 1, mochila[i].nome, mochila[i].tipo, 
               mochila[i].quantidade, mochila[i].prioridade);
    }
}

void buscarItemSequencial() {
    if (totalItens == 0) {
        printf("Mochila vazia! Nada para buscar.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    
    printf("\n=== BUSCA SEQUENCIAL ===\n");
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
    
    // Busca sequencial
    bool encontrado = false;
    int comparacoes = 0;
    
    for (int i = 0; i < totalItens; i++) {
        comparacoes++;
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n*** ITEM ENCONTRADO ***\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("Prioridade: %d\n", mochila[i].prioridade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            encontrado = true;
            break;
        }
    }
    
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila!\n", nomeBusca);
        printf("Comparacoes realizadas: %d\n", comparacoes);
    }
}

int compararItens(const Item* a, const Item* b, CriterioOrdenacao criterio) {
    switch(criterio) {
        case POR_NOME:
            return strcmp(a->nome, b->nome);
        case POR_TIPO:
            return strcmp(a->tipo, b->tipo);
        case POR_PRIORIDADE:
            return b->prioridade - a->prioridade; // Decrescente (maior prioridade primeiro)
        default:
            return 0;
    }
}

void insertionSort(int* comparacoes) {
    *comparacoes = 0;
    
    for (int i = 1; i < totalItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        
        while (j >= 0 && compararItens(&mochila[j], &chave, criterioAtual) > 0) {
            (*comparacoes)++;
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = chave;
    }
}

void ordenarItens() {
    if (totalItens == 0) {
        printf("Mochila vazia! Nada para ordenar.\n");
        return;
    }
    
    printf("\n=== ORDENAR ITENS ===\n");
    printf("1 - Por nome (A-Z)\n");
    printf("2 - Por tipo\n");
    printf("3 - Por prioridade (maior primeiro)\n");
    printf("Escolha o criterio: ");
    
    int opcao;
    scanf("%d", &opcao);
    getchar();
    
    switch(opcao) {
        case 1:
            criterioAtual = POR_NOME;
            break;
        case 2:
            criterioAtual = POR_TIPO;
            break;
        case 3:
            criterioAtual = POR_PRIORIDADE;
            break;
        default:
            printf("Opcao invalida! Usando ordenacao por nome.\n");
            criterioAtual = POR_NOME;
    }
    
    int comparacoes = 0;
    insertionSort(&comparacoes);
    
    if (criterioAtual == POR_NOME) {
        ordenadoPorNome = true;
    }
    
    printf("Itens ordenados com sucesso!\n");
    printf("Total de comparacoes realizadas: %d\n", comparacoes);
    
    listarItens();
}

void buscarItemBinario() {
    if (totalItens == 0) {
        printf("Mochila vazia! Nada para buscar.\n");
        return;
    }
    
    if (!ordenadoPorNome) {
        printf("Erro: Para busca binaria, a lista deve estar ordenada por nome!\n");
        printf("Use a opcao 5 para ordenar por nome primeiro.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    
    printf("\n=== BUSCA BINARIA ===\n");
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
    
    // Busca binária
    int esquerda = 0;
    int direita = totalItens - 1;
    int comparacoes = 0;
    bool encontrado = false;
    
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        comparacoes++;
        
        int comparacao = strcmp(mochila[meio].nome, nomeBusca);
        
        if (comparacao == 0) {
            printf("\n*** ITEM ENCONTRADO ***\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            encontrado = true;
            break;
        } else if (comparacao < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila!\n", nomeBusca);
        printf("Comparacoes realizadas: %d\n", comparacoes);
    }
}
