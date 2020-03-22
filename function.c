#include "blockChain.h"


char* subString(char *hash, int pos, int len){
   char *sub = (char*)malloc((len+1)*sizeof(char));
   int i;
   for (i = 0 ; i < len; i++){
      *(sub+i) = *(hash+pos);
      hash++;
   }

   *(sub+i) = '\0';
   return sub;
}



char* afficheTimestamp(time_t tim){
  struct tm  ts;
  char* buf = (char*)malloc(100*sizeof(char));
  ts = *localtime(&tim);
  strftime(buf, 100*sizeof(buf), "%Y/%m/%d %H:%M:%S", &ts);
  return buf;
}



void afficherBC(Graph* G) {
  Node* s = G->first;
  char* tim = (char*)malloc(100*sizeof(char));
  tim = afficheTimestamp(s->timestamp);
  while(s != NULL) {
    for(int i = 1; i <= s->nb_trans; i++){
      printf("____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n");
      printf("|index : %d        |author : %s        |timestamp : %s       |data : %s        |hash : %s        |previousHash : %s       |nonce : %d       |nb_trans : %d       |trans.dest : %s       |trans.rep : %s       |trans.montant : %d       |\n",s->index, s->author, tim, s->data, s->hash, s->previousHash, s->nonce, s->nb_trans, s->trans[i].dest, s->trans[i].rep, s->trans[i].montant);
      printf("____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________\n\n");
    }
    printf("\n");
    s = s->successor;
  }
  printf("\n");
}
