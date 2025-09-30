#include <stdio.h>
#include <string.h>
#include <locale.h>

//constante para definir o número máximo de territórios
#define MAX_TERR 5 

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

int main(){
    //para ler os caracteres como acento, cedilha etc...
    setlocale(LC_ALL, "Portuguese");

    //cria a struct na função principal
    struct Territorio war[MAX_TERR];

    int totalTerritorios = 0;
    int opcao = 0;

    do
    {
        //menu de interface do usuário
        printf("=======================\n");
        printf("------ JOGO WAR -------\n");
        printf("=======================\n");
        printf("1. Cadastrar Território\n");
        printf("2. Listar Territórios Cadastrados\n");
        printf("0. Sair\n");
        printf("-----------------------\n");
        printf("Escolha uma opção: ");
        
        //lê o dado de entrada
        scanf("%d", &opcao);
        //limpa o buffer do ENTER para receber a próxima opção
        limparBufferEntrada();

        switch (opcao)
        {
        case 1:
            //opção de cadastrar novo território
            printf("=======================\n");
            printf("Cadastrar Novo Território\n");
            printf("\n");

            //condição para saber se a constante definida anteriormente excedeu o limite de territórios
            //caso tenha espaço é permitido cadastrar novos territórios
            if (totalTerritorios < MAX_TERR)
            {
                printf("Nome: ");
                //função fgets para ler as strings possui mais segurança do que a scanf e evita o buffer overflow
                fgets(war[totalTerritorios].nome, sizeof(war[totalTerritorios].nome), stdin); 
                printf("Cor: ");
                fgets(war[totalTerritorios].cor, sizeof(war[totalTerritorios].cor), stdin);
                
                //remove o caractere de quebra de linha (\n) do final das strings.
                war[totalTerritorios].nome[strcspn(war[totalTerritorios].nome, "\n")] = '\0';
                war[totalTerritorios].cor[strcspn(war[totalTerritorios].cor, "\n")] = '\0';

                printf("Total de Tropas: ");
                scanf("%d", &war[totalTerritorios].tropas);

                limparBufferEntrada();

                totalTerritorios++; //incrementa a variável para inserção do próximo território

                printf("Território Cadastrado!\n");
                printf("\n");

            } else {
                //caso o totalTerritorios seja maior que o máximo
                printf("Não é possível cadastrar mais territórios.\n");
                printf("\n");
            }
            
            printf("Pressione ENTER para continuar...");
            printf("\n");
            getchar(); //pausa para o usuário ler a mensagem antes de voltar ao menu
            break;

        case 2:
            //mostra a lista de terrirórios 
            printf("\n");
            printf("--------------------------------------\n");
            printf("---Lista de Territórios Cadastrados---\n");
            printf("--------------------------------------\n");
            printf("\n");
            
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
    
    return 0;

}
