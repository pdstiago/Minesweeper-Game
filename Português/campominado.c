#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

#define ANSI_COLOR_RED     	"\x1b[31m" 
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_PURPLE  "\x1b[35m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_CYAN  "\x1b[36m"

typedef struct{
    int mina, minasvizinhas, estado, flag;
} infotab;

typedef struct{
    char nome[100];
    int min, seg, total;
} infopessoas;

infotab tabuleiro[18][32];

infopessoas *pessoas1=NULL, *pessoas2=NULL, *pessoas3=NULL;

int linhas, colunas, abertos, numPessoas1, numPessoas2, numPessoas3;

int mini(int x, int y){
    return (x<y)?x:y;
}

int valido(int x, int y) {
    if(x==0 && y==0) return 1;
    if (x > linhas || y > colunas || x < 1 || y < 1) {
        printf("\nCoordenada inválida, a linha deve estar entre 1 e %d, e a coluna deve estar entre 1 e %d!\n", linhas, colunas);
        return 0;
    }

    if (tabuleiro[x][y].estado == 1) {
        printf("\nPosição já jogada, selecione outra posição!\n");
        return 0;
    }

    return 1;

}

void textoGanhou(){
	FILE *arq;
	char ganhou[128];
	
	if((arq=fopen("ganhou.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(ganhou, sizeof(ganhou), arq)!=NULL){
			printf("%s", ganhou);
		}
	}
}

void textoPerdeu(){
	FILE *arq;
	char perdeu[128];
	
	if((arq=fopen("perdeu.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(perdeu, sizeof(perdeu), arq)!=NULL){
			printf("%s", perdeu);
		}
	}
}


void textoTitulo(){
	FILE *arq;
	char titulo[128];
	
	if((arq=fopen("titulo.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(titulo, sizeof(titulo), arq)!=NULL){
			printf("%s", titulo);
        }
	}
}


void iniciaMinas(int cont){
   int x, y;
   srand(time(NULL));

   while(1){
        if(cont<1) break;
        x=rand()%linhas;
        y=rand()%colunas;
        x++;
        y++;
        if(!(tabuleiro[x][y].mina)){
            tabuleiro[x][y].mina=1;
            cont--;
        }
    }
}

void contaMinas(int row, int col){
    int i, j;

    for(i=-1; i<=1; i++){
        for(j=-1; j<=1; j++){
            int x=row+i;
            int y=col+j;
            if(x>=1 && x<=linhas && y>=1 && y<=colunas && tabuleiro[x][y].mina){
                tabuleiro[row][col].minasvizinhas++;
            }
        }
    }
}

void iniciaTabuleiro(int cont){
    int i, j;

    iniciaMinas(cont);

    for(i=1; i<=linhas; i++){
        for(j=1; j<=colunas; j++){
            if(!(tabuleiro[i][j].mina)){
                contaMinas(i, j);
            }
        }
    }
}

void limpaTabuleiro(){
    int i, j;

    for(i = 1; i <=linhas; i++){
        for(j = 1; j <=colunas; j++){
            tabuleiro[i][j].mina = 0;
            tabuleiro[i][j].estado = 0;
            tabuleiro[i][j].minasvizinhas = 0;
            tabuleiro[i][j].flag = 0;
        }
    }


}

void percorreTabuleiro(int row, int col){
    tabuleiro[row][col].estado=1;
    abertos++;

    if(!(tabuleiro[row][col].minasvizinhas)){
        int i, j;
        for(i=-1; i<=1; i++){
            for(j=-1; j<=1; j++){
                int x=row+i;
                int y=col+j;
                if(x>=1 && x<=linhas && y>=1 && y<=colunas && !(tabuleiro[x][y].mina) && !(tabuleiro[x][y].estado)){
                    percorreTabuleiro(x, y);
                }
            }
        }
    }
}

void imprimirTabuleiro(int cont, int difi){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");

    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].flag){
                printf("|");
                printf(ANSI_COLOR_RED " ! " ANSI_COLOR_RESET);
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

void perdeuTabuleiro(int cont, int difi, int x, int y){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");

    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].mina){
                printf("|");
                if(i==x && j==y){
                    printf(ANSI_COLOR_YELLOW " * " ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED " * " ANSI_COLOR_RESET);
                }
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

void ganhouTabuleiro(int difi){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");
    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].mina){
                printf("|");
                printf(ANSI_COLOR_RED " * " ANSI_COLOR_RESET);
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

int ganhou(int cont){
    if(abertos==linhas*colunas-cont) return 1;
    return 0;
}

void tempo(int x, int y, infopessoas *sla){
    int min=x/60;
    int seg=x-(min*60);
    if(y==1){
        sla->min=min;
        sla->seg=seg;
        sla->total=min*60+seg;
        if(x<60){
            printf("\n\nVocê venceu o Campo Minado em %ld segundo(s).\n", seg);
        }else{
            printf("\n\nVocê venceu o Campo Minado em %ld minuto(s) e %ld segundo(s).\n", min, seg);
        }
    }else if(y==2){
        if(x<60){
            printf("\n\nFaltaram %ld segundo(s) para você bater o seu recorde pessoal.\n", seg);
        }else{
            printf("\n\nFaltaram %ld minuto(s) e %ld segundo(s) para você bater o seu recorde pessoal.\n", min, seg);
        }
    }else{
        if(x<60){
            printf("\n\nO tempo da partida foi de %ld segundo(s).\n", seg);
        }else{
            printf("\n\nO tempo da partida foi de %ld minuto(s) e %ld segundo(s).\n", min, seg);
        }
    }
}

void ordenar(){
    int i, j;

    for(i=0; i<numPessoas1; i++){
        for(j=0; j<numPessoas1; j++){
            if(pessoas1[i].total<pessoas1[j].total){
                infopessoas temp=pessoas1[i];
                pessoas1[i]=pessoas1[j];
                pessoas1[j]=temp;
            }
        }
    }

    for(i=0; i<numPessoas2; i++){
        for(j=0; j<numPessoas2; j++){
            if(pessoas2[i].total<pessoas2[j].total){
                infopessoas temp=pessoas2[i];
                pessoas2[i]=pessoas2[j];
                pessoas2[j]=temp;
            }
        }
    }

    for(i=0; i<numPessoas3; i++){
        for(j=0; j<numPessoas3; j++){
            if(pessoas3[i].total<pessoas3[j].total){
                infopessoas temp=pessoas3[i];
                pessoas3[i]=pessoas3[j];
                pessoas3[j]=temp;
            }
        }
    }
}

void carregar(){
    int i=0;
    char buffer[256];
    char cont[256];
    infopessoas temp;

    FILE *arq=fopen("recordesfacil.txt", "r");
    if(arq==NULL){
        printf("Infelizmente, não foi possível carregar os recordes anteriores do nível fácil!\n\n");
    }

    fgets(buffer, 256, arq);

    fgets(buffer, 256, arq);

    while(fgets(cont, 256, arq)){
        fgets(temp.nome, 100, arq);
        fscanf(arq, "%d", &temp.min);
        fgets(buffer, 14, arq);
        fscanf(arq, "%d", &temp.seg);
        fgets(buffer, 13, arq);
        fgets(buffer, 256, arq);

        pessoas1=realloc(pessoas1, (i+1)*sizeof(infopessoas));
        
        pessoas1[i]=temp;
        pessoas1[i].total=pessoas1[i].min*60+pessoas1[i].seg;
        pessoas1[i].nome[strlen(pessoas1[i].nome)-1]='\0';
        i++;
    }
    numPessoas1=i;
    fclose(arq);

    i=0;

    arq=fopen("recordesmedio.txt", "r");
    if(arq==NULL){
        printf("Infelizmente, não foi possível carregar os recordes anteriores do nível medio!\n\n");
    }

    fgets(buffer, 256, arq);

    fgets(buffer, 256, arq);

    while(fgets(cont, 256, arq)){
        fgets(temp.nome, 100, arq);
        fscanf(arq, "%d", &temp.min);
        fgets(buffer, 14, arq);
        fscanf(arq, "%d", &temp.seg);
        fgets(buffer, 13, arq);
        fgets(buffer, 256, arq);

        pessoas2=realloc(pessoas2, (i+1)*sizeof(infopessoas));
        
        pessoas2[i]=temp;
        pessoas2[i].total=pessoas2[i].min*60+pessoas2[i].seg;
        pessoas2[i].nome[strlen(pessoas2[i].nome)-1]='\0';
        i++;
    }
    numPessoas2=i;
    fclose(arq);

    i=0;

    arq=fopen("recordesdificil.txt", "r");
    if(arq==NULL){
        printf("Infelizmente, não foi possível carregar os recordes anteriores do nível dificil!\n\n");
    }

    fgets(buffer, 256, arq);

    fgets(buffer, 256, arq);

    while(fgets(cont, 256, arq)){
        fgets(temp.nome, 100, arq);
        fscanf(arq, "%d", &temp.min);
        fgets(buffer, 14, arq);
        fscanf(arq, "%d", &temp.seg);
        fgets(buffer, 13, arq);
        fgets(buffer, 256, arq);

        pessoas3=realloc(pessoas3, (i+1)*sizeof(infopessoas));
        
        pessoas3[i]=temp;
        pessoas3[i].total=pessoas3[i].min*60+pessoas3[i].seg;
        pessoas3[i].nome[strlen(pessoas3[i].nome)-1]='\0';
        i++;
    }
    numPessoas3=i;
    fclose(arq);
}

void salvar(){
    int i=0;

    FILE *arq=fopen("recordesfacil.txt", "w");
    if(arq==NULL){
        printf("Infelizmente, não foi possível salvar os recordes fáceis!\n\n");
    }
    
    fprintf(arq, "Recordes Campo Minado Fácil\n\n");
    for(i=0; i<numPessoas1; i++){
        fprintf(arq, "%d.\n", i+1);
        fprintf(arq, "%s\n", pessoas1[i].nome);
        if(i==numPessoas1-1) fprintf(arq, "%d minuto(s) e %d segundo(s)", pessoas1[i].min, pessoas1[i].seg);
        else fprintf(arq, "%d minuto(s) e %d segundo(s)\n\n", pessoas1[i].min, pessoas1[i].seg);
    }
    fclose(arq);

    arq=fopen("recordesmedio.txt", "w+");
    if(arq==NULL){
        printf("Infelizmente, não foi possível salvar os recordes médios!\n\n");
    }
    
    fprintf(arq, "Recordes Campo Minado Médio\n\n");
    for(i=0; i<numPessoas2; i++){
        fprintf(arq, "%d.\n", i+1);
        fprintf(arq, "%s\n", pessoas2[i].nome);
        if(i==numPessoas2-1) fprintf(arq, "%d minuto(s) e %d segundo(s)", pessoas2[i].min, pessoas2[i].seg);
        else fprintf(arq, "%d minuto(s) e %d segundo(s)\n\n", pessoas2[i].min, pessoas2[i].seg);
    }
    fclose(arq);
    arq=fopen("recordesdificil.txt", "w+");
    if(arq==NULL){
        printf("Infelizmente, não foi possível salvar os recordes difíceis!\n\n");
    }
    
    fprintf(arq, "Recordes Campo Minado Difícil\n\n");
    for(i=0; i<numPessoas3; i++){
        fprintf(arq, "%d.\n", i+1);
        fprintf(arq, "%s\n", pessoas3[i].nome);
        if(i==numPessoas3-1) fprintf(arq, "%d minuto(s) e %d segundo(s)", pessoas3[i].min, pessoas3[i].seg);
        else fprintf(arq, "%d minuto(s) e %d segundo(s)\n\n", pessoas3[i].min, pessoas3[i].seg);
    }
    fclose(arq);
}

void criar(){
    FILE *arq;
    
    if((arq = fopen("recordesfacil.txt", "r+")) == NULL){
    	if((arq = fopen("recordesfacil.txt","w+")) == NULL){
	        puts("O arquivo de recordes de nível fácil não pode ser criado.\n");
        }
    }
    fclose(arq);

    if((arq = fopen("recordesmedio.txt", "r+")) == NULL){
    	if((arq = fopen("recordesmedio.txt","w+")) == NULL){
	        puts("O arquivo de recordes de nível médio não pode ser criado.\n");
        }
    }
    fclose(arq);

    if((arq = fopen("recordesdificil.txt", "r+")) == NULL){
    	if((arq = fopen("recordesdificil.txt","w+")) == NULL){
	        puts("O arquivo de recordes de nível difícil não pode ser criado.\n");
        }
    }
    fclose(arq);
}

int main(){

    time_t comeco, fim;
    int i, x, y, sa;
    int dificuldade, jogada, primeira=1, achei=0, comando;
    int qntFlags, minas;
    char jogar = 's', ctz;
    infopessoas atual;
    
    criar();

    carregar();

    ordenar();

    while(1){
        #ifdef _WIN32
        	system("cls");
    	#else 
        	system("clear");
    	#endif

        textoTitulo();
        printf("\nBem vindo ao famoso jogo \"Campo Minado\"!\n\n1 - Jogar\n2 - Leaderboard do nível Fácil\n3 - Leaderboard do nível Médio\n4 - Leaderboard do nível Difícil\n\n0 - Sair o jogo\n\nInsira o comando desejado: ");
    
        while(scanf("%d", &comando)==0 || comando < 0 || comando > 4){
            fflush(stdin);
            printf("\n\nComando inválido, selecione um número entre 0 e 4: ");
        }

        if(!comando){
            return 0;
        }

        switch(comando){
            case 1:
                jogar='s';
                while(jogar == 's' || jogar == 'S'){
        
                    #ifdef _WIN32
                        system("cls");
                    #else 
                        system("clear");
                    #endif

                    textoTitulo();
                    printf("\nAs dificuldades disponíveis são:\n\n1 - Fácil (tabuleiro 9x9 / 10 minas)\n2 - Médio (tabuleiro 16x16 / 40 minas)\n3 - Difícil (tabuleiro 16x30 / 99 minas)\n\n0 - Retornar ao Menu Principal\n\nInsira o comando desejado: ");

                    while(scanf("%d", &dificuldade)==0 || dificuldade < 0 || dificuldade > 3){
                        fflush(stdin);
                        printf("\n\nComando inválido, selecione um número entre 0 e 3: ");
                    }

                    if(!dificuldade){
                        break;
                    }
                    getchar();
                    printf("Insira seu nome: ");
                    fgets(atual.nome, 100, stdin);
                    atual.nome[strlen(atual.nome)-1]='\0';

                    switch(dificuldade){
                        case 1:
                            linhas=9, colunas=9, minas=qntFlags=10;
                            break;
                        case 2:
                            linhas=16, colunas=16, minas=qntFlags=40;
                            break;
                        case 3:
                            linhas=16, colunas=30, minas=qntFlags=99;
                            break;
                        default:
                            break;
                    }

                    primeira=1;
                    abertos=0;

                    limpaTabuleiro();
                    iniciaTabuleiro(minas);

                    while(1){
                        
                        #ifdef _WIN32
                            system("cls");
                        #else 
                            system("clear");
                        #endif

                        imprimirTabuleiro(qntFlags, dificuldade);

                        printf("\nJogadas:\n\n1 - Escavar\n2 - Inserir flag de mina\n3 - Remover flag de mina\n\n0 - Retornar ao Menu \"Jogar\"\n\nInsira o comando desejado: ");

                        while(scanf("%d", &jogada)==0 || jogada<0 || jogada>3){
                            fflush(stdin);
                            printf("\nComando inválido, selecione um número entre 0 e 3: ");
                        }

                        if(!jogada){
                            getchar();
                            printf("\nTem certeza que deseja voltar ao Menu \"Jogar\"? (S/N): ");
                            scanf("%c", &ctz);
                            getchar();

                            while(ctz!='S' && ctz!='s' && ctz!='N' && ctz!='n'){
                                printf("\nComando inválido, selecione as letras solicitadas: ");
                                scanf("%c", &ctz);
                                getchar();
                            }

                            if(ctz=='S' || ctz=='s') break;
                            else continue;
                        }

                        printf("\nInsira a linha e a coluna da sua jogada, caso queira cancelar a jogada insira \"0 0\": ");
                    
                        while(!(sa=scanf("%d %d", &x, &y)) || !(valido(x, y))){
                            fflush(stdin);

                            if(!sa){
                                printf("\n\nCoordenada inválida, a linha deve estar entre 1 e %d, e a coluna deve estar entre 1 e %d!\n", linhas, colunas);
                            }

                            printf("\nInsira a linha e a coluna da sua jogada, caso queira cancelar a jogada insira \"0 0\": ");
                        }

                        if(x==0 && y==0) continue;

                        if(jogada==1){
                            if(primeira){

                                while(tabuleiro[x][y].mina || tabuleiro[x][y].minasvizinhas){
                                    limpaTabuleiro();
                                    iniciaTabuleiro(minas);
                                }

                                comeco = time(NULL);
                                primeira=0;
                            }
                            if(tabuleiro[x][y].mina==1){
                                
                                #ifdef _WIN32
                                system("cls");
                                #else 
                                    system("clear");
                                #endif

                                perdeuTabuleiro(qntFlags, dificuldade, x, y);

                                fim = time(NULL);

                                textoPerdeu();
                                tempo(fim-comeco, 0, NULL);
                                getchar();
                                printf("\nVocê deseja jogar novamente? (S/N): ");
                                scanf("%c", &jogar);
                                getchar();

                                while(jogar!='S' && jogar!='s' && jogar!='N' && jogar!='n'){
                                    printf("\nComando inválido, selecione as letras solicitadas: ");
                                    scanf("%c", &jogar);
                                    getchar();
                                }
                                break;
                            }else{
                                percorreTabuleiro(x, y);
                            }
                        }

                        if(jogada==2){
                            if(qntFlags==0){
                                printf("\nNão há mais flags disponíveis, remova uma flag para acrescentar em outro lugar!\n\nEnter para continuar >>");
                                fflush(stdin);
                                getc(stdin);
                            }else{
                                if((tabuleiro[x][y].flag)){
                                    printf("\nJogada inválida, já existe uma flag nessa posição!\n\nEnter para continuar >>");
                                    fflush(stdin);
                                    getc(stdin);
                                }else{
                                    tabuleiro[x][y].flag=1;
                                    tabuleiro[x][y].estado=1;
                                    qntFlags--;
                                }
                            }
                        }

                        if(jogada==3){
                            if(qntFlags==minas){
                                printf("\nNão existem flags para serem removidas, acrescente uma flag primeiro, para depois retirá-la!\n\nEnter para continuar >>");
                                fflush(stdin);
                                getc(stdin);
                            }else{
                                if(!(tabuleiro[x][y].flag)){
                                    printf("\nNão existe uma flag nessa posição, acrescente uma flag primeiro, para depois retirá-la!\n\nEnter para continuar >>");
                                    fflush(stdin);
                                    getc(stdin);
                                }else{
                                    tabuleiro[x][y].flag=0;
                                    tabuleiro[x][y].estado=0;
                                    qntFlags++;
                                }
                            }
                        }
                        if(ganhou(minas)){
                            
                            #ifdef _WIN32
                            system("cls");
                            #else 
                                system("clear");
                            #endif

                            ganhouTabuleiro(dificuldade);

                            fim = time(NULL);

                            textoGanhou();
                            tempo(fim-comeco, 1, &atual);
                            getchar();
                            switch(dificuldade){
                                achei=0;
                                case 1:
                                    for(i=0; i<numPessoas1; i++){
                                        if(strcmp(pessoas1[i].nome, atual.nome) == 0){
                                            achei=1;
                                            if(pessoas1[i].total>(fim-comeco)){
                                                printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                                pessoas1[i]=atual;
                                            }else{
                                                tempo(((fim-comeco)-pessoas1[i].total), 2, NULL);
                                            }
                                        }
                                    }
                                    if(!achei){
                                        printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                        pessoas1=realloc(pessoas1, (numPessoas1+1)*sizeof(infopessoas));
                                        pessoas1[numPessoas1++]=atual;
                                    }
                                    break;
                                case 2:
                                    for(i=0; i<numPessoas2; i++){
                                        if(strcmp(pessoas2[i].nome, atual.nome) == 0){
                                            achei=1;
                                            if(pessoas2[i].total>(fim-comeco)){
                                                printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                                pessoas2[i]=atual;
                                            }else{
                                                tempo(((fim-comeco)-pessoas2[i].total), 2, NULL);
                                            }
                                        }
                                    }
                                    if(!achei){
                                        printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                        pessoas2=realloc(pessoas2, (numPessoas2+1)*sizeof(infopessoas));
                                        pessoas2[numPessoas2++]=atual;
                                    }
                                    break;
                                case 3:
                                    for(i=0; i<numPessoas3; i++){
                                        if(strcmp(pessoas3[i].nome, atual.nome) == 0){
                                            achei=1;
                                            if(pessoas3[i].total>(fim-comeco)){
                                                printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                                pessoas3[i]=atual;
                                            }else{
                                                tempo(((fim-comeco)-pessoas3[i].total), 2, NULL);
                                            }
                                        }
                                    }
                                    if(!achei){
                                        printf("\n\nParabéns %s você atingiu um novo recorde pessoal!\n", atual.nome);
                                        pessoas3=realloc(pessoas3, (numPessoas3+1)*sizeof(infopessoas));
                                        pessoas3[numPessoas3++]=atual;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            ordenar();
                            salvar();
                            switch (dificuldade){
                            case 1:
                                for(i=0; i<numPessoas1; i++){
                                    if(strcmp(pessoas1[i].nome, atual.nome) == 0){
                                        printf("\nVocê se encontra na %dº posição do ranking de recordes do nível Fácil!", i+1);
                                        break;
                                    }
                                }
                                break;
                            
                            case 2:
                                for(i=0; i<numPessoas2; i++){
                                    if(strcmp(pessoas2[i].nome, atual.nome) == 0){
                                        printf("\nVocê se encontra na %dº posição do ranking de recordes do nível Médio!", i+1);
                                        break;
                                    }
                                }

                            case 3:
                                for(i=0; i<numPessoas3; i++){
                                    if(strcmp(pessoas3[i].nome, atual.nome) == 0){
                                        printf("\nVocê se encontra na %dº posição do ranking de recordes do nível Difícil!", i+1);
                                        break;
                                    }
                                }

                            default:
                                break;
                            }
                            printf("\n\nVocê deseja jogar novamente? (S/N): ");
                            scanf("%c", &jogar);
                            getchar();

                            while(jogar!='S' && jogar!='s' && jogar!='N' && jogar!='n'){
                                printf("\nComando inválido, selecione as letras solicitadas: ");
                                scanf("%c", &jogar);
                            }
                            break;
                        }
                    }
                }
                break;
            case 2:
                #ifdef _WIN32
                    system("cls");
                #else 
                    system("clear");
                #endif

                textoTitulo();
                printf("\n\n=============================== Leaderboard Campo Minado Fácil ===============================\n\n");
                if(numPessoas1==0){
                    printf("Nenhum usuário ganhou o jogo no nível Fácil!\n");
                }else{
                    for(i=0; i<mini(numPessoas1, 5); i++){
                        printf("%d. ", i+1);
                        printf("%s\n", pessoas1[i].nome);
                        printf("%d minuto(s) e %d segundo(s)\n\n", pessoas1[i].min, pessoas1[i].seg);
                    }
                }
                printf("\nEnter para continuar >>");
                fflush(stdin);
                getc(stdin);
                break;
            case 3:
                #ifdef _WIN32
                    system("cls");
                #else 
                    system("clear");
                #endif

                textoTitulo();
                printf("\n\n=============================== Leaderboard Campo Minado Médio ===============================\n\n");
                if(numPessoas2==0){
                    printf("Nenhum usuário ganhou o jogo no nível Médio!\n");
                }else{
                    for(i=0; i<mini(numPessoas2, 5); i++){
                        printf("%d. ", i+1);
                        printf("%s\n", pessoas2[i].nome);
                        printf("%d minuto(s) e %d segundo(s)\n\n", pessoas2[i].min, pessoas2[i].seg);
                    }
                }
                printf("\nEnter para continuar >>");
                fflush(stdin);
                getc(stdin);
                break;
            case 4:
                #ifdef _WIN32
                    system("cls");
                #else 
                    system("clear");
                #endif

                textoTitulo();
                printf("\n\n=============================== Leaderboard Campo Minado Difícil ===============================\n\n");
                if(numPessoas3==0){
                    printf("Nenhum usuário ganhou o jogo no nível Difícil!\n");
                }else{
                    for(i=0; i<mini(numPessoas3, 5); i++){
                        printf("%d. ", i+1);
                        printf("%s\n", pessoas3[i].nome);
                        printf("%d minuto(s) e %d segundo(s)\n\n", pessoas3[i].min, pessoas3[i].seg);
                    }
                }
                printf("\nEnter para continuar >>");
                fflush(stdin);
                getc(stdin);
                break;
            default:
                break;
        }
    }
    return 0;
}
