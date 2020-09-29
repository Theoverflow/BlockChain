#include <stdio.h>
#include "blockChain.h"
int main(void){
  Blockchain BC;
  initBC(&BC);
  menu(&BC);
  return 0;
}
