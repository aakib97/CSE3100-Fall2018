#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define     NUM_CHAR_PER_LINE       8
#define     FONT_WIDTH              8
#define     FONT_HEIGHT             8
#define     FONT_NBYTES_PER_ROW     1
#define     FONT_NBYTES_PER_CHAR    (FONT_NBYTES_PER_ROW * FONT_HEIGHT)
#define     PIXEL_FONT              '*'
#define     PIXEL_BACKGROUND        ' '

/* buf is the display buffer.
 * s is the string to be displayed on this 'line'.
 * fp points to the opened font file.
 *
 * The function retrieves font information for up to NUM_CHAR_PER_LINE characters
 * in s from fp, and updates the display buffer (buf). 
 *
 * The function returns the number of bytes that have been processed. 
 * Should be a number between 0 and NUM_CHAR_PER_LINE.
 * */
unsigned int str_render (char **buf, char *s, FILE *fp)
{
    unsigned int n = 0;
    // TODO
    for(int j = 0; j < NUM_CHAR_PER_LINE; j++){
        for(int i = 0; i < NUM_CHAR_PER_LINE; i++){
            char charac = s[i];
            fseek(fp, charac*8+j, SEEK_SET);
            char read = fgetc(fp);
            buf[j][i] = read;
        }
    }
    return n;
}

/* Clear the display buffer. 
 * If print_buffer() is called right after, only PIXEL_BACKGROUND will be displayed.
 * Try to type 'clear' in your bash.
 * */
void    clear_buffer(char **buf)
{
    // TODO
    for(int j = 0; j < NUM_CHAR_PER_LINE; j++){
        for(int i = 0; i < NUM_CHAR_PER_LINE; i++){
            buf[j][i] = 0;
        }
    }
}

/* shown the display buffer on the screen. */
void print_buffer(char ** buf)
{
    // TODO
    char charac;
    int bits;
    char arr[65] = {0};

    for(int i = 0; i < FONT_HEIGHT; i++){
        for(int j = 0; j < NUM_CHAR_PER_LINE; j++){

            charac = buf[i][j];
            bits = 1;

            for(int z = 0; z < NUM_CHAR_PER_LINE; z++){
                if((charac & bits) == 0){
                    arr[j * NUM_CHAR_PER_LINE + z] = 32;
                }else{
                    arr[j * NUM_CHAR_PER_LINE + z] = 42;
                }
                bits = bits << 1;
            }
        }
        arr[64] = 0;
        printf("%s\n", arr);
    }
}

int main(int argc, char **argv)
{

    char * font_filename = "font8x8.dat";

    if (argc != 2) {
	fprintf(stderr, "Usage: %s <a string>\n", argv[0]);
	return 1;
    }

    // open the font file
    FILE *fp;
    fp = fopen(font_filename, "r");
    if (fp == NULL) {
	fprintf(stderr, "Cannot open font file %s :", font_filename);
        perror("");
        fprintf(stderr, "Run ./generate-fontfile to create it. Do not add it to your repo.\n"); 
	return 2;
    }

    /* There are many ways to do it. 
     * 
     * One strategy can be:
     *
     *      1. Allocate a 'display' buffer, a 2-D array that has FONT_HEIGHT rows.
     *      2. Clear buffer.
     *      3. Call str_render() to render NUM_CHAR_PER_LINE characters into the display buffer.
     *      4. Print the buffer.
     *      5. If there are more characters to display, goto 2.
     *      6. Free buffer.
     * 
     * There are several ways to use buffer. Be consistent in your implementation. 
     */

    //TODO
    char ** buffer = malloc(FONT_HEIGHT* sizeof(char*));
    for(int i = 0; i < FONT_WIDTH; i++){
        buffer[i] = (char*)malloc(sizeof(char)*FONT_WIDTH);
    }

    char * strg = calloc(9, sizeof(char));
    int x = 0, y;
    clear_buffer(buffer);

    while(argv[1][x]){
        y = x%8;
        strg[y] =argv[1][x];

        if ((y==7)){
            str_render(buffer, strg, fp);
            print_buffer(buffer);
            memset(strg, 0, 9);
        }
        x++;
    }

    if(strg[0] != 0){
        str_render(buffer, strg, fp);
        print_buffer(buffer);
    }

    fclose(fp);
    free(strg);

    for(int z = 0; z < NUM_CHAR_PER_LINE; z++){
        free(buffer[z]);
    }

    free(buffer);

    return 0;
}
