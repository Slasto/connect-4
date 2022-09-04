
///FORZA 4 PER 2 GIOCATORI\\\

#include <stdio.h>
#include <stdlib.h>
#define n 7

void iniztab(char table[][n]);
void stamptab(char table[][n]);
void turno(char table[][n],int y[],char* p);
void controllovittoria(char table[][n],int* win,char* p);

int main ()
{
	char table[n][n];//tabbela di giovo
	int  segno[n]= {5,5,5,5,5,5,5}; //segna dovre dovra mettere X o O in ogni colonna (5 perchè  in 6 stanno i numeri)
	int  win=-1; //flag ; 1 se win 1; 2 se vince 2,; 0 se non ci sono più spazzi disponibbili
	int flag=0; //flag X o O messi
	char p1 = 'X';
	char p2 = 'O';
	//preliminari
	iniztab(table);
	stamptab(table);

	do {
		//operazioni giocatore 1
		turno(table,segno,&p1);
		flag++;
		stamptab(table);
		controllovittoria(table, &win,&p1);
		//operazioni giocatore 2
		if(win==-1 && flag<42) { //se giocatore 1 vince...
			turno(table,segno,&p2);
			flag++;
			stamptab(table);
			controllovittoria(table,&win,&p2);
		}
	} while (win==-1 && flag<42);

	//chi ha vinto?
	if (win==0) {
		printf("****pareggio****\n");
	}

	if (win==1) {
		printf("****vince il giocatore 1****\n");
	}
	if (win==2) {
		printf("****vince il giocatore 2****\n");
	}
	return 0;
}

void iniztab(char table[][n])   //azzera la tabella
{
	for (int i = 0; i < n-1; i++) {
		for (int j = 0; j < n; j++) {
			table[i][j]='-';
		}
	}
	for (int i = 0; i < n; i++) {
		table[n-1][i]=49+i;
	}
};


void stamptab(char table[][n])   //stampa la tabella
{

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%c",table[i][j]);
		}
		printf("\n");
	}
};


void turno(char table[][n],int y[],char* p)
{
	int x;
	int temp;
	do {
		do {
			if(*p=='O') {
				printf("Giocatore 2 tocca a te! (inserisci la colonna): \n");
			}
			if(*p=='X') {
				printf("Giocatore 1 tocca a te! (inserisci la colonna): \n");
			}
			scanf("%d",&x);
			x--;    //mettera un n tra 1-7
			if (x<=-1 || x>=7) {
				printf("valore non valido...scegliere un compreso tra 1 e 7\n");
			}
		} while(x<=-1 || x>=7); //se o è minore di 0 o maggiore uguale a 7

		if (y[x]==-1) {
			printf("colonna piena...\n");
		}
	} while (y[x]==-1);//se vale -1 vuol dire che la casella 0 è gia occupata ///se y[j]=6 metto in 6 e diminuisco ...
	//se y[j]=5 metti in 5 e diminuisco... se vale 1 metto in 1 e dim se 0 metto in zero e diminuisco quindi -1

	temp=y[x];
	table[temp][x]=*p;
	y[x]--;
}

void controllovittoria(char table[][n], int* win,char* p)
{
	int win1=0;
	//RIGHE
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			//printf("%c", table[i][j]);        //elaborazione valore
			if(table[i][j]==*p) {
				win1++;
				if(*p=='X' && win1==4) {
					*win=1;   //vince il giocatore 1 ///ERRORE QUI
				}
				if(*p=='O' && win1==4) {
					*win=2;   //il giocatore 2 ha vinto
				}
			}
			if (table[i][j]!=*p) {
				win1=0;
			}
			//printf("%c", table[i][j]);
		}
		win1=0;
	}//printf("\t\t");
	printf("\n");

	if(*win==-1) {
		for(int j=0; j<n; j++) {
			for(int i=0; i<n; i++)    {
				//printf("%c", table[i][j]);        //elaborazione valore
				if(table[i][j]==*p) {
					win1++;
					if(*p=='X' && win1==4) {
						*win=1;   //vince il giocatore 1 ///ERRORE QUI
					}
					if(*p=='O' && win1==4) {
						*win=2;   //il giocatore 2 ha vinto
					}
				}
				if (table[i][j]!=*p) {
					win1=0;
				}
				//printf("%c", table[i][j]);
			}
			win1=0;
		}////printf("\t\t");

		if(*win==-1) {
			///DIAGONALI...
			int segnoP=0; //deve saltare la colonna numero 0 1 2 9 10 11 12 -> deve stampare la 3 4 5 6 7 8
			for(int e=1-n; e<=n-1; e++) {
				if(segnoP==3 || segnoP==4 || segnoP==5 ||segnoP==6 || segnoP==7 || segnoP==8) {
					for(int i=0; i<n; i++) {
						for(int j=0; j<n; j++) {
							if(i-j==e) {
								//condizone che il valore è presenta nella p diagonale
								//printf("%c", table[i][j]);        //elaborazione valore
								if(table[i][j]==*p) {
									win1++;
									if(*p=='X' && win1==4) {
										*win=1;   //vince il giocatore 1 ///ERRORE QUI
									}
									if(*p=='O' && win1==4) {
										*win=2;   //il giocatore 2 ha vinto
									}
								}
								if (table[i][j]!=*p) {
									win1=0;
								}
							}
						}
					}
				}
				segnoP++;
				win1=0;
				//printf("\n");
			}

			int segnoS=0;
			for(int f=0; f<=2*n; f++) {

				if(segnoS==3 || segnoS==4 || segnoS==5 ||segnoS==6 || segnoS==7 || segnoS==8) {
					for(int i=0; i<n; i++) {
						for(int j=0; j<n; j++) {
							if(i+j==f) {
								//condizone che il valore è presenta nella p diagonale
								//printf("%c", table[i][j]);       //elaborazione valore
								if(table[i][j]==*p) {
									win1++;
									if(*p=='X' && win1==4) {
										*win=1;   //vince il giocatore 1 ///ERRORE QUI
									}
									if(*p=='O' && win1==4) {
										*win=2;   //il giocatore 2 ha vinto
									}
								}
								if (table[i][j]!=*p) {
									win1=0;
								}
							}
						}
					}
				}
				segnoS++;
				win1=0;
				//printf("\n");
			}
		}
	}
}