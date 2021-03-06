#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct BoardTag {
   int row;
   int col;
   char** src;
} Board;

Board* makeBoard(int r,int c)
{
   Board* p = (Board*)malloc(sizeof(Board));
   p->row = r;
   p->col = c;
   p->src = (char**)malloc(sizeof(char*)*r);
   int i;
   for(i=0;i<r;i++)
      p->src[i] = (char*)malloc(sizeof(char)*c);
   return p;
}
void freeBoard(Board* b)
{
   int i;
   for(i=0;i<b->row;i++)
      free(b->src[i]);
   free(b->src);
   free(b);
}

Board* readBoard(char* fName)
{
   int row,col,i,j;
   FILE* src = fopen(fName,"r");
   fscanf(src,"%d %d\n",&row,&col);
   Board* rv = makeBoard(row,col);
   for(i=0;i<row;i++) {
      for(j=0;j<col;j++) {
         char ch = fgetc(src);
         rv->src[i][j] = ch == '*';
      }
      char skip = fgetc(src);
      while (skip != '\n') skip = fgetc(src);
   }
   fclose(src);
   return rv;
}

void saveBoard(Board* b,FILE* fd)
{
   int i,j;
   for(i=0;i<b->row;i++) {
      fprintf(fd,"|");
      for(j=0;j < b->col;j++)
         fprintf(fd,"%c",b->src[i][j] ? '*' : ' ');
      fprintf(fd,"|\n");
   }
}

void printBoard(Board* b)
{
   printf("\033\143");  /* this is to clear the terminal */
   saveBoard(b,stdout);
}

int liveNeighbors(int i,int j,Board* b)
{
   const int pc = (j-1) < 0 ? b->col-1 : j - 1;
   const int nc = (j + 1) % b->col;
   const int pr = (i-1) < 0 ? b->row-1 : i - 1;
   const int nr = (i + 1) % b->row;
   int xd[8] = {pc , j , nc,pc, nc, pc , j , nc };
   int yd[8] = {pr , pr, pr,i , i , nr , nr ,nr };
   int ttl = 0;
   int k;
   for(k=0;k < 8;k++)
      ttl += b->src[yd[k]][xd[k]];
   return ttl;
}

void evolveBoard(Board* src,Board* out)
{
   static int rule[2][9] = {
      {0,0,0,1,0,0,0,0,0},
      {0,0,1,1,0,0,0,0,0}
   };
   int i,j;
   for(i=0;i<src->row;i++) {
      for(j=0;j<src->col;j++) {
         int ln = liveNeighbors(i,j,src);
         int c  = src->src[i][j];
         out->src[i][j] = rule[c][ln];
      }
   }
}

int main(int argc,char* argv[])
{
   int cycles = 1000;
   Board* life1 = readBoard(argv[1]);
   Board* life2 = makeBoard(life1->row,life1->col);
   Board *b0, *b1;
   int g;
   if(argc>2) cycles = atoi(argv[2]);

   for(g=0;g < cycles; g++) {
      b0 = g & 0x1 ? life2 : life1;
      b1 = g & 0x1 ? life1 : life2;
      printBoard(b0);
      evolveBoard(b0,b1);
      usleep(15000);
   }
   printBoard(b1);
   FILE* final = fopen("final.txt","w");
   saveBoard(b1,final);
   fclose(final);
   freeBoard(life1);
   freeBoard(life2);
   return 0;
}
