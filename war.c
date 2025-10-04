#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

//========================= ESTRUTURAS DE DADOS =========================//

struct Territorio { 
    char nome[30];
    char cor[10];
    int tropas;
};

struct Jogador {
    char nome[50];
    char cor[10];
    char* missao; // Ponteiro para a missão alocada dinamicamente
};

//========================= PROTÓTIPOS DAS FUNÇÕES =========================//
// Declarando todas as funções aqui, antes do main.

void limparBufferEntrada(); // Função para limpar o buffer de entrada
void exibirMenu();// Função para exibir o menu principal
void exibirMissao(const char* missao); // Função para exibir a missão do jogador
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes);// Função para atribuir uma missão aleatória a um jogador
int verificarMissao(const char* missao, const char* corDoJogador, struct Territorio* mapa, int territoriosCadastrados); // Função para verificar se a missão foi cumprida
void cadastrarJogadores(struct Jogador* jogadores, int numJogadores, const char* missoes[], int totalMissoes); // Função para cadastrar jogadores
void atacar(struct Territorio* atacante, struct Territorio* defensor); // Função para realizar o ataque entre territórios
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int numJogadores); // Função para liberar memória alocada
void cadastrarTerritorio(struct Territorio* war, int* territoriosCadastrados, int totalTerritorios); // Função para cadastrar territórios
void listarTerritorios(struct Territorio* war, int territoriosCadastrados); // Função para listar territórios


//========================= FUNÇÃO PRINCIPAL (main) =========================//
int main() {
    // --- CONFIGURAÇÃO INICIAL ---
    setlocale(LC_ALL, "Portuguese"); // Configura a localidade para Português (Brasil)
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    const char* missoes[] = { // Lista de missões disponíveis
        "CONQUISTAR 3 TERRITORIOS",
        "ELIMINAR O EXERCITO VERMELHO",
        "ACUMULAR 12 TROPAS",
        "CONQUISTAR O TERRITORIO BRASIL",
        "TER UM TERRITORIO COM 8 TROPAS"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);// Calcula o total de missões

    printf("Bem-vindo ao WAR em C!\n");

    // --- CADASTRO DE JOGADORES ---
    int numJogadores = 0; 
    printf("Quantos jogadores irão participar (ex: 2)? "); // Solicita o número de jogadores
    scanf("%d", &numJogadores);
    limparBufferEntrada(); 

    struct Jogador* jogadores = (struct Jogador*) calloc(numJogadores, sizeof(struct Jogador)); // Aloca memória para os jogadores
    if (jogadores == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro fatal: Falha ao alocar memória para os jogadores!\n");
        return 1; // Encerra o programa em caso de erro
    }
    cadastrarJogadores(jogadores, numJogadores, missoes, totalMissoes); // Chama a função para cadastrar os jogadores

    // --- SETUP DO MAPA ---
    int totalTerritorios = 0;       // Capacidade máxima do mapa
    int territoriosCadastrados = 0; // Contador de territórios já criados

    // Solicita ao usuário o número máximo de territórios
    printf("\nDigite o número máximo de territórios para o mapa: ");
    scanf("%d", &totalTerritorios); 
    limparBufferEntrada();

    // Aloca memória para os territórios do mapa
    struct Territorio* war = (struct Territorio*) calloc(totalTerritorios, sizeof(struct Territorio));
    if (war == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro fatal: Falha ao alocar memória para o mapa!\n");
        liberarMemoria(NULL, jogadores, numJogadores); // Libera o que já foi alocado
        return 1;
    }
    printf("Memória para %d territórios alocada com sucesso. Cadastre-os no menu.\n", totalTerritorios); 
    
    // --- LOOP PRINCIPAL DO JOGO ---
    int opcao = -1; // Variável para armazenar a opção do menu
    int turno = 0; // Contador de turnos
    int gameOver = 0; // Flag para indicar o fim do jogo
 
    do {
        int jogadorAtual = turno % numJogadores; // Alterna entre os jogadores de forma circular
        // Exibe o turno atual
        printf("\n======================================\n");
        printf("TURNO DE: %s (Cor: %s)\n", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].cor); // Indica de quem é o turno
        printf("======================================\n");

        exibirMenu(); // Exibe o menu principal
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) { // Verifica se a entrada é um número
            opcao = -1; // Se a entrada não for um número, define como opção inválida
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1: { // Cadastrar UM Território
                cadastrarTerritorio(war, &territoriosCadastrados, totalTerritorios); // Chama a função para cadastrar um território
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }

            case 2: { 
                //Chama a função para listar os territórios cadastrados
                listarTerritorios(war, territoriosCadastrados);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            }

            case 3: { 
                // chama a função para atacar
                    if (territoriosCadastrados < 2) { // Verifica se há territórios suficientes para atacar
                    printf("É preciso ter pelo menos 2 territórios cadastrados para atacar.\n"); 
                    printf("Pressione ENTER para continuar...");
                    getchar();
                    break;
                }
                // Exibe o estado atual do mapa
                printf("\n--- MAPA DO MUNDO - ESTADO ATUAL ---\n");
                for (int i = 0; i < territoriosCadastrados; i++) { // Lista cada território cadastrado
                    printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas);
                }
                printf("--------------------------------------\n");
                // Solicita ao jogador que escolha os territórios para o ataque
                int id_atacante, id_defesa;
                printf("Digite o número do território ATACANTE: ");
                scanf("%d", &id_atacante);
                limparBufferEntrada();
                printf("Digite o número do território DEFENSOR: ");
                scanf("%d", &id_defesa);
                limparBufferEntrada();
                // Ajusta os índices para acessar o array (base 0)
                int idx_atac = id_atacante - 1;
                int idx_def = id_defesa - 1;

                //printf("\n[DEBUG] territoriosCadastrados = %d | idx_atac = %d | idx_def = %d\n\n", territoriosCadastrados, idx_atac, idx_def);
                // Valida as seleções e executa o ataque se forem válidas
                if (idx_atac >= 0 && idx_atac < territoriosCadastrados && idx_def >= 0 && idx_def < territoriosCadastrados) {
                    if (strcmp(jogadores[jogadorAtual].cor, war[idx_atac].cor) != 0) { // Verifica se o território atacante pertence ao jogador atual
                        printf("Ação inválida! Você só pode atacar com territórios da sua cor (%s).\n", jogadores[jogadorAtual].cor);
                    } else if (strcmp(war[idx_atac].cor, war[idx_def].cor) == 0) { // Verifica se o território defensor é aliado
                        printf("Ação inválida! Não é possível atacar um território aliado.\n");
                    } else {
                        // Executa o ataque
                        atacar(&war[idx_atac], &war[idx_def]);
                    }
                } else {
                    printf("Seleção inválida! Território não existe.\n");
                }
                
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }

            case 4: { // Passar o turno para o próximo jogador
                printf("\nTurno passado para o próximo jogador.\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }

            case 0:
                // Encerra o jogo
                printf("Saindo do Jogo...\n");
                break;
            
            default:
            // Opção inválida do menu
                printf("Opção inválida! Tente Novamente.\n");
                printf("Pressione ENTER para Continuar...");
                getchar();
                break;
        }

        if (opcao != 0 && !gameOver) { // Verifica se o jogo não terminou
            // Verifica se algum jogador cumpriu sua missão
            for (int i = 0; i < numJogadores; i++) { // Loop através dos jogadores
                if (verificarMissao(jogadores[i].missao, jogadores[i].cor, war, territoriosCadastrados)) { // Verifica se a missão foi cumprida
                    printf("\n\n!!! FIM DE JOGO !!!\n");
                    printf("O jogador %s (Cor: %s) cumpriu sua missão: \"%s\"\n", jogadores[i].nome, jogadores[i].cor, jogadores[i].missao);
                    printf("PARABÉNS!\n");
                    gameOver = 1;
                    break;
                }
            }
        }
        
        if (opcao == 3 || opcao == 4) { // Incrementa o turno apenas se a ação foi atacar ou passar o turno
             turno++;
        }

    } while (opcao != 0 && !gameOver); // Continua até o jogador escolher sair ou o jogo terminar

    // --- LIMPEZA DA MEMÓRIA ---
    liberarMemoria(war, jogadores, numJogadores); // Libera toda a memória alocada
    printf("Obrigado por jogar WAR em C! Até a próxima!\n");
    
    return 0;
}
//========================= FIM DA FUNÇÃO PRINCIPAL =========================//


//========================= DEFINIÇÕES DAS FUNÇÕES =========================//

void limparBufferEntrada() { // Função para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() { // Função para exibir o menu principal
    printf("\n==========================\n");
    printf("------ JOGO WAR ----------\n");
    printf("==========================\n");
    printf("1. Cadastrar Território\n");
    printf("2. Listar Territórios\n");
    printf("3. Atacar\n");
    printf("4. Passar o turno\n");
    printf("0. Sair\n");
    printf("---------------------------\n");
}

void atacar(struct Territorio* atacante, struct Territorio* defensor) { // Função para realizar o ataque entre territórios
    printf("\n------ --- Ataque Iniciado --- ------\n");
    printf("%s (%d tropas) ataca %s (%d tropas)!\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    printf("--------------------------------------\n");

    // CORREÇÃO: Regra de ataque ajustada para > 1 tropa.
    if (atacante->tropas <= 1) {
        printf("Ataque falhou! %s precisa ter mais de 1 tropa para atacar.\n", atacante->nome);
        return;
    }
    
    if (defensor->tropas <= 0) {
        printf("Ataque falhou! O %s já não possui tropas para defender.\n", defensor->nome);
        return;
    }

    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    // CORREÇÃO: Formatação do printf do dado.
    printf("Jogo de dados: %s rolou %d | %s rolou %d\n", atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        defensor->tropas--;
        printf("Ataque bem-sucedido! %s perdeu 1 tropa.\n", defensor->nome);

        if (defensor->tropas == 0) {
            printf("--------------------------------------\n");
            printf("TERRITÓRIO CONQUISTADO!\n");
            
            strcpy(defensor->cor, atacante->cor);
            printf("%s agora pertence à cor %s.\n", defensor->nome, defensor->cor);

            int tropas_para_mover = atacante->tropas / 2;
            if (tropas_para_mover == 0) {
                tropas_para_mover = 1;
            }

            printf("Transferindo %d tropas de %s para %s.\n", tropas_para_mover, atacante->nome, defensor->nome);
            
            defensor->tropas = tropas_para_mover;
            atacante->tropas -= tropas_para_mover;
        }
    } else {
        atacante->tropas--;
        printf("Defesa bem-sucedida! %s perdeu 1 tropa.\n", atacante->nome);
    }
    
    printf("\n--- Tropas restantes ---\n");
    printf(" -> %s: %d\n", atacante->nome, atacante->tropas);
    printf(" -> %s: %d\n", defensor->nome, defensor->tropas);
    printf("--- FIM DO ATAQUE ---\n");
}

void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int numJogadores) {
    printf("\nLiberando toda a memória alocada...\n");

    if (mapa != NULL) {
        free(mapa);
    }
    if (jogadores != NULL) {
        for (int i = 0; i < numJogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
            }
        }
        free(jogadores);
    }
    printf("Memória liberada com sucesso.\n");
}

void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    const char* missaoEscolhida = missoes[indiceSorteado];

    *destino = (char*) malloc(strlen(missaoEscolhida) + 1);
    if (*destino == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a missão!\n");
        exit(1);
    }
    strcpy(*destino, missaoEscolhida);
}

// CORREÇÃO: A função 'verificarMissao' precisa saber quantos territórios estão cadastrados.
// Adicionamos o parâmetro 'territoriosCadastrados'.
int verificarMissao(const char* missao, const char* corDoJogador, struct Territorio* mapa, int territoriosCadastrados) {
    if (strcmp(missao, "CONQUISTAR 3 TERRITORIOS") == 0) {
        int contador = 0;
        for (int i = 0; i < territoriosCadastrados; i++) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0) {
                contador++;
            }
        }
        return (contador >= 3);
    }
    else if (strcmp(missao, "ELIMINAR O EXERCITO VERMELHO") == 0) {
        for (int i = 0; i < territoriosCadastrados; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                return 0;
            }
        }
        return 1;
    }
    else if (strcmp(missao, "ACUMULAR 12 TROPAS") == 0) {
        int somaTropas = 0;
        for (int i = 0; i < territoriosCadastrados; i++) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0) {
                somaTropas += mapa[i].tropas;
            }
        }
        return (somaTropas >= 12);
    }
    else if (strcmp(missao, "CONQUISTAR O TERRITORIO BRASIL") == 0) {
        for (int i = 0; i < territoriosCadastrados; i++) {
            if (strcmp(mapa[i].nome, "Brasil") == 0) {
                return (strcmp(mapa[i].cor, corDoJogador) == 0);
            }
        }
        return 0;
    }
    else if (strcmp(missao, "TER UM TERRITORIO COM 8 TROPAS") == 0) {
        for (int i = 0; i < territoriosCadastrados; i++) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0 && mapa[i].tropas >= 8) {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

void exibirMissao(const char* missao) { 
    printf("Sua missão secreta é: \"%s\"\n", missao);
}


void cadastrarJogadores(struct Jogador* jogadores, int numJogadores, const char* missoes[], int totalMissoes) {
    printf("\n--- CADASTRO DE JOGADORES ---\n");
    for (int i = 0; i < numJogadores; i++) {
        printf("\n--- Jogador %d ---\n", i + 1);
        
        printf("Digite o nome do Jogador %d: ", i + 1);
        fgets(jogadores[i].nome, 50, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        int corValida = 0;
        while (!corValida) {
            printf("Digite a cor do Jogador %d (ex: Azul, Verde): ", i + 1);
            fgets(jogadores[i].cor, 10, stdin);
            jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

            corValida = 1;
            for (int j = 0; j < i; j++) {
                if (strcmp(jogadores[j].cor, jogadores[i].cor) == 0) {
                    printf("Erro: A cor '%s' já foi escolhida. Tente outra.\n", jogadores[i].cor);
                    corValida = 0;
                    break;
                }
            }
        }
        
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes);
        exibirMissao(jogadores[i].missao);
    }
}

void cadastrarTerritorio(struct Territorio* war, int* territoriosCadastrados, int totalTerritorios) {
    if (*territoriosCadastrados >= totalTerritorios) {
        printf("Erro: Capacidade máxima de territórios atingida (%d).\n", totalTerritorios);
        return;
    }

    struct Territorio* novoTerritorio = &war[*territoriosCadastrados];

    printf("\n--- Cadastro do Território %d ---\n", *territoriosCadastrados + 1);
    
    printf("Nome do Território: ");
    fgets(novoTerritorio->nome, 30, stdin);
    novoTerritorio->nome[strcspn(novoTerritorio->nome, "\n")] = '\0';

    printf("Cor do Território (ex: Azul, Verde): ");
    fgets(novoTerritorio->cor, 10, stdin);
    novoTerritorio->cor[strcspn(novoTerritorio->cor, "\n")] = '\0';

    printf("Número inicial de tropas (mínimo 1): ");
    int tropas;
    while (1) {
        if (scanf("%d", &tropas) != 1 || tropas < 1) {
            printf("Entrada inválida! Digite um número inteiro maior ou igual a 1: ");
            limparBufferEntrada();
        } else {
            break;
        }
    }
    limparBufferEntrada();
    novoTerritorio->tropas = tropas;

    (*territoriosCadastrados)++;
    printf("Território '%s' cadastrado com sucesso!\n", novoTerritorio->nome);
}

void listarTerritorios(struct Territorio* war, int territoriosCadastrados) { // Função para listar territórios
    printf("\n--- Lista de Territórios Cadastrados ---\n"); 
    if (territoriosCadastrados == 0) {// Verifica se há territórios cadastrados
        printf("Nenhum Território Cadastrado!\n");
    } else { 
        for (int i = 0; i < territoriosCadastrados; i++) { 
            printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas); // Lista cada território cadastrado
        }
    }
}

//========================= FIM DAS FUNÇÕES =========================//