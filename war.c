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
void executarTurnoDeAtaque(struct Territorio* war, int territoriosCadastrados, struct Jogador* jogadores, int jogadorAtual); // Função para gerenciar o turno de ataque
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
            case 1: { 
                // Cadastrar UM Território
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
                // chama a função para executar o turno de ataque do jogador atual
                executarTurnoDeAtaque(war, territoriosCadastrados, jogadores, jogadorAtual);
                break;
            }


            case 4: { 
                // Passar o turno para o próximo jogador
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
            // Verifica APENAS o jogador que acabou de jogar
            if (verificarMissao(jogadores[jogadorAtual].missao, jogadores[jogadorAtual].cor, war, territoriosCadastrados)) {
                printf("\n\n!!! FIM DE JOGO !!!\n"); 
                // Anuncia o vencedor
                printf("O jogador %s (Cor: %s) cumpriu sua missão: \"%s\"\n", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].cor, jogadores[jogadorAtual].missao);
                printf("PARABÉNS!\n"); 
                gameOver = 1;
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

/**
 * @brief Gerencia a ação de ataque completa para o turno de um jogador.
 * Exibe o mapa, solicita input, valida e chama a função 'atacar'.
 */
void executarTurnoDeAtaque(struct Territorio* war, int territoriosCadastrados, struct Jogador* jogadores, int jogadorAtual) { 
    if (territoriosCadastrados < 2) { // Verifica se há territórios suficientes para atacar
        printf("É preciso ter pelo menos 2 territórios cadastrados para atacar.\n"); 
    } else {
        // Exibe o estado atual do mapa
        printf("\n--- MAPA DO MUNDO - ESTADO ATUAL ---\n"); // Exibe o estado atual do mapa
        for (int i = 0; i < territoriosCadastrados; i++) { // Lista cada território cadastrado
            printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas); // Índice base 1 para o usuário
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

        // Valida as seleções e executa o ataque se forem válidas
        if (idx_atac >= 0 && idx_atac < territoriosCadastrados && idx_def >= 0 && idx_def < territoriosCadastrados) {
            if (strcmp(jogadores[jogadorAtual].cor, war[idx_atac].cor) != 0) { // Verifica se o território atacante pertence ao jogador atual
                printf("Ação inválida! Você só pode atacar com territórios da sua cor (%s).\n", jogadores[jogadorAtual].cor); 
            } else if (strcmp(war[idx_atac].cor, war[idx_def].cor) == 0) { // Verifica se o território defensor é aliado
                printf("Ação inválida! Não é possível atacar um território aliado.\n"); // Impede ataques entre territórios da mesma cor
            } else {
                atacar(&war[idx_atac], &war[idx_def]); // Chama a função de ataque
            }
        } else {
            printf("Seleção inválida! Território não existe.\n"); // Informa se a seleção foi inválida
        }
    }
    
    // Pausa para o usuário ler o resultado
    printf("Pressione ENTER para continuar...");
    getchar();
}


void atacar(struct Territorio* atacante, struct Territorio* defensor) { // Função para realizar o ataque entre territórios
    printf("\n------ --- Ataque Iniciado --- ------\n");
    printf("%s (%d tropas) ataca %s (%d tropas)!\n", atacante->nome, atacante->tropas, defensor->nome, defensor->tropas); // Exibe os detalhes do ataque
    printf("--------------------------------------\n");

    // Validações iniciais
    if (atacante->tropas <= 1) {
        printf("Ataque falhou! %s precisa ter mais de 1 tropa para atacar.\n", atacante->nome);
        return;
    }
    
    if (defensor->tropas <= 0) {
        printf("Ataque falhou! O %s já não possui tropas para defender.\n", defensor->nome);
        return;
    }

    int dadoAtaque = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadoDefesa = (rand() % 6) + 1; 

    // Exibe os resultados dos dados
    printf("Jogo de dados: %s rolou %d | %s rolou %d\n", atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);
    
    if (dadoAtaque > dadoDefesa) { // Ataque bem-sucedido
        defensor->tropas--; // Remove uma tropa do defensor
        printf("Ataque bem-sucedido! %s perdeu 1 tropa.\n", defensor->nome); 

        if (defensor->tropas == 0) { // Verifica se o defensor foi derrotado
            printf("--------------------------------------\n");
            printf("TERRITÓRIO CONQUISTADO!\n");
            
            strcpy(defensor->cor, atacante->cor); // Atualiza a cor do território conquistado
            printf("%s agora pertence à cor %s.\n", defensor->nome, defensor->cor); 

            int tropas_para_mover = atacante->tropas / 2; // Move metade das tropas do atacante para o defensor
            if (tropas_para_mover == 0) { // Garante que pelo menos 1 tropa seja movida
                tropas_para_mover = 1; // Move pelo menos 1 tropa
            }

            printf("Transferindo %d tropas de %s para %s.\n", tropas_para_mover, atacante->nome, defensor->nome); // Exibe a transferência de tropas
            
            defensor->tropas = tropas_para_mover; // Atualiza as tropas do defensor
            atacante->tropas -= tropas_para_mover; // Atualiza as tropas do atacante
        }
    } else {
        atacante->tropas--; // Remove uma tropa do atacante
        printf("Defesa bem-sucedida! %s perdeu 1 tropa.\n", atacante->nome);   // Ataque falhou
    }
    
    printf("\n--- Tropas restantes ---\n");
    printf(" -> %s: %d\n", atacante->nome, atacante->tropas); 
    printf(" -> %s: %d\n", defensor->nome, defensor->tropas);
    printf("--- FIM DO ATAQUE ---\n");
}

void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int numJogadores) { // Função para liberar memória alocada
    printf("\nLiberando toda a memória alocada...\n");

    if (mapa != NULL) { // Verifica se o mapa foi alocado
        free(mapa); // Libera a memória do mapa
    }
    if (jogadores != NULL) { // Verifica se os jogadores foram alocados
         // Libera a memória de cada missão alocada para os jogadores
        for (int i = 0; i < numJogadores; i++) { // Itera sobre cada jogador
            if (jogadores[i].missao != NULL) { // Verifica se a missão foi alocada
                free(jogadores[i].missao); // Libera a memória da missão
                jogadores[i].missao = NULL; // Evita ponteiros pendentes
            }
        }
        free(jogadores); // Libera a memória dos jogadores
    }
    printf("Memória liberada com sucesso.\n");
}

void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) { // Função para atribuir uma missão aleatória a um jogador
    int indiceSorteado = rand() % totalMissoes; // Sorteia um índice aleatório
    const char* missaoEscolhida = missoes[indiceSorteado]; // Seleciona a missão correspondente

    *destino = (char*) malloc(strlen(missaoEscolhida) + 1); // Aloca memória para a missão
    if (*destino == NULL) { // Verifica se a alocação foi bem-sucedida
        printf("Erro fatal: Falha ao alocar memória para a missão!\n");
        exit(1);// Encerra o programa em caso de erro
    }
    strcpy(*destino, missaoEscolhida); // Copia a missão para o destino
}

// Função para verificar se a missão foi cumprida
int verificarMissao(const char* missao, const char* corDoJogador, struct Territorio* mapa, int territoriosCadastrados) {
    // lógica simples inicial para verificação das missões
    if (strcmp(missao, "CONQUISTAR 3 TERRITORIOS") == 0) { 
        int contador = 0; 
        for (int i = 0; i < territoriosCadastrados; i++) { // Conta quantos territórios o jogador possui
            if (strcmp(mapa[i].cor, corDoJogador) == 0) { // Compara a cor do território com a cor do jogador
                contador++; // Incrementa o contador se as cores coincidirem
            }
        }
        return (contador >= 3); // Retorna verdadeiro se o jogador possuir 3 ou mais territórios
    }
    else if (strcmp(missao, "ELIMINAR O EXERCITO VERMELHO") == 0) {
        for (int i = 0; i < territoriosCadastrados; i++) { // Verifica se ainda existe algum território vermelho
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {  // Se encontrar um território vermelho, a missão não foi cumprida
                return 0; // Retorna falso se encontrar um território vermelho
            }
        }
        return 1; // Retorna verdadeiro se nenhum território vermelho for encontrado
    }
    else if (strcmp(missao, "ACUMULAR 12 TROPAS") == 0) { // Verifica se o jogador acumulou 12 ou mais tropas
        int somaTropas = 0;
        for (int i = 0; i < territoriosCadastrados; i++) { // Soma as tropas dos territórios do jogador
            if (strcmp(mapa[i].cor, corDoJogador) == 0) {
                somaTropas += mapa[i].tropas; //soma as tropas do território do jogador
            }
        }
        return (somaTropas >= 12); // Retorna verdadeiro se a soma for 12 ou mais
    }
    else if (strcmp(missao, "CONQUISTAR O TERRITORIO BRASIL") == 0) { // Verifica se o jogador conquistou o território Brasil
        for (int i = 0; i < territoriosCadastrados; i++) { // Procura pelo território Brasil
            if (strcmp(mapa[i].nome, "Brasil") == 0) { // Se encontrar o território Brasil, verifica se pertence ao jogador
                return (strcmp(mapa[i].cor, corDoJogador) == 0); // Retorna verdadeiro se o território Brasil pertence ao jogador
            }
        }
        return 0; // Retorna falso se o território Brasil não for encontrado
    }
    else if (strcmp(missao, "TER UM TERRITORIO COM 8 TROPAS") == 0) {
        for (int i = 0; i < territoriosCadastrados; i++) { // Verifica se o jogador possui algum território com 8 ou mais tropas
            if (strcmp(mapa[i].cor, corDoJogador) == 0 && mapa[i].tropas >= 8) { // Se encontrar, a missão foi cumprida
                return 1; // Retorna verdadeiro
            }
        }
        return 0; // Retorna falso se nenhum território com 8 ou mais tropas for encontrado
    }
    return 0; // Retorna falso se a missão não for reconhecida
}

void exibirMissao(const char* missao) {  // Função para exibir a missão do jogador
    printf("Sua missão secreta é: \"%s\"\n", missao); 
}

// Função para cadastrar jogadores
void cadastrarJogadores(struct Jogador* jogadores, int numJogadores, const char* missoes[], int totalMissoes) { 
    printf("\n--- CADASTRO DE JOGADORES ---\n");
    for (int i = 0; i < numJogadores; i++) { // Itera para cada jogador
        printf("\n--- Jogador %d ---\n", i + 1); // Exibe o número do jogador atual
        
        printf("Digite o nome do Jogador %d: ", i + 1); 
        fgets(jogadores[i].nome, 50, stdin); // Lê o nome do jogador
        // Remove o caractere de nova linha, se presente
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        int corValida = 0;
        while (!corValida) { // Loop até o jogador escolher uma cor válida
            printf("Digite a cor do Jogador %d (ex: Azul, Verde): ", i + 1); // Solicita a cor do jogador
            fgets(jogadores[i].cor, 10, stdin); // Lê a cor do jogador
            jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0'; // Remove o caractere de nova linha, se presente

            corValida = 1;
            for (int j = 0; j < i; j++) {// Verifica se a cor já foi escolhida por outro jogador
                if (strcmp(jogadores[j].cor, jogadores[i].cor) == 0) { // Compara as cores
                    printf("Erro: A cor '%s' já foi escolhida. Tente outra.\n", jogadores[i].cor); // Informa o jogador sobre a cor já escolhida
                    corValida = 0;
                    break;
                }
            }
        }
        
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes); // atribui uma missão ao jogador
        exibirMissao(jogadores[i].missao); // Exibe a missão do jogador
    }
}

// Função para cadastrar territórios
void cadastrarTerritorio(struct Territorio* war, int* territoriosCadastrados, int totalTerritorios) {
    if (*territoriosCadastrados >= totalTerritorios) { // Verifica se a capacidade máxima foi atingida
        printf("Erro: Capacidade máxima de territórios atingida (%d).\n", totalTerritorios); // Informa o usuário
        return; 
    }

    struct Territorio* novoTerritorio = &war[*territoriosCadastrados]; // Aponta para o próximo território disponível

    printf("\n--- Cadastro do Território %d ---\n", *territoriosCadastrados + 1); // Exibe o número do território sendo cadastrado
    
    printf("Nome do Território: "); 
    fgets(novoTerritorio->nome, 30, stdin); // Lê o nome do território
    novoTerritorio->nome[strcspn(novoTerritorio->nome, "\n")] = '\0'; // Remove o caractere de nova linha, se presente

    printf("Cor do Território (ex: Azul, Verde): ");
    fgets(novoTerritorio->cor, 10, stdin);
    novoTerritorio->cor[strcspn(novoTerritorio->cor, "\n")] = '\0';

    printf("Número inicial de tropas (mínimo 1): "); 
    int tropas;
    while (1) { // Loop até o usuário fornecer uma entrada válida
        if (scanf("%d", &tropas) != 1 || tropas < 1) { // Verifica se a entrada é um número inteiro maior ou igual a 1
            printf("Entrada inválida! Digite um número inteiro maior ou igual a 1: ");
            limparBufferEntrada();
        } else {
            break; // Entrada válida, sai do loop
        }
    }
    limparBufferEntrada();
    novoTerritorio->tropas = tropas; // Define o número de tropas do território

    (*territoriosCadastrados)++; // Incrementa o contador de territórios cadastrados
    printf("Território '%s' cadastrado com sucesso!\n", novoTerritorio->nome); // Confirma o cadastro do território
}

void listarTerritorios(struct Territorio* war, int territoriosCadastrados) { // Função para listar territórios
    printf("\n--- Lista de Territórios Cadastrados ---\n"); 
    if (territoriosCadastrados == 0) {// Verifica se há territórios cadastrados
        printf("Nenhum Território Cadastrado!\n");
    } else { 
        for (int i = 0; i < territoriosCadastrados; i++) {  // Itera sobre os territórios cadastrados
            printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas); // Lista cada território cadastrado
        }
    }
}

//========================= FIM DAS FUNÇÕES =========================//
