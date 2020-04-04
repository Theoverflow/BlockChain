#include "blockChain.h"
#include "hash.h"


/*
Description :

*/
void initBC(Blockchain* G) {
  int i;
  time_t tps = time(NULL);
  Block* s = (Block*)malloc(sizeof(Block));

  printf("\nChoisir difficulté : ");
  scanf("%d",&G->difficult);

  G->condition = (char*)malloc(sizeof(char));
  printf("Choisir la condition de minage :[a,b,c,d,e,f,0,1,2,3,4,5,6,7,8,9]\nCaractère : ");
  scanf("%s", G->condition);

  G->nb_Blocks = 1;
  G->reward = G->difficult*100;
  G->nb_trans = 0;
  G->transEnAtt = (Transaction*)malloc(50*sizeof(Transaction));

  s->index = G->nb_Blocks - 1;
  s->nonce = 0;
  s->author = "admin";
  s->timestamp = tps;
  s->data = "Transaction_0";
  s->previousHash = "0";
  calculateHash(s);

  s->nb_trans = 1;
  s->trans = (Transaction*)malloc(sizeof(Transaction));
  s->trans[0].dest = "destinataire";
  s->trans[0].rep = "expéditeur";
  s->trans[0].montant = 0;

  s->successor = NULL;
	G->first = s;
}


/*
Description :

*/
void ajouterBlock(Blockchain* G) {
  char* nom2 = (char*)malloc(25*sizeof(char));
  char* data2 = (char*) malloc(1000*sizeof(char));
  Block* s2 = (Block*)malloc(sizeof(Block));

  printf("\nAjouter un bloc à la chaîne : \nDonner les informations du bloc :\nauthor : ");
  scanf("%s", nom2);
  printf("data : ");
  scanf("%s", data2);

  Block* s = G->first;
  while(s->successor != NULL){
    s = s->successor;
  }

  time_t tps2 = time(NULL);
  G->nb_Blocks ++;
  s2->index = s->index + 1;
  s2->nonce = 0;
  s2->author = nom2;
  s2->data = data2;
  s2->timestamp = tps2;
  s2->previousHash = s->hash;
  proofOfWorkBC(s2, G->difficult, G->condition);
  s->successor = s2;
  s2->successor = NULL;

  printf("Le bloc a bien été ajouté\n\n");
}


/*
Description :

*/
void modifierBloc(Blockchain* G){
  int index;
  int modif;
  char* nom = (char*)malloc(25*sizeof(char));
  char* data = (char*)malloc(1000*sizeof(char));

  printf("Voici la liste des blocs : \n\n");
  afficherBC(G);

  printf("Quel bloc souhaitez-vous modifier ?");
  scanf("%d", &index);

  Block* s = G->first;
  while(s->index != index){
    s = s->successor;
  }

  printf("Que souhaitez-vous modifier :\n");
  printf("1 - Le nom de l'auteur\n");
  printf("2 - Le message\n");
  printf("3 - Une transaction\n");
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

    case 3:
      modifierTransaction(G, s);
    break;

    default:
    break;
  }

  while(s != NULL){
    proofOfWorkBC(s, G->difficult, G->condition);
    time_t tps = time(NULL);
    s->timestamp = tps;
    if(s->successor != NULL)
      s->successor->previousHash = s->hash;
    s = s->successor;
  }

  printf("Le bloc n°%d a bien été modifié\n\n", index);
}


/*
Description :

*/
char* calculateHash(Block* s){
  int nb;
  char* hash;
  char* author;
  char* data;
  char temp[100];
  SHA256_CTX ctx;
  BYTE text[100];
  BYTE buf[SHA256_BLOCK_SIZE];
  char* word = (char*)malloc(100*sizeof(char));

  data = s->data;
  hash = s->previousHash;
  author = s->author;
  nb = s->index + (long)s->timestamp + s->nonce;

  sprintf(temp, "%d", nb);
  strcat(temp,hash);
  strcat(temp, author);
  strcat(temp, data);
  strcpy(text, temp);

  sha256_init(&ctx);
	sha256_update(&ctx, text, strlen(text));
	sha256_final(&ctx, buf);

  for(int i = 0; i < SHA256_BLOCK_SIZE; i++){
    sprintf(temp,"%x", buf[i]);
    strcat(word, temp);
  }
  return word;
}


/*
Description :

*/
void proofOfWorkBC(Block* s, int difficult, char* c){
  int j;
  char* tab = (char*)malloc(difficult*sizeof(char));
  char* tab2 = (char*)malloc(100*sizeof(char));

  for(j = 0; j < difficult; j++){
      *(tab+j) = *(c);
  }
  *(tab+j) = '\0';

  s->nonce++;
  s->hash = calculateHash(s);
  tab2 = subString(s->hash, 0, difficult);
  while(strcmp(tab2, tab) != 0){
    s->nonce++;
    s->hash = calculateHash(s);
    tab2 = subString(s->hash, 0, difficult);
    printf("%ld\n", s->nonce);
    printf("%s\n", s->hash);
    printf("%s\n", tab);
  }
  printf("Le block a été miné\n\n");
}


/*
Description :

*/
bool blockChainValide(Blockchain* G){
  int nb;
  bool valide = true;
  Block* s = G->first;
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


/*
Description :

*/
void estValide(Blockchain* G){
  if(blockChainValide(G))
    printf("Block Chain valide\n\n");
  else
    printf("Block Chain non valide\n\n");
}


/*
Description :

*/
void creerTransaction(Blockchain* G){
  int montant;
  Transaction trans;
  char* dest = (char*)malloc(25*sizeof(char));
  char* rep = (char*)malloc(25*sizeof(char));

  printf("destinataire : ");
  scanf("%s", dest);
  printf("expéditeur : ");
  scanf("%s", rep);
  printf("montant : ");
  scanf("%d", &montant);

  G->transEnAtt[G->nb_trans].dest = dest;
  G->transEnAtt[G->nb_trans].rep = rep;
  G->transEnAtt[G->nb_trans].montant = montant;
  G->nb_trans++;

  for(int i = 0; i < G->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i+1, G->transEnAtt[i].dest, G->transEnAtt[i].rep, G->transEnAtt[i].montant);
  }
  printf("La transaction a bien été créée\n\n");
}


/*
Description :

*/
void miningTransaction(Blockchain* G){
  int montant;
  char* rewardAddress = (char*)malloc(25*sizeof(char));
  char* nom = (char*)malloc(25*sizeof(char));
  char* data = (char*) malloc(1000*sizeof(char));
  Block* s2 = (Block*)malloc(sizeof(Block));

  printf("Nom à adresser le gain : ");
  scanf("%s", rewardAddress);

  G->transEnAtt[G->nb_trans].dest = rewardAddress;
  G->transEnAtt[G->nb_trans].rep = (char*)NULL;
  G->transEnAtt[G->nb_trans].montant = G->reward;
  G->nb_trans++;

  printf("\nTransactions en traitement : \n");
  for(int i = 0; i < G->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i+1, G->transEnAtt[i].dest, G->transEnAtt[i].rep, G->transEnAtt[i].montant);
  }

  printf("\nAjouter un bloc à la chaîne : \nDonner les informations du bloc :\nauthor : ");
  scanf("%s", nom);
  printf("Nom de la transaction : ");
  scanf("%s", data);

  Block* s = G->first;

  while(s->successor != NULL){
    s = s->successor;
  }

  time_t tps2 = time(NULL);
  s2->timestamp = tps2;
  s2->index = s->index + 1;
  s2->nonce = 0;
  s2->author = nom;
  s2->data = data;
  s2->previousHash = s->hash;
  proofOfWorkBC(s2, G->difficult, G->condition);

  s2->trans = (Transaction*)malloc(50*sizeof(Transaction));
  s2->nb_trans = G->nb_trans;
  s2->trans = G->transEnAtt;
  G->nb_Blocks ++;

  for(int i = 0; i < s2->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i+1, s2->trans[i].dest, s2->trans[i].rep, s2->trans[i].montant);
  }

  s->successor = s2;
  s2->successor = NULL;
  free(G->transEnAtt);
  G->transEnAtt = (Transaction*)malloc(50*sizeof(Transaction));
  G->nb_trans = 0;
  printf("La transaction a bien été réalisée\n\n");
}


/*
Description :

*/
void compteBloc(Blockchain* G){
  int compte = 0;
  char* address = (char*)malloc(25*sizeof(char));

  printf("Voici la liste des noms de la blockchain : \n");
  afficherNom(G);
  printf("Nom : ");
  scanf("%s", address);

  Block* s = G->first;

  while(s != NULL){
    for(int i = 0; i < s->nb_trans; i++){

      if(strcmp(s->trans[i].rep, address) == 0){
        compte = compte - s->trans[i].montant;
      }

      if(strcmp(s->trans[i].dest, address) == 0){
        compte = compte + s->trans[i].montant;
      }
    }
    s = s->successor;
  }
  printf("Compte de %s : %d\n\n", address, compte);
}


/*
Description :

*/
void modifierTransaction(Blockchain* G, Block* s){
  int num_trans;
  int montant;
  int choix2;
  char* dest = (char*)malloc(25*sizeof(char));
  char* rep = (char*)malloc(25*sizeof(char));

  printf("Voici la liste des transactions qui ont été réalisé : \n");
  for(int i = 0; i < s->nb_trans; i++){
    printf("Transaction %d : %s %s %d\n", i+1, s->trans[i].dest, s->trans[i].rep, s->trans[i].montant);
  }

  printf("Quelle transaction souhaitez-vous modifier ? ");
  scanf("%d", &num_trans);
  printf("Que souhaitez-vous modifier ?\n");
  printf("1 - Le destinataire\n");
  printf("2 - L'expéditeur\n");
  printf("3 - Le montant\n");
  scanf("Choix : ");

  switch(choix2){
    case 1:
      printf("Quel est le nom du nouveau destinataire : ");
      scanf("%s", dest);
      s->trans[num_trans-1].dest = dest;
    break;

    case 2:
      printf("Quel est le nom du nouvel expéditeur : ");
      scanf("%s", rep);
      s->trans[num_trans-1].rep = rep;
    break;

    case 3:
      printf("Quel est le nouveau montant : ");
      scanf("%d", &montant);
      s->trans[num_trans-1].montant = montant;
    break;

    default:
    break;
  }
  printf("Voici la transaction modifiée : \n");
  printf("Transaction %d : %s %s %d\n\n", num_trans, s->trans[num_trans-1].dest, s->trans[num_trans-1].rep, s->trans[num_trans-1].montant);
}


/*
Description :

*/
void signTransaction(Transaction* trans, char* signingKey){
  char* transHash = (char*)malloc(100*sizeof(char));

  if(strcmp(trans->rep, signingKey) != 0)
    return;
  /*
  const hashTx = this.calculateHash();
  const sig = signingKey.sign(hashTx, 'base64');
  this.signature = sig.toDER('hex');*/
}


/*
Description :

*/
bool blockVerifyTransaction(Block* s){
  for(int i = 0; i < s->nb_trans; i++){
    if(!transactionValide(s->trans[i])){
      printf("La transaction %d du block %d n'est pas valide\n", i+1, s->index);
      return false;
    }
  }
  return true;
}


/*
Description :

*/
bool transactionValide(Transaction trans){
  char* publicKey = (char*)malloc(100*sizeof(char));

  if(trans.rep == NULL)
    return true;

  if((trans.signature == NULL)||strlen(trans.signature) == 0){
    printf("Pas de signature\n");
    return false;
  }

  publicKey = getFromPublic(trans.rep);
  return verifyKey(trans, publicKey);
}


/*
Description :

*/
char* getFromPublic(char* from){
  return from;
}


/*
Description :

*/
bool verifyKey(Transaction trans, char* key){
  return true;
}


/*
Description :

*/
void menu(Blockchain *G){
  initBC(G);
  int quitter = true;
  while(quitter){
    int choix;
    printf("Choisir l'action que vous voulez faire :\n");
    printf("1 - Afficher la blockchain\n");
    printf("2 - Modifier un bloc\n");
    printf("3 - Vérifier que la blockchain est valide\n");
    printf("4 - Créer une transaction\n");
    printf("5 - Miner la transaction\n");
    printf("6 - Vérifier un compte\n");
    printf("7 - Quitter");
    printf("Choix : ");
    scanf("%d", &choix);

    switch(choix){
      case 1:
        afficherBC(G);
      break;

      case 2:
        modifierBloc(G);
      break;

      case 3:
        estValide(G);
      break;

      case 4:
        creerTransaction(G);
      break;

      case 5:
        miningTransaction(G);
      break;

      case 6:
        compteBloc(G);
      break;

      default:
        printf("Vous avez quitté l'application.\n");
        quitter = false;
      break;
    }
  }
}
