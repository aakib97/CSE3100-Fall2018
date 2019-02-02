#define N 10000

int a[N][N];

int main()
{
   int i, j;

   for(i=0; i<N; i++)
      for(j=0; j<N; j++)
         a[i][j] = i+j;

   return 0;
}
