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

void atacar(struct Territorio* atacante, struct Territorio* defensor){
    //função para realizar o ataque entre territórios
    printf("\n------ --- Ataque Iniciado --- ------\n");
    printf("Território Atacante: %s\n", atacante->nome);
    printf("Território Defensor: %s\n", defensor->nome);
    printf("--------------------------------------\n");

    printf("%s está atacando %s!\n", atacante->nome, defensor->nome);

    if(atacante->tropas <=1){
        printf("Ataque falhou! O %s deve ter mais de 1 tropa.\n", atacante->nome);
        return;
    }
    
    int dadoAtaque = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6
    int dadoDefesa = (rand() % 6) + 1; // Gera um número aleatório entre 1 e 6

    printf("Jogo de dados: %s rolou %d\n, %s rolou %d\n", atacante->nome, dadoAtaque, defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa){
        defensor->tropas -= 1;
        printf("Ataque bem-sucedido! %s perdeu 1 tropa. Tropas restantes: %d\n", defensor->nome, defensor->tropas);
        printf("--- FIM DO ATAQUE ---\n");

    } else {
        atacante->tropas -= 1;
        printf("Defesa bem-sucedida! %s perdeu 1 tropa. Tropas restantes: %d\n", atacante->nome, atacante->tropas);
        printf("--- FIM DO ATAQUE ---\n");

    }
    if (defensor->tropas <= 0){
        printf("%s foi conquistado por %s!\n", defensor->nome, atacante->nome);
        defensor->tropas = 1; // O território conquistado começa com 1 tropa
        atacante->tropas -= 1; // O atacante deve mover uma tropa para o território conquistado
    }  
    printf("--------------------------------------\n");
    
}

int main(){
    //para ler os caracteres como acento, cedilha etc...
    setlocale(LC_ALL, "Portuguese");

    srand(time(NULL)); // Inicializa a semente de números aleatórios

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

    do
    {
        //chama a função para exibir o menu
        exibirMenu();
        printf("Escolha uma opção: ");
        
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
            int id_ataque, id_defesa;
            printf("============================\n");
            printf("MAPA DO MUNDO - ESTADO ATUAL\n");
            printf("============================\n");
            //imprime os territórios para os usuários escolherem
            for (int i = 0; i < totalTerritorios; i++) {
                printf("%d: %s (%d tropas)\n", i + 1, war[i].nome, war[i].tropas);
            }
            printf("---------------------------\n");
            
            printf("Digite o número do território ATACANTE: ");
            scanf("%d", &id_ataque);
            limparBufferEntrada();

            printf("Digite o número do território DEFENSOR: ");
            scanf("%d", &id_defesa);
            limparBufferEntrada();

            if(id_ataque > 0 && id_ataque <= totalTerritorios && id_defesa > 0 && id_defesa <= totalTerritorios && id_ataque != id_defesa){
                atacar(&war[id_ataque - 1], &war[id_defesa - 1]);
            } else{
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

    } while (opcao != 0);

    //libera a memória alocada para evitar vazamentos de memória
    free(war);
    
    return 0;

}
