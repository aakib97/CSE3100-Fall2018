#define N 10000

int a[N][N];

int main()
{
   int i, j;

   for(j=0; j<N; j++)
      for(i=0; i<N; i++)
         a[i][j] = i+j;

   return 0;
}
