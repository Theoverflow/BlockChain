/*********************************************************************************
A Blockchain is defined by:
- Number of Blocks
- Array of pointers, containing the addresses of the successor lists of Blockchain Blocks
*********************************************************************************/

#ifndef BLOCKCHAIN_H

#define BLOCKCHAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>




typedef struct _transaction {
  char* dest;
  char* rep;
  char* signature;
  char* hash;
  int montant;
} Transaction;

typedef struct _Block
{
	int index;
  long nonce;
  int nb_trans;
  char* author;
	time_t timestamp;
  char* data;
	char* hash;
  char* previousHash;
  Transaction* trans;
	struct _Block* successor;
} Block;

typedef struct
{
	int nb_Blocks;
  int difficult;
  int reward;
  int nb_trans;
  char* condition;
  Transaction* transEnAtt;
	Block* first;
} Blockchain;

/////////////////////////////////////////////////

void initBC(Blockchain* G);
void ajouterBlock(Blockchain* G);
void modifierBloc(Blockchain* G);
char* calculateHash(Block* s);
void proofOfWorkBC(Block* s, int difficult, char* c);
bool blockChainValide(Blockchain* G);
void estValide(Blockchain* G);
void creerTransaction(Blockchain* G);
void miningTransaction(Blockchain* G);
void compteBloc(Blockchain* G);
void modifierTransaction(Blockchain* G, Block* s);
void signTransaction(Transaction* trans, char* signingKey);
bool transactionValide(Transaction trans);
bool blockVerifyTransaction(Block* s);
char* getFromPublic(char* from);
bool verifyKey(Transaction trans, char* key);
void menu(Blockchain* G);

char* subString(char *hash, int pos, int len);
char* afficheTimestamp(time_t tim);
void afficherBC(Blockchain* G);
void afficherNom(Blockchain* G);

#endif
