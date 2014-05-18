/*
 ============================================================================
 Name        : euristicaBRP.c
 Author      : Andrea Mancini - Università di Bologna
 Version     : 1.0
 Description : Euristica Min-Max del BRP (Blocks Relocation Problem)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 10
#define MAXBLOCK 100
#define HMAX 20

void cp_vector(int min_sort[],int min[],int dim){
int i;
	for(i=0;i<=dim;i++){
		min_sort[i]=min[i];
	}
}
int find_max_stack(int v[],int dim){
int i, max,max_i;
max=v[0];
	for(i=1;i<=dim;i++){
		if(v[i]>max){
			max=v[i];
			max_i=i;
		}
	}
return max_i;
}
void BubbleSort(int a[], int dim)
 {
 int i, j, priority;
 for (i = 0; i < (dim - 1); ++i)
 {
      for (j = 0; j < dim - 1 - i; ++j )
      {
           if (a[j] > a[j+1])
           {
                priority = a[j+1];
                a[j+1] = a[j];
                a[j] = priority;
           }
      }
 }
 }

int main()
{

int bay[MAXSTACK][MAXSTACK*3];//contiene in memoria l'istanza del problema (baia di container)
int move_counter=0;//contatore che tiene conto delle rilocazioni effettuate
int NSTACK;		//Numero di Stack dell'istanza presa in input
int NBLOCK;		//Numero di Blocchi dell'istanza presa in input
int priority=1; //La priorità del blocco da evadere
int r; 			//Blocco da rilocare in cima allo stack di R
int min[MAXSTACK],min_sort[MAXSTACK];//vettore che contiene le priorità più alte di tutti i blocchi per ogni stack
int maxstack,minstack;		 //contiene il massimo dei min[i]
int R[HMAX]; 	//insieme di blocchi da rilocare (con politica LIFO)
int i,j,z,noloop=0,stack=0,h,dimR=0,flag=0,s_star;
FILE * file;


printf("------Euristica Min-Max del Blocks Relocation Problem (BRP)------\n");
printf("-----------------      Università di Bologna    -------------\n");

file= fopen ("//home//blizzard//Scrivania//BRP//istanze//data3-4-6.dat","r");
if (file!=NULL){
	printf("Aperto file dati dell'istanza iniziale.... \n");
}

fscanf(file, "%d", &NSTACK);
fscanf(file, "%d", &NBLOCK);

printf("NSTACK:%d e NBLOCK:%d  \n",NSTACK,NBLOCK);
//Settaggio a zero della matrice bay
for(i=0;i<=NSTACK-1; i++){
	for(j=0;j<=(NSTACK-1)*3; j++){
		bay[i][j]=0;
	}
}
//Lettura del file e stampa a video
for(i=0;i<=NSTACK-1; i++){
  fscanf(file, "%d", &h);
  for(j=0;j<=h-1; j++){
	  fscanf(file, "%d",&bay[stack][j]);
	  printf("%d ",bay[stack][j]);
  }
  stack++;
  printf("\n");
}

//Applicazione dell'algoritmo sull'istanza bay per  NBLOCK-1
for(priority=1;priority<=NBLOCK-1; priority++){
	//Troviamo l'insieme R e la sua dimensione
	for(i=0;i<=NSTACK-1;i++){
	  for(j=0;j<=(NSTACK-1)*3;j++){
		  if((bay[i][j])==priority){
			  printf("Dobbiamo prendere %d devo rilocare R:",priority);
			  for(z=j+1;z<=(NSTACK-1)*3;z++){
				  if(bay[i][z]!=0){
					  R[dimR]=bay[i][z];
					  printf(" %d,",bay[i][z]);
					  dimR++;
				  }
			  }
		  }
	  }
	}
	//ora abbiamo R...
	//cicliamo per dimR (dobbiamo rilocare tutti i blocchi dell'insieme R
	while(dimR!=0){
		r=R[dimR-1]; //metto in r il blocco in cima ad R
		printf("\nRiloco il blocco:%d con ",R[dimR-1]);

		//SETTIAMO min[i]
		printf("min[i]:");
		for(z=0;z<=NSTACK-1;z++){
			if(bay[z][0]==0){	//se lo stack è vuoto lo consideriamo come NBLOCK+1
				min[z]=NBLOCK+1;
				//printf("DEBUG: considero lo stack %d come vuoto \n",z+1);
			}
			else{				//altrimenti per quello stack consideriamo il più prioritario
				min[z]=bay[z][0];
				for(j=1;j<=(NSTACK-1)*3;j++){
					if((bay[z][j]<min[z])&&(bay[z][j]!=0)){
						min[z]=bay[z][j];
					}
				}
			}
			printf(" %d ",min[z]);
		}
		printf("\n------------\n");

//CASO ARGMIN{ min(i)>r }

		//usiamo un vettore min_sort di supporto per controllare i min[i]
		//dal piu piccolo al piu grande (caso arg{ min[i]>r })
		cp_vector(min_sort,min,NSTACK-1);
		//printf("min copiata: %d %d %d %d %d \n",min_sort[0],min_sort[1],min_sort[2],min_sort[3],min_sort[4]);
		BubbleSort(min_sort,NSTACK); //ordiniamo dal più piccolo al più grande
		//printf("min normali: %d %d %d %d %d \n",min[0],min[1],min[2],min[3],min[4]);
		//printf("min ordinati: %d %d %d %d %d \n",min_sort[0],min_sort[1],min_sort[2],min_sort[3],min_sort[4]);

		//ora dobbiamo calcolare S* per rilocare r
		for(s_star=0;s_star<=NSTACK-1;s_star++){
			if(min_sort[s_star]>r){
				//abbiamo trovato il più piccolo valore di min[i] che sia maggiore di r
				printf("Trovato: ");
				printf(" min[i]=%d>%d ",min_sort[s_star],r);
				for(i=0;i<=NSTACK-1;i++){
				  for(j=0;j<=(NSTACK-1)*3;j++){
					  if(bay[i][j]==r){
						  bay[i][j]=0; //togliamo il blocco dal vecchio stack
					  }
				  }
				}
				//troviamo la posizione dello stack min_sort[s_star] in min[i]
				noloop=0;
				for(i=0;i<=NSTACK-1;i++){
					for(j=0;j<=(NSTACK-1)*3;j++){
						if((bay[i][j]==min_sort[s_star])&&(noloop==0)){
							s_star=i;
							printf("\nBlocco %d da rilocare in posizione %d di min[i] \n",r,s_star+1);
							//printf(" min_sort[s_star]=%d ",min_sort[s_star]);
							noloop++;//evitiamo di rientrare nel for
							break;
						}
					}
				}
				printf("\n");
				//ora s_star contiene lo stack da rilocare
				for(i=0;i<=(NSTACK-1)*3;i++){
					if(bay[s_star][i]==0){
						bay[s_star][i]=r; //inseriamo il blocco sul nuovo stack
						break;//lo poggio ed esco
					}
				}
			flag=1;
			break;// non considero gli altri min[i]
			}
		}

//CASO ARGMAX{ min(i) }

		//non siamo riusciti a trovare in min[i] nessun valore > r
		if(flag==0){//altrimenti lo rilochiamo nel max{min[i]}
			maxstack=find_max_stack(min,NSTACK-1);
			printf("Non ho trovato nessun min[i]>%d considero maxstack=%d \n",r,maxstack+1);
			for(i=0;i<=NSTACK-1;i++){
			  for(j=0;j<=(NSTACK-1)*3;j++){
				  if(bay[i][j]==r){
					  bay[i][j]=0; //togliamo il blocco dal vecchio stack
				  }
			  }
			}
			//troviamo la cima di maxstack
			for(i=0;i<=(NSTACK-1)*3;i++){
				if(bay[maxstack][i]==0){
					bay[maxstack][i]=r; //inseriamo il blocco sul nuovo stack
					break;//lo poggio ed esco
				}
			}
		}
		flag=0;
		dimR--;
		move_counter++;
		//STAMPA dopo ogni rilocazione
		for(i=0;i<=NSTACK-1;i++){
			for(j=0;j<=(NSTACK-1)*3;j++){
				printf("%d ",bay[i][j]);
			}
			  printf("\n");
		}

	}//while(dimR!=0)

	//ora posso togliere il blocco dalla baia
	printf("\nTolgo il blocco %d dalla baia \n\n",priority);
	for(i=0;i<=NSTACK-1;i++){
		for(j=0;j<=(NSTACK-1)*3;j++){
			if(bay[i][j]==priority){
				bay[i][j]=0; //togliamo il blocco dalla baia
			}
			printf("%d ",bay[i][j]);
		}
		printf("\n");
	}
	printf("\n------------\n");

}//for(priority=1;....

printf("tolgo il blocco finale %d dalla baia \nHo dovuto rilocare %d blocchi",priority,move_counter);

fclose(file);
return(0);
}


