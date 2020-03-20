#include "blockChain.h"
#include "hash.h"
#include "hash.c"
#include "function.c"

void initBC(Graph* G) {
  time_t tps = time(NULL);
  int i;
  G->nb_nodes = 1;
  printf("Choisir difficulté : ");
  scanf("%d",&i);
  G->difficult = 3;
  G->reward = G->difficult*100;
  G->nb_trans = 0;
  G->transEnAtt = (Transaction*)malloc(50*sizeof(Transaction));

  Node* s = (Node*)malloc(sizeof(Node));
  s->index = G->nb_nodes - 1;
  s->nonce = 0;
  s->author = "admin";
  s->timestamp = tps;
  s->data = "data";
  s->previousHash = "0";
  s->nb_trans = 1;
  s-> trans = (Transaction*)malloc(sizeof(Transaction));
  s->trans[s->nb_trans].dest = "coucou";
  s->trans[s->nb_trans].rep = "couc";
  s->trans[s->nb_trans].montant = 100;
  s->hash = calculateHash(s);
  s->successor = NULL;
	G->first = s;
}



void ajouterBlock(Graph* G) {
  char* nom2 = (char*)malloc(25*sizeof(char));
  char* data = (char*) malloc(1000*sizeof(char));
  printf("Ajouter un bloc à la chaîne : \nDonner les informations du bloc :\nauthor : ");
  scanf("%s", nom2);

  Node* s = G->first;
  while(s->successor != NULL){
    s = s->successor;
  }

  time_t tps2 = time(NULL);
  G->nb_nodes ++;
  Node* s2 = (Node*)malloc(sizeof(Node));
  s2->index = s->index + 1;
  s2->nonce = 0;
  s2->author = nom2;
  s2->data = data;
  s2->previousHash = s->hash;
  s2->timestamp = tps2;
  proofOfWorkBC(G, s2, G->difficult, 'a');
  s->successor = s2;
  s2->successor = NULL;
}



char* calculateHash(Node* s){
  SHA256_CTX ctx;
  char* hash;
  char* author;
  char temp[100];
  int nb;
  int i = 0;

  hash = s->previousHash;
  author = s->author;
  nb = s->index + (long)s->timestamp + s->nonce;

  sprintf(temp, "%d", nb);
  strcat(temp,hash);
  strcat(temp, author);

  BYTE text1[100];
  strcpy(text1, temp);
  BYTE buf[SHA256_BLOCK_SIZE];
  char word[100];
  char* newHash = (char*)malloc(100*sizeof(char));

  sha256_init(&ctx);
	sha256_update(&ctx, text1, strlen(text1));
	sha256_final(&ctx, buf);
  while(i < SHA256_BLOCK_SIZE){
    sprintf(word,"%x", buf[i]);
    strcat(newHash, word);
    i++;
  }
  return newHash;
}



void proofOfWorkBC(Graph* G, Node* s, int difficult, char c){
  char* tab = (char*)malloc((G->difficult)*sizeof(char));
  int j = 0;
  for(j = 0; j < G->difficult; j++){
      *(tab+j) = c;
  }
  *(tab+j) = '\0';

  char* tab2 = (char*)malloc(100*sizeof(char));

  s->hash = calculateHash(s);
  tab2 = subString(s->hash, 0, G->difficult);
  while(strcmp(tab2, tab) != 0){
    s->nonce++;
    s->hash = calculateHash(s);
    tab2 = subString(s->hash, 0, G->difficult);
    printf("%d\n", s->nonce);
    printf("%s\n", s->hash);
    printf("%s\n", tab);
  }
  printf("Le block a été miné\n");
}



void modifierBloc(Graph* G){
  int index;
  printf("Quel bloc souhaitez-vous modifier ?");
  scanf("%d", &index);

  Node* s = G->first;
  while(s->index != index){
    s = s->successor;
  }

  char* nom = (char*)malloc(25*sizeof(char));
  char* data = (char*)malloc(1000*sizeof(char));

  int modif;
  printf("Que souhaitez-vous modifier ?");
  scanf("%d", &modif);
  switch(modif){
    case 1:
      printf("author : ");
      scanf("%s", nom);
      s->author = nom;
    break;

    case 2:
      printf("data : ");
      scanf("%s", data);
      s->author = data;
    break;

    default:
    break;
  }
  while(s != NULL){
    proofOfWorkBC(G, s, G->difficult, 'a');
    time_t tps = time(NULL);
    if(s->successor != NULL)
      s->successor->previousHash = s->hash;
    s->timestamp = tps;
    s = s->successor;
  }
}



bool blocChainValide(Graph* G){
  bool valide = true;
  int nb;
  Node* s = G->first;
  while(s->successor != NULL){
    if((s->successor->index - s->index) > 1){
      valide = false;
      nb = s->successor->index - s->index;
      printf("%d\n", nb);
    }

    if(strcmp(s->hash,s->successor->previousHash)!=0){
      valide = false;
      printf("%s\n", s->hash);
      printf("%s\n", s->successor->previousHash);
    }
    s = s->successor;
  }
  return valide;
}



void estValide(Graph* G){
  if(blocChainValide(G))
    printf("Block Chain valide\n");
  else
  printf("Block Chain non valide\n");
}


void miningTransaction(Graph* G){
  char* rewardAddress = (char*)malloc(25*sizeof(char));
  int montant;
  printf("reward address : ");
  scanf("%s", rewardAddress);
  G->nb_trans++;
  G->transEnAtt[G->nb_trans].dest = rewardAddress;
  G->transEnAtt[G->nb_trans].rep = " ";
  G->transEnAtt[G->nb_trans].montant = G->reward;
  for(int i = 1; i <= G->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i, G->transEnAtt[i].dest, G->transEnAtt[i].rep, G->transEnAtt[i].montant);
  }

  char* nom = (char*)malloc(25*sizeof(char));
  char* data = (char*) malloc(1000*sizeof(char));
  printf("Ajouter un bloc à la chaîne : \nDonner les informations du bloc :\nauthor : ");
  scanf("%s", nom);

  Node* s = G->first;
  Node* s2 = (Node*)malloc(sizeof(Node));
  while(s->successor != NULL){
    s = s->successor;
  }

  G->nb_nodes ++;

  time_t tps2 = time(NULL);
  s2->timestamp = tps2;

  s2->index = s->index + 1;
  s2->nonce = 0;

  s2->author = nom;
  s2->data = data;

  s2->previousHash = s->hash;

  s2->trans = (Transaction*)malloc(50*sizeof(Transaction));
  s2->nb_trans = G->nb_trans;
  s2->trans = G->transEnAtt;
  proofOfWorkBC(G, s2, G->difficult, 'a');
  for(int i = 1; i <= s2->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i, s2->trans[i].dest, s2->trans[i].rep, s2->trans[i].montant);
  }
  s->successor = s2;
  s2->successor = NULL;

  free(G->transEnAtt);
  G->nb_trans = 0;
}


void creerTransaction(Graph* G){
  Transaction trans;
  char* dest = (char*)malloc(25*sizeof(char));
  char* rep = (char*)malloc(25*sizeof(char));
  int montant;
  printf("dest : ");
  scanf("%s", dest);
  printf("rep : ");
  scanf("%s", rep);
  printf("montant : ");
  scanf("%d", &montant);
  G->nb_trans++;
  G->transEnAtt[G->nb_trans].dest = dest;
  G->transEnAtt[G->nb_trans].rep = rep;
  G->transEnAtt[G->nb_trans].montant = montant;

  for(int i = 1; i <= G->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i, G->transEnAtt[i].dest, G->transEnAtt[i].rep, G->transEnAtt[i].montant);
  }
}


void compteBloc(Graph* G){
  char* address = (char*)malloc(25*sizeof(char));
  printf("address : ");
  scanf("%s", address);

  int compte = 0;
  Node* s = G->first;
  int i;
  while(s != NULL){
    printf("s->nb_trans : %d\n", s->nb_trans);
    for(int i = 1; i <= s->nb_trans; i++){

      printf("|index : %d        |author : %s        |data : %s        |hash : %s        |previousHash : %s       |nonce : %d       |nb_trans : %d       |trans.dest : %s       |trans.rep : %s       |trans.montant : %d       |\n",s->index, s->author, s->data, s->hash, s->previousHash, s->nonce, s->nb_trans, s->trans[i].dest, s->trans[i].rep, s->trans[i].montant);

      printf("la\n");
      if(strcmp(s->trans[i].rep, address) == 0){
        compte = compte - s->trans[i].montant;
      }
      printf("la\n");
      if(strcmp(s->trans[i].dest, address) == 0){
        compte = compte + s->trans[i].montant;
      }
      printf("la\n");
    }
    printf("lo\n");
    s = s->successor;
  }
  printf("compte de %s : %d\n", address, compte);
  //return compte;
}

void menu(Graph *G){
  initBC(G);
  int quitter = true;
  while(quitter){
    int choix;
    printf("Choisir l'action que vous voulez faire :");
    scanf("%d", &choix);

    switch(choix){
      case 1:
        afficherBC(G);
      break;

      case 2:
        ajouterBlock(G);
      break;

      case 3:
        modifierBloc(G);
      break;

      case 4:
        estValide(G);
      break;

      case 5:
        creerTransaction(G);
      break;

      case 6:
        miningTransaction(G);
      break;

      case 7:
        compteBloc(G);
      break;

      default:
        quitter = false;
      break;
    }
  }
}

int main(void){
  Graph G;
  menu(&G);
  return 0;
}
