#include <iostream>

using namespace std;

int trailingFac(int n);
int Factorial(int n);

int main()
{
  int n;
  cout<<"Factorial number: ";
  cin >> n;

  n = trailingFac(n);
  
  cout<< n;
}


int trailingFac(int n)
{
  int m = Factorial(n);
  int ii = 0;

  cout<<m<<endl;
  
  while( m%10 == 0 )
  {
    m /= 10;
    ii++;
  }
  return ii;
}

int Factorial(int n)
{
  if ( n<=1 )
  {
    return 1;
  } else {
    return n * Factorial(n-1);
  }
  
}
