#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

//construção da struct que armazenará os territórios
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

//Armazena os dados de cada jogador
struct Jogador {
    char nome[50];
    char cor[10];
    char* missao; // Ponteiro para a missão alocada dinamicamente
};


//Limpar o Buffer de Entrada após o usuário apertar o ENTER
void limparBufferEntrada(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu(){
    //menu de interface do usuário
        printf("==========================\n");
        printf("------ JOGO WAR ----------\n");
        printf("==========================\n");
        printf("1. Cadastrar Território\n");
        printf("2. Listar Territórios Cadastrados\n");
        printf("3. Jogar\n");
        printf("0. Sair\n");
        printf("---------------------------\n");
}

void cadastrarTerritorio(){
    //opção de cadastrar novo território
    printf("================================\n");
    printf("Cadastrar Novo Território\n");
    printf("\n");
}

void listarTerritorios(){
    //mostra a lista de terrirórios 
    printf("\n");
    printf("--------------------------------------\n");
    printf("---Lista de Territórios Cadastrados---\n");
    printf("--------------------------------------\n");
    printf("\n");
}
//função para atacar territórios
void atacar(struct Territorio* atacante, struct Territorio* defensor){
    //função para realizar o ataque entre territórios
    printf("\n------ --- Ataque Iniciado --- ------\n");
    printf("Território Atacante: %s\n", atacante->nome);
    printf("Território Defensor: %s\n", defensor->nome);
    printf("--------------------------------------\n");

    printf("%s está atacando %s!\n", atacante->nome, defensor->nome);
    //verifica se o território atacante tem mais de 1 tropa
    //VERIFICAR ESSA LÓGICA PARA SABER O MÍNIMO DE TROPAS E QUANDO O USUÁRIO PERDE
    if(atacante->tropas <=1){
        printf("Ataque falhou! O %s precisa ter mais de 1 tropa para atacar.\n", atacante->nome);
        return;
    }
    //verifica se o território defensor tem pelo menos 1 tropa
    if(defensor->tropas <=0){
        printf("Ataque falhou! O %s não tem tropas para defender.\n", defensor->nome);
        return;
    }
    int dadoAtaque = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadoDefesa = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6

    printf("Jogo de dados:\n %s rolou %d\n%s rolou %d\n", atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa){
        defensor->tropas -= 1;
        printf("Ataque bem-sucedido! %s perdeu 1 tropa. Tropas restantes: %d\n", defensor->nome, defensor->tropas);
        printf("--- FIM DO ATAQUE ---\n");

        //Lógica de conquista do território
        if (defensor->tropas ==0){
            printf("TERRITÓRIO CONQUISTADO!\n");
            printf("%s foi conquistado por %s!\n", defensor->nome, atacante->nome);
            
            // 1. Muda a cor do território usando strcpy (string copy) - cópia segura de strings
            strcpy(defensor->cor, atacante->cor);
            printf("%s agora pertence à cor %s.\n", defensor->nome, defensor->cor);

            // 2. Move metade das tropas restantes do atacante
            // O atacante deve deixar pelo menos 1 tropa para trás.
            int tropas_para_mover = atacante->tropas / 2;
            if (tropas_para_mover == 0) {
                 tropas_para_mover = 1; // Garante que pelo menos 1 tropa se mova
            }

            printf("Transferindo %d tropas de %s para %s.\n", tropas_para_mover, atacante->nome, defensor->nome);
            
            defensor->tropas = tropas_para_mover; // Define as tropas do defensor como as tropas movidas
            atacante->tropas -= tropas_para_mover; // Reduz as tropas do atacante
        }
        // Fim da lógica de conquista do território 

    } else {
        atacante->tropas -= 1;
        printf("Defesa bem-sucedida! %s perdeu 1 tropa. Tropas restantes: %d\n", atacante->nome, atacante->tropas); //atacante perde 1 tropa
        printf("--- FIM DO ATAQUE ---\n");

    }
    
    printf("--------------------------------------\n");
    
}
//função para liberar a memória alocada
/**
 * @brief Libera toda a memória alocada dinamicamente durante o jogo.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param jogadores Ponteiro para o vetor de jogadores.
 * @param numJogadores O número total de jogadores.
 */
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int numJogadores) {
    printf("\nLiberando toda a memória alocada...\n");

    // 1. Libera o mapa de territórios
    free(mapa);

    // 2. Libera a string da missão de cada jogador
    for (int i = 0; i < numJogadores; i++) {
        free(jogadores[i].missao);
    }

    // 3. Libera o vetor de jogadores em si
    free(jogadores);

    printf("Memória liberada com sucesso.\n");
}


/**
 * Sorteio da missão usa a passagem por referência para o destino
 * Sorteia uma missão do vetor e a copia para o ponteiro de destino,
 * alocando a memória necessária.
 */
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) { // Passagem por referência para o ponteiro destino
    // Gera um índice aleatório para selecionar uma missão
    int indiceSorteado = rand() % totalMissoes; // Gera um número entre 0 e totalMissoes-1
    const char* missaoEscolhida = missoes[indiceSorteado]; // Seleciona a missão sorteada

    //Armazenamento dinâmico da missão usando malloc
    *destino = (char*) malloc(strlen(missaoEscolhida) + 1); // +1 para o '\0'
    if (*destino == NULL) { // Verificação de erro na alocação
        printf("Erro fatal: Falha ao alocar memória para a missão!\n"); 
        exit(1);
    }
    strcpy(*destino, missaoEscolhida); // Copia a missão para o destino
}

/**
 * Verificação da missão
 * Avalia se a condição da missão de um jogador foi cumprida.
 * Retorna 1 se cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const char* corDoJogador, struct Territorio* mapa, int totalTerritorios) {
    // Exemplo de verificação para as missões fornecidas
    if (strcmp(missao, "CONQUISTAR 3 TERRITORIOS") == 0) { 
        int contador = 0; // Inicializa o contador de territórios do jogador
        for (int i = 0; i < totalTerritorios; i++) { // Percorre os territórios do mapa
            if (strcmp(mapa[i].cor, corDoJogador) == 0) { // Verifica se o território pertence ao jogador
                contador++; // Incrementa o contador se o território pertence ao jogador
            }
        }
        return (contador >= 3); // Retorna 1 (verdadeiro) se contador for >= 3, senão 0
    }

    else if (strcmp(missao, "ELIMINAR O EXERCITO VERMELHO") == 0) {
        for (int i = 0; i < totalTerritorios; i++) { // Percorre os territórios do mapa
            if (strcmp(mapa[i].cor, "Vermelho") == 0) { // Verifica se ainda existe algum território Vermelho
                return 0; // Se encontrar algum território Vermelho, missão não cumprida
            }
        }
        return 1; // Se o loop terminar, não há mais territórios Vermelhos
    }

    else if (strcmp(missao, "ACUMULAR 12 TROPAS") == 0) {
        int somaTropas = 0; // Inicializa a soma das tropas
        for (int i = 0; i < totalTerritorios; i++) { // Percorre todos os territórios
            if (strcmp(mapa[i].cor, corDoJogador) == 0) { // Verifica se o território pertence ao jogador
                somaTropas += mapa[i].tropas; // Adiciona as tropas do território à soma
            }
        }
        return (somaTropas >= 12); // Retorna 1 se a soma for >= 12, senão 0
    }

    else if (strcmp(missao, "CONQUISTAR O TERRITORIO BRASIL") == 0) { 
        for (int i = 0; i < totalTerritorios; i++) { 
            // Primeiro, encontra o território com o nome "Brasil"
            if (strcmp(mapa[i].nome, "Brasil") == 0) {
                // Depois, verifica se a cor dele é a do jogador
                // Se for, a missão está cumprida.
                return (strcmp(mapa[i].cor, corDoJogador) == 0);
            }
        }
        return 0; // Retorna 0 se o "Brasil" não for encontrado no mapa
    }

    else if (strcmp(missao, "TER UM TERRITORIO COM 8 TROPAS") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {// Percorre todos os territórios
            // Verifica se o território pertence ao jogador E se tem tropas suficientes
            if (strcmp(mapa[i].cor, corDoJogador) == 0 && mapa[i].tropas >= 8) { 
                return 1; // Encontrou um território que cumpre a condição, missão completa!
            }
        }
        return 0; // Se o loop terminar, nenhum território do jogador tinha 8 tropas.
    }

    return 0; // Missão não reconhecida ou não cumprida
}

/**
 *Passagem por valor para exibição da missão
 * Exibe a missão de um jogador.
 */
void exibirMissao(const char* missao) { // Passagem por valor, pois não precisamos modificar a missão
    printf("Sua missão secreta é: \"%s\"\n", missao); // Exibe a missão
}


void cadastrarJogadores(struct Jogador* jogadores, int numJogadores, const char* missoes[], int totalMissoes) {
    printf("\n--- CADASTRO DE JOGADORES ---\n");
    for (int i = 0; i < numJogadores; i++) {
        printf("\n--- Jogador %d ---\n", i + 1);
        
        printf("Digite o nome do Jogador %d: ", i + 1);
        fgets(jogadores[i].nome, 50, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        // Loop para garantir que a cor seja única
        int corValida = 0;
        while (!corValida) {
            printf("Digite a cor do Jogador %d (ex: Azul, Verde): ", i + 1);
            fgets(jogadores[i].cor, 10, stdin);
            jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

            corValida = 1; // Assume que a cor é válida
            // Verifica se algum jogador anterior já escolheu esta cor
            for (int j = 0; j < i; j++) {
                if (strcmp(jogadores[j].cor, jogadores[i].cor) == 0) {
                    printf("Erro: A cor '%s' já foi escolhida. Tente outra.\n", jogadores[i].cor);
                    corValida = 0; // Marca como inválida para repetir o pedido
                    break;
                }
            }
        }
        
        // Atribui e exibe a missão do jogador recém-cadastrado
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes);
        exibirMissao(jogadores[i].missao);
    }
}


//========================= FUNÇÃO PRINCIPAL =========================//
int main(){
    //para ler os caracteres como acento, cedilha etc...
    setlocale(LC_ALL, "Portuguese");

    srand(time(NULL)); // Inicializa a semente de números aleatórios

    //Vetor de missões disponíveis
    const char* missoes[] = {
        "CONQUISTAR 3 TERRITORIOS",
        "ELIMINAR O EXERCITO VERMELHO",
        "ACUMULAR 12 TROPAS",
        "CONQUISTAR O TERRITORIO BRASIL",
        "TER UM TERRITORIO COM 8 TROPAS"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Definição dinâmica dos jogadores com base na entrada do usuário.
    int numJogadores = 0;
    printf("Quantos jogadores irão participar (ex: 2)? ");
    scanf("%d", &numJogadores);
    limparBufferEntrada();

    // Alocação dinâmica para o vetor de jogadores
    struct Jogador* jogadores = (struct Jogador*) calloc(numJogadores, sizeof(struct Jogador));
    if (jogadores == NULL) {
        printf("Erro fatal: Falha ao alocar memória para os jogadores!\n");
        return 1;
    }
    
    // Chamada da nova função de cadastro
    cadastrarJogadores(jogadores, numJogadores, missoes, totalMissoes);


    //solicita a capacidade total de territórios ao usuário
    int totalTerritorios = 0;
    printf("Digite o número máximo de territórios: ");
    scanf("%d", &totalTerritorios);

    //cria a struct na função principal vazia para receber o total de Territorios fornecida pelo usuário
    struct Territorio *war = NULL;

    //aloca o espaço na memória usando calloc para aumentar a segurança e evitar erros de buffer
    war = (struct Territorio *) calloc(totalTerritorios, sizeof(struct Territorio));

    //Verificação de segurança para mostrar se a memória foi alocada corretamente e caso não, exibir uma mensagem de erro.
    if (war == NULL){
        printf("Erro: Falha ao alocar memória!\n");
        return 1; //encerra o programa com erro.
    } 
    
    printf("Memória para %d territórios alocada e limpa com sucesso.\n", totalTerritorios);
    
    int opcao = 0;
    int turno = 0;
    int gameOver = 0;

    do
    {
        //chama a função para exibir o menu
        exibirMenu();
        printf("Escolha uma opção: ");
        
        // A lógica do turno agora usa a variável 'numJogadores'
        int jogadorAtual = turno % numJogadores;
        printf("\n======================================\n");
        // Exibe o nome e a cor do jogador
        printf("TURNO DE: %s (Cor: %s)\n", jogadores[jogadorAtual].nome, jogadores[jogadorAtual].cor);
        printf("======================================\n");

        
        // Exibir mapa
        for (int i = 0; i < totalTerritorios; i++) {
            printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas);
        }

        // Menu de ações
        printf("\nEscolha uma ação:\n1. Atacar\n2. Passar o turno\n0. Sair do Jogo\nOpção: ");
        int opcao;
        scanf("%d", &opcao);
        limparBufferEntrada();

        //lê o dado de entrada
        scanf("%d", &opcao);
        //limpa o buffer do ENTER para receber a próxima opção
        limparBufferEntrada();

        switch (opcao)
        {
        case 1:
            //chama a função para exibir o cabeçalho
            cadastrarTerritorio();

            //condição para saber se a constante definida anteriormente excedeu o limite de territórios
            //caso tenha espaço é permitido cadastrar novos territórios
            for (int i = 0; i < totalTerritorios; i++)
            {
                printf("Território %d\n", i+1);
                printf("Nome: ");
                //função fgets para ler as strings possui mais segurança do que a scanf e evita o buffer overflow
                fgets(war[i].nome, sizeof(war[i].nome), stdin); 
                printf("Cor: ");
                fgets(war[i].cor, sizeof(war[i].cor), stdin);
                
                //remove o caractere de quebra de linha (\n) do final das strings.
                war[i].nome[strcspn(war[i].nome, "\n")] = '\0';
                war[i].cor[strcspn(war[i].cor, "\n")] = '\0';

                printf("Total de Tropas: ");
                scanf("%d", &war[i].tropas);

                limparBufferEntrada();

                printf("Território Cadastrado!\n");
                printf("\n");

            } //fim do for
            
            printf("Pressione ENTER para continuar...");
            printf("\n");
            getchar(); //pausa para o usuário ler a mensagem antes de voltar ao menu
            break;

        case 2:
            //chama a função para exibir o cabeçalho
            listarTerritorios();
            if (totalTerritorios == 0) {
                printf("Nenhum Território Cadastrado!\n");
                printf("\n");
            } else {
                //percorre cada território inserido e mostra na tela
                for (int i = 0; i < totalTerritorios; i++){
                    printf("-----------------------\n");
                    printf("\n");
                    printf("TERRITÓRIO %d\n", i+1);
                    printf(" - Nome: %s\n", war[i].nome);
                    printf(" - Exército: %s\n", war[i].cor);
                    printf(" - Total de Tropas: %d\n", war[i].tropas);
                    printf("\n");
                }
                printf("-----------------------\n");
                printf("\n");
            }
            printf("Pressione ENTER para continuar...");
            printf("\n");
            getchar(); //pausa para o usuário ler a mensagem antes de voltar ao menu
            break;

        case 3:
            //Ataque
            
            int id_ataque, id_defesa;//variaveis para receber as escolhas do usuário
            printf("============================\n");
            printf("MAPA DO MUNDO - ESTADO ATUAL\n");
            printf("============================\n");
            //imprime os territórios para os usuários escolherem
            for (int i = 0; i < totalTerritorios; i++) {
                printf("%d: %s (Cor: %s, Tropas: %d)\n", i + 1, war[i].nome, war[i].cor, war[i].tropas);
            }//fim do for
            printf("---------------------------\n");
            
            //recebe os dados de entrada do usuário
            printf("Digite o número do território ATACANTE: ");
            scanf("%d", &id_ataque);
            limparBufferEntrada();

            printf("Digite o número do território DEFENSOR: ");
            scanf("%d", &id_defesa);
            limparBufferEntrada();

            //verifica se os dados inseridos são válidos
            //Se o id_ataque e id_defesa estiverem dentro do intervalo  não forem iguais e forem de cores diferentes, chama a função atacar
            if(id_ataque > 0 && id_ataque <= totalTerritorios && id_defesa > 0 && id_defesa <= totalTerritorios && id_ataque != id_defesa && (strcmp(war[id_ataque - 1].cor, war[id_defesa - 1].cor) != 0)){
                atacar(&war[id_ataque - 1], &war[id_defesa - 1]);//transforma a escolha do usuário para o índice correto do array e chama a função atacar
            }                         
            else{
                printf("Opção Inválida!");
            }

            printf("Pressione ENTER para continuar...");
            getchar();

            break;

        case 0:
            //sair do sistema
            printf("Saindo do Jogo...");
            break;
        
        default:
            //Caso o usuário digite uma opção inválida
            printf("Opção inválida! Tente Novamente.\n");
            printf("Pressione ENTER para Continuar...");
            getchar();
            break;
        }
        // A mensagem de vitória agora usa o nome do jogador
        if (!gameOver) {
            for (int i = 0; i < numJogadores; i++) {
                if (verificarMissao(jogadores[i].missao, jogadores[i].cor, war, totalTerritorios)) {
                    printf("\n\n!!! FIM DE JOGO !!!\n");
                    printf("O jogador %s (Cor: %s) cumpriu sua missão: \"%s\"\n", jogadores[i].nome, jogadores[i].cor, jogadores[i].missao);
                    printf("PARABÉNS!\n");
                    gameOver = 1;
                    break;
                }
            }
        }
        // Incrementa o turno apenas se o jogo não terminou
        turno++;


    } while (opcao != 0 && !gameOver);

    //libera a memória alocada para evitar vazamentos de memória
    liberarMemoria(war, jogadores, numJogadores);
    
    return 0;

}
//========================= FIM DA FUNÇÃO PRINCIPAL =========================//


