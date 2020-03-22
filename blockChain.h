/*********************************************************************************
A graph is defined by:
- Number of nodes
- Array of pointers, containing the addresses of the successor lists of graph nodes
*********************************************************************************/

#ifndef GRAPH_H

#define GRAPH_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "hash.h"



typedef struct _transaction {
  char* dest;
  char* rep;
  int montant;
} Transaction;

typedef struct _node
{
	int index;
  int nonce;
  int nb_trans;
  char* author;
	time_t timestamp;
  char* data;
	char* hash;
  char* previousHash;
  Transaction* trans;
	struct _node* successor;
} Node;

typedef struct
{
	int nb_nodes;
  int difficult;
  int reward;
  int nb_trans;
  Transaction* transEnAtt;
	Node* first;
} Graph;

/////////////////////////////////////////////////

void initBC(Graph* G);
void afficherBC(Graph* G);
void ajouterBlock(Graph* G);
void menu(Graph* G);
char* calculateHash(Node* s);
bool blocChainValide(Graph* G);
void estValide(Graph* G);
void modifierBloc(Graph* G);
char* afficheTimestamp(time_t tim);
void proofOfWorkBC(Graph* G, Node* s, int difficult, char c);
char* subString(char *hash, int pos, int len);
void miningTransaction(Graph* G);
void creerTransaction(Graph* G);
void compteBloc(Graph* G);


#endif
