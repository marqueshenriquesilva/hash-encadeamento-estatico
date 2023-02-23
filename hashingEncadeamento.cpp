/* 
Esse programa foi escrito, compilado e testado em um ambiente Linux Ubuntu, mas deve funcionar tambem em ambientes Windows
Como pedido no enunciado, possui funcoes de inserir, buscar, exibir, imprimir de iniciais iguais, e remover valores de uma tabela usando hash
Todas as operacoes sao efetuadas com o uso de uma funcao hash, aplicada via encadeamento estatico com overflow
Eh essencial que o tipo correto de valor seja colocado em cada prompt
As chaves de cada registro possuem capacidade de 1 (incluso) para ate 19 caracteres (o 20o eh o '/0')
Atencao: os nomes devem comecar com letra maiuscula
A remocao so tem efeito caso outro valor de mesma inicial seja inserido para garantir funcionamento da funcao imprimir_inicial
*/

#include <stdio.h>
#include <string.h>
#define TF 36

struct pessoa
{
	char nome[20];
	char inicial;
	char status;
	int elo;
};

void inicializar_tabela(struct pessoa tabela[TF]);
void imprimir_menu(void);
void imprimir_tabela(struct pessoa tabela[TF]);
int index_hash(char nome[]);
void inserir_valor(char nome[], int endereco, struct pessoa tabela[TF]);
int buscar_nome(char nome[20], int end, struct pessoa tab[TF]);
void imprimir_inicial(char inicial[], struct pessoa tabela[TF]);
void remover_valor(int posicao, struct pessoa tabela[TF]);



int main(void)
{
	// Declaracao de variaveis
	int opcao = 0, endereco, posicao = -1;
	char nome[20], inicial[1];
	struct pessoa tabela[TF];
	
	// Tabela eh inicializada com valores vazios
	inicializar_tabela(tabela);
	printf("Tabela inicializada com sucesso\n");
	
	// Menu
	while(opcao != 6)
	{
		// Usuario entra com opcao desejada
		printf("\n");
		imprimir_menu();
		scanf("%d", &opcao);
		printf("\n");
		switch(opcao)
		{
			case 1:
				printf("Nome da nova pessoa: ");
				scanf("%s", nome);
				endereco = index_hash(nome);
				inserir_valor(nome, endereco, tabela);		
				break;
			case 2:
				imprimir_tabela(tabela);
				break;
			case 3:
				printf("Nome da pessoa procurada: ");
				scanf("%s", nome);
				endereco = index_hash(nome);
				posicao = buscar_nome(nome, endereco, tabela);
				if (posicao < 0)
					printf("Valor nao foi encontrado\n");
				else
					printf("Nome %s encontrado na posicao %d\n", nome, posicao);
				break;
			case 4:
				// Limpa o buffer para fazer a funcao getchar funcionar
				printf("Escreva inicial dos nomes: ");
				scanf("%s", inicial);
				imprimir_inicial(inicial, tabela);
				break;
			case 5:
				printf("Nome da pessoa a ser removida: ");
				scanf("%s", nome);
				endereco = index_hash(nome);
				posicao = buscar_nome(nome, endereco, tabela);
				if (posicao < 0)
					printf("Valor nao foi encontrado\n");
				else
				{
					remover_valor(posicao, tabela);
					printf("Valor removido com sucesso\n");
				}
				break;
			case 6:
				break;
			default:
			{
				printf("Entre com opcao valida\n");
			}
		}
	}
	
	return 0;
}



void inicializar_tabela(struct pessoa tabela[TF])
{
	// Inicializa tabela com valores vazios
	for (int i = 0; i < TF; i++)
	{
		strcpy(tabela[i].nome, "");
		tabela[i].inicial = '\0';
		tabela[i].status = 'L';
		tabela[i].elo = -1;
	}
	
	// Inicializa iniciais
	for (int i = 0, j = 78; i < 26; i++, j++)
	{
		if (j > 90)
			j = 65;
		tabela[i].inicial = (char) j;
	}
}

void imprimir_menu(void)
{
	// Imprimi menu de opcoes
	printf("1. Inserir nome\n");
	printf("2. Exibir tabela\n");
	printf("3. Buscar nome\n");
	printf("4. Exibir nomes por inicial\n");
	printf("5. Remover valor\n");
	printf("6. Sair\n");
	printf("Opcao: ");
}

void imprimir_tabela(struct pessoa tabela[TF])
{
	// Imprimi valores da tabela
	struct pessoa valor;
	printf("Posicao	Status	Letra	Nome	Elo\n");
	int i = 0;
	for (i = 0; i < TF; i++)
	{
		printf("%d	%c	%c	%s	%d\n", i, tabela[i].status, tabela[i].inicial, tabela[i].nome, tabela[i].elo);
	}
}

int index_hash(char nome[])
{
	// Retorna um endereco hash baseado na inicial do nome
	return nome[0] % 26;
}

void inserir_valor(char nome[], int end, struct pessoa tab[TF])
{
	// Cria struct de pessoa com base no nome
	struct pessoa inserido;
	inserido.status = 'O';
	inserido.inicial = nome[0];
	strcpy(inserido.nome, nome);
	inserido.elo = tab[end].elo;
	
	// Procura espaco na tabela se endereco hash estiver ocupado
	if (tab[end].status == 'O')
	{
		// Index inicial do bucket e valor de mesma inicial que esta ocupando espaco
		int i = 26, antes = -1, pode_inserir = 0;
		
		// Itera pelo bucket enquanto nao pode inserir ou index eh menor que TF
		while(i < TF && !pode_inserir)
		{
			// Se encontrar um valor que esta ocupado mas possui mesma inicial, salva seu index
			if (tab[i].status == 'O' && tab[i].inicial == inserido.inicial)
			{
				antes = i;
			}
			// Insere valor apenas se tiver mesma inicial e estar livre para evitar problemas nas outras funcoes e.g imprimi_inicial()
			if ((tab[i].inicial == inserido.inicial || tab[i].inicial == '\0') && tab[i].status != 'O')
			{
				pode_inserir = 1;
			}
			else
			{
				i++;
			}
		}

		// Imprimi mensagem se tabela estiver cheia
		if (i >= TF)
		{
			printf("Tabela cheia\n");
		}
		else
		{
			// Salva os novas informacoes e linka valor com mesma inicial antes de i com o link correto
			tab[end].elo = i;
			inserido.elo = antes;
			tab[i] = inserido;
			printf("Nova entrada inserida com sucesso\n");
		}
	}
	// Se endereco hash estiver livre, insere nome
	else
	{
		tab[end] = inserido;
		printf("Nova entrada inserida com sucesso\n");
	}
}

int buscar_nome(char nome[20], int end, struct pessoa tab[TF])
{
	int pos = -1;
	// Se o nome nao for igual, procura na tabela
	if ((strcmp(tab[end].nome, nome)))
	{
		int procurada = tab[end].elo;
		// Procura por elos enquanto nao encontra igual
		while(procurada != -1)
		{
			// Retorna posicao caso nome no elo seja igual
			if ((strcmp(tab[procurada].nome, nome)))
				procurada = tab[procurada].elo;
			else
				return procurada;
		}
	}
	// Se for igual, imprimi na tela posicao
	else
	{
		pos = end;
	}
	return pos;
}

void imprimir_inicial(char inicial[], struct pessoa tabela[TF])
{
	// Funcao hash da posicao
	int pos = inicial[0] % 26;

	// Verifica se ha valores com inicial
	if (tabela[pos].status == 'L')
		printf("Nao ha nomes com essa inicial\n");
	else
	{
		printf("Posicao	Status	Letra	Nome	Elo\n");

		// Imprimi valor ate elo nao referenciar nenhum
		while (pos >= 0)
		{
			printf("%d	%c	%c	%s	%d\n", pos, tabela[pos].status, tabela[pos].inicial, tabela[pos].nome, tabela[pos].elo);
			pos = tabela[pos].elo;
		}
	}
}

void remover_valor(int posicao, struct pessoa tabela[TF])
{
	// Troca status para char R na posicao dada
	tabela[posicao].status = 'R';
}
