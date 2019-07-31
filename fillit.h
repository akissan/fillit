#include <stdio.h>
#include <stdlib.h>

typedef struct s_tetrushka
{
	unsigned short grid;
	short ox;
	short oy;
	unsigned char w;
	unsigned char h;
} t_ttr;

int print_grid(unsigned short grid);


short BIG_PRINT(t_ttr **ttrs, int count, short bnd);

short getFromXY(short grid, short x, short y);



unsigned short move_vertical(short grid, short val);

unsigned short move_horizontal(unsigned short grid, short val, unsigned short *cols);


short AreIntersect(t_ttr *a, t_ttr *b, unsigned short *cols);


unsigned short *InitCols(unsigned short *cols);



unsigned short grid_fill(char *str);