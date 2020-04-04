#include "blockChain.h"


/*
Description :

*/
char* subString(char *hash, int pos, int len){
  int i;
  char *sub = (char*)malloc((len+1)*sizeof(char));

   for (i = 0 ; i < len; i++){
      *(sub+i) = *(hash+pos);
      hash++;
   }
   *(sub+i) = '\0';

   return sub;
}


/*
Description :

*/
char* afficheTimestamp(time_t tim){
  struct tm  ts;
  char* buf = (char*)malloc(100*sizeof(char));

  ts = *localtime(&tim);
  strftime(buf, 100*sizeof(buf), "%Y/%m/%d %H:%M:%S", &ts);

  return buf;
}


/*
Description :

*/
void afficherBC(Blockchain* G) {
  char* tim = (char*)malloc(100*sizeof(char));

  Block* s = G->first;
  tim = afficheTimestamp(s->timestamp);

  while(s != NULL) {
    printf("________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n");
    for(int i = 0; i < s->nb_trans; i++){
      printf("____________________________________________________________________________________________________________________________________________________________________________________________________________\n");
      printf("|index : %d        |author : %s        |timestamp : %s       |Nom de la transaction : %s        |hash : %s        |previousHash : %s       |nonce : %ld       |transaction : %d       |destinataire : %s       |expéditeur : %s       |montant : %d       |\n",s->index, s->author, tim, s->data, s->hash, s->previousHash, s->nonce, i + 1, s->trans[i].dest, s->trans[i].rep, s->trans[i].montant);
      printf("____________________________________________________________________________________________________________________________________________________________________________________________________________\n");
    }
    printf("________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n");
    printf("\n");
    s = s->successor;
  }
  printf("\n");
}


/*
Description :

*/
void afficherNom(Blockchain* G){
  int j = 0;
  char** noms = (char**)malloc(100*sizeof(char*));

  Block* s = G->first;
  s = s->successor;
  while(s != NULL) {
    for(int i = 0; i < s->nb_trans; i++){
      noms[j] = s->trans[i].dest;
      j++;
      noms[j] = s->trans[i].rep;
      j++;
      noms[j] = s->author;
      j++;
    }
    s = s->successor;
  }
  for(int k = 0; k < j; k++){
    printf("%s\n", noms[k]);
  }
}
