#include <iostream>
#include <cmath>

using namespace std;

int maxPow(int n)
{
  int i = 1;
  while (pow(2,i) <= n/2)
    {
      i++;
    }
  return i;
}



int main(int argc, char *argv[])
{
  if ( argc < 2 || argc > 2 )
  {
    cout<<"Usage: dectobin ddd"<<endl;
    return 1;
  }

  int n = stoi(argv[1]);
  //cout<<n<<" "<<maxPow(n)<<endl;
  for ( int ii = maxPow(n); ii > -1; ii--)
    {
      //cout<<pow(2,ii) << " ";
      if (n >= pow(2,ii) )
	{
	  cout<<1;
	  n -= pow(2,ii);
	} else
	{
	  cout<<0;
	}
    }
  
  cout<<endl;
}
