#include <stdlib.h>
#include <gtk/gtk.h>
#include <pango/pango.h>
#include "textcolor.h"
#define n 7

///struttura principale

typedef struct ElementoDellaCasella {
	gchar label[2];             //etichetta (- X O), 2 elementi per problemi con i label
	int riga;                   //posizione y
	int colonna;                //posizione x
	GtkWidget* widget;          //rappresenta il bottone/lable
} casella;

//parte dichiarativa gtk

GtkWidget *win = NULL;    //finestra
GtkWidget *table = NULL; //tabella
GtkWidget *labelpun1 = NULL; //dice il punteggio del giocatore 1
GtkWidget *labelpun2 = NULL; //dice il punteggio del giocatore 2
GtkWidget *labelstatus = NULL;  //dice il punteggio del giocatore 1;
gchar Vuoto[]="Pedine/Vuoto.png";
gchar P1[]="Pedine/1.png";
gchar P2[]="Pedine/2.png";

//parte dichiarativa algoritmica

int  segno[n]= {5,5,5,5,5,5,5}; //segna dove dovrà mettere X o O in ogni colonna (5 perchè  in 6 stanno i numeri)
int  vittoria=-1; //flag ; 1 se win 1; 2 se vince 2,;
int  flag=0; //quanti X o O messi?
gchar turno[2]="X";//a chi tocca
casella elemento[n][n];
gchar punteggio1[14]="Giocatore 1: 0";
gchar punteggio2[14]="Giocatore 2: 0";

//Funzioni usate

void iniztab(void);
void mossa(GtkWidget* button, int* colonna);
void nuovapartita(void);
void controllovittoria(void);
static void colonnapiena(void);
static void fineparita(void);
/*
// cambio di colore per label

PangoAttrList  *impcolore2;
PangoAttrList  *impcolore1;
PangoAttrList  *impcoloreneutro;
*/
int main (int argc, char *argv[])
{
	/*
    pango_parse_markup("<span foreground=\"yellow\">   <b>Bold</b></span>", -1, 0, &impcolore1, NULL , NULL, NULL);
    pango_parse_markup("<span foreground=\"red\">   <b>Bold</b></span>", -1, 0, &impcolore2, NULL , NULL, NULL);
    pango_parse_markup("<span foreground=\"black\">   <b>Bold</b></span>", -1, 0, &impcoloreneutro, NULL, NULL, NULL);
	*/

	/* Initialize GTK+ */

	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

	/* Create the main window */
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (win), 5);
	gtk_window_set_title (GTK_WINDOW (win), "FORZA 4");
	gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
	gtk_widget_realize (win);
	g_signal_connect (win, "destroy", gtk_main_quit, NULL);

	/* Create a vertical box with buttons */

	table=gtk_table_new(7,6,FALSE);                       //numero di colonne,numero di righe,si ridimenstionano dinamicamente
	gtk_container_add (GTK_CONTAINER (win), table); //aggiunta della table alla finsestra

            //label del punteggio del giocatore 1
	labelpun1 = gtk_label_new(punteggio1);
	gtk_widget_modify_font(labelpun1, pango_font_description_from_string("12"));
    gtk_table_attach_defaults(GTK_TABLE(table),labelpun1,0,2,0,1);

            //label del punteggio del giocatore 2
	labelpun2 = gtk_label_new(punteggio2);
	gtk_widget_modify_font(labelpun2, pango_font_description_from_string("12"));
	gtk_table_attach_defaults(GTK_TABLE(table),labelpun2,2,4,0,1);

            //label turno;
	labelstatus = gtk_label_new("Turno giocatore 1");
	gtk_widget_modify_font(labelstatus, pango_font_description_from_string("11"));
	gtk_table_attach_defaults(GTK_TABLE(table),labelstatus,5,7,0,1);

	iniztab();

	/* Enter the main loop */
	gtk_widget_show_all (win);
	gtk_main ();
	return 0;
}

void iniztab(void)     //inizializza la tabella
{
    colorprintf(4,"Storico Partita attuale\n");
	for (int i = 0; i < n-1; i++) {     //tabella passiva
		for (int j = 0; j < n; j++) {
			elemento[i][j].label[0]='-';//caselle di gioco
			elemento[i][j].riga=i;      //riga
			elemento[i][j].colonna=j;   //colonna
			elemento[i][j].widget=gtk_image_new_from_file(Vuoto);        //creazione della parte grafica della tabella di gioco
			gtk_table_attach_defaults(GTK_TABLE(table),elemento[i][j].widget,  //collega la lable alla tabella
			                          elemento[i][j].colonna,elemento[i][j].colonna+1, //colonna
			                          elemento[i][j].riga+1,elemento[i][j].riga+2);//riga
		}
	}

	for (int i = 0; i < n; i++) { //creazione della tabella attiva
		elemento[n-1][i].label[0]=49+i;   //caselle non di gioco(codifica UTF-8)
		elemento[n-1][i].riga=n-1;        //riga
		elemento[n-1][i].colonna=i;       //colonna
		elemento[n-1][i].widget=gtk_button_new_with_label(elemento[n-1][i].label); //creazione pulsanti
		gtk_table_attach_defaults(GTK_TABLE(table),elemento[n-1][i].widget,elemento[n-1][i].colonna,elemento[n-1][i].colonna+1,n,n+1);//posizionamento dei bottoni
		g_signal_connect (G_OBJECT(elemento[n-1][i].widget), "clicked", G_CALLBACK (mossa), (gpointer) &elemento[n-1][i].colonna);//collegamento a funzione

	}
}

void mossa(GtkWidget* button, int* colonna)
{
	int temp=0;
	if(vittoria==-1) {
		if (segno[*colonna]!=-1) {
			temp=segno[*colonna];
			elemento[temp][*colonna].label[0]=turno[0];
			controllovittoria();
			if(turno[0]=='X') {
				gtk_image_set_from_file_utf8(GTK_IMAGE(elemento[temp][*colonna].widget),P1);
				turno[0]='O';
				segno[*colonna]--;
				gtk_label_set_text(GTK_LABEL(labelstatus),"Turno giocatore 2");
				flag++;
			} else {
				gtk_image_set_from_file_utf8(GTK_IMAGE(elemento[temp][*colonna].widget),P2);
				turno[0]='X';
				segno[*colonna]--;
				gtk_label_set_text(GTK_LABEL(labelstatus),"Turno giocatore 1");
				flag++;
			}
		} else {
			colonnapiena();
		}
	}
	if(vittoria!=-1) {
		fineparita();
		nuovapartita();
	}
	if(flag==42 && vittoria==-1) {
		fineparita();
		flag=0;
		nuovapartita();
	}
}

void nuovapartita(void)
{
	printf("\n");
	for (int i = 0; i < n-1; i++) { //tabella passiva
		for (int j = 0; j < n; j++) {
			if(elemento[i][j].label[0]=='X') {
				colorprintf(2,"X ");
			}
			if(elemento[i][j].label[0]=='O') {
				colorprintf(0,"O ");
			}
			if(elemento[i][j].label[0]=='-') {
				printf("- ");
			}
			elemento[i][j].label[0]='-';
			gtk_image_set_from_file_utf8(GTK_IMAGE(elemento[i][j].widget),Vuoto);
		}
		printf("\n");
	}
	printf("1 2 3 4 5 6 7\n_____________\n");
	for(int i=0; i<n; i++)
		segno[i]=5;
	flag=0;
}

void controllovittoria(void)
{
	int win1=0;
	for(int i=0; i<n; i++) {
		if(vittoria!=-1) break;
		for(int j=0; j<n; j++) {
			if(elemento[i][j].label[0]==turno[0]) {
				win1++;
				if(turno[0]=='X' && win1==4) {
					vittoria=1;   //vince il giocatore 1
				}
				if(turno[0]=='O' && win1==4) {
					vittoria=2;   //il giocatore 2 ha vinto
				}
			}
			if (elemento[i][j].label[0]!=turno[0]) {
				win1=0;
			}
		}
		win1=0;
	}

	if(vittoria==-1) {
		///COLONNE
		for(int j=0; j<n; j++) {
			if(vittoria!=-1) break;
			for(int i=0; i<n; i++)    {
				if(elemento[i][j].label[0]==turno[0]) {
					win1++;
					if(turno[0]=='X' && win1==4) {
						vittoria=1;   //vince il giocatore 1
					}
					if(turno[0]=='O' && win1==4) {
						vittoria=2;   //il giocatore 2 ha vinto
					}
				}
				if (elemento[i][j].label[0]!=turno[0]) {
					win1=0;
				}
			}
			win1=0;
		}

		if(vittoria==-1) {
			///DIAGONALI PRINCIPALI...
			int segnoP=0; //deve saltare le diagonali numero 0 1 2 9 10 11 12 -> deve cercare nelle diagonali 3 4 5 6 7 8
			for(int e=1-n; e<=n-1; e++) {
				if(vittoria!=-1) break;
				if(segnoP==3 || segnoP==4 || segnoP==5 ||segnoP==6 || segnoP==7 || segnoP==8) {//colonne dove è possibile fare "forza4"
					for(int i=0; i<n; i++) {
						for(int j=0; j<n; j++) {
							if(i-j==e) {//condizone che il valore è presenta nella p diagonale
								if(elemento[i][j].label[0]==turno[0]) {
									win1++;
									if(turno[0]=='X' && win1==4) {
										vittoria=1;   //vince il giocatore
									}
									if(turno[0]=='O' && win1==4) {
										vittoria=2;   //il giocatore 2 ha vinto
									}
								}
								if (elemento[i][j].label[0]!=turno[0]) {
									win1=0;
								}
							}
						}
					}
					//printf("\n");
				}
				segnoP++;
				win1=0;
			}

			///iagonali secondarie...
			int segnoS=0;
			for(int f=0; f<=2*n; f++) {
				if(vittoria!=-1) break;
				if(segnoS==3 || segnoS==4 || segnoS==5 ||segnoS==6 || segnoS==7 || segnoS==8) {
					for(int i=0; i<n; i++) {
						for(int j=0; j<n; j++) {
							if(i+j==f) {//condizone che il valore è presenta nella diagonale
								if(elemento[i][j].label[0]==turno[0]) {
									win1++;
									if(turno[0]=='X' && win1==4) {
										vittoria=1;   //vince il giocatore 1
									}
									if(turno[0]=='O' && win1==4) {
										vittoria=2;   //il giocatore 2 ha vinto
									}
								}
								if (elemento[i][j].label[0]!=turno[0]) {
									win1=0;
								}
							}
						}
					}
					win1=0;
				}
				segnoS++;
			}
		}
	}

}

static void colonnapiena(void)
{
	GtkWidget *dialog = NULL;

	dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"Colonna piena");
	//dove          //che tipo di messagio      //generazione del bottone standard di chiusura   //messagio
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);//posizione della finestra
	gtk_dialog_run (GTK_DIALOG (dialog));   //esecuzione
	gtk_widget_destroy (dialog);            //agg al bottone la funzione di chiusura
}

static void fineparita(void)
{
	GtkWidget *dialog = NULL;
	if(vittoria!=-1) {
		if(vittoria==1) {
			printf("\n ***vince il giocatore 1***\n");
			vittoria=-1;
			punteggio1[13]=punteggio1[13]+1;
			gtk_label_set_text(GTK_LABEL(labelpun1),punteggio1);
			if(punteggio1[13]=='3') {
				dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,"Fine del round\nPartita finita\nVince il giocatore 1");
				printf("\n Partita vinta dal giocatore 1\n");
				punteggio1[13]='0'; //viene ridefinito il dialog
				punteggio2[13]='0';
				gtk_label_set_text(GTK_LABEL(labelpun1),punteggio1);
				gtk_label_set_text(GTK_LABEL(labelpun2),punteggio2);
				turno[0]='X';
			} else
				dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER,GTK_BUTTONS_CLOSE,"Round vinto dal giocatore 1");
		}
		if(vittoria==2) {
			printf("\n ***vince il giocatore 2***");

			vittoria=-1;
			punteggio2[13]=punteggio2[13]+1;
			gtk_label_set_text(GTK_LABEL(labelpun2),punteggio2);
			if(punteggio2[13]=='3') {

				dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Fine del round\nPartita finita\nVince il giocatore 2");
				printf("\n Partita vinta dal giocatore 2\n");
				punteggio1[13]='0';
				punteggio2[13]='0';
				turno[0]='X';
				gtk_label_set_text(GTK_LABEL(labelpun1),punteggio1);
				gtk_label_set_text(GTK_LABEL(labelpun2),punteggio2);
			} else
				dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER,GTK_BUTTONS_CLOSE,"Round vinto dal giocatore 2");
		}       //turno non viene reinizializzo a X per permettere al giocatore che perde di iniziare
	}
	printf("\nGiocatore 1: %c\tGiocatore 2:%c\n",punteggio1[13],punteggio2[13]);
	if(flag==42 && vittoria==-1) {
		printf("\nPareggio");
		dialog = gtk_message_dialog_new (GTK_WINDOW (win),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,"Fine del round\nPareggio");
		flag=0;
	}
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ON_PARENT);//posizione della finestra
	gtk_dialog_run (GTK_DIALOG (dialog));   //esecuzione
	gtk_widget_destroy (dialog);
	//agg al bottone la funzione di chiusura

}
