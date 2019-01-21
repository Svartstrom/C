#include <iostream>

struct l
{
  int val;
  l *N = NULL;
};

void printLL(l *N);
void printRevLL(l *N);

int main()
{
  l * N = new(l);
  N->val = 15;
  //N->N = NULL;
  for (int ii = 0; ii <5; ii++)
    {
      l * t = new(l);
      t->val=ii;
      t->N = N;
      N = t;
    }
  printLL(N);
  std::cout<<std::endl;
  printRevLL(N);
}


void printLL(l *N)
{
  if (N != NULL)
    {
      std::cout<<N->val<<std::endl;
      printLL(N->N);
    }
}
void printRevLL(l *N)
{
  if (N!=NULL)
    {
      printRevLL(N->N);
      std::cout<<N->val<<std::endl;
    }
}
