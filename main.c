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

short getFromXY(short grid, short x, short y)
{
	return ((grid & 1 << (y * 4 + x)) > 0 ? 1 : 0);
}

int print_grid(unsigned short grid)
{
	for (size_t j = 0; j < 4; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			printf("%d ", getFromXY(grid, i, j));
		}
		printf("\n");
	}
	return (0);
}

struct tetormino
{
	unsigned short grid;
};

unsigned short move_vertical(short grid, short val)
{
	if (val >= 0)
		return (grid << (val << 2));
	val = -val;

	return (grid >> (val << 2)) & (32767 >> ((val << 2) - 1));
}

unsigned short move_horizontal(unsigned short grid, short val)
{
	unsigned short col_l;
	unsigned short col_r;
	unsigned short matrix;
	int i;

	i = 0;
	matrix = 0;
	col_r = (8) | (8 << 4) | (8 << 8) | (8 << 12);
	col_l = (1) | (1 << 4) | (1 << 8) | (1 << 12);
	while (i < val)
	{
		grid = (i == 0) ? grid << val : grid;
		matrix = matrix | (col_l << i);
		i++;
	}
	val = -val;
	i = 0;
	while (i < val)
	{
		grid = (i == 0) ? grid >> val : grid;
		matrix = matrix | (col_r >> i);
		i++;
	}
	matrix = 65535 - matrix;
	return (matrix & grid);
}

short AreIntersect(t_ttr *a, t_ttr *b)
{
	short dx;
	short dy;
	unsigned short temp;

	dx = b->ox - a->ox;
	dy = b->oy - a->oy;
	/*
	if ((dx / 4 | dy / 4))
		return (0);
	 */
	temp = b->grid;
	temp = (dx) ? move_horizontal(temp, dx) : temp;
	temp = (dy) ? move_vertical(temp, dy) : temp;
	/*
	if (a->oy == 2)
	{
		printf("INTERSECT (%d %d) (%d %d) d(%d %d)\n", a->ox, a->oy, b->ox, b->oy, dx, dy);
		print_grid(a->grid);
		print_grid(b->grid);
		print_grid(temp);
		printf("---\n");
	}
	 */
	return (a->grid & temp);
}

unsigned short *InitCols(unsigned short *cols)
{
	cols[0] = (1) | (1 << 4) | (1 << 8) | (1 << 12);
	cols[1] = (1) | (2) | (3) | (4);
	cols[2] = (8) | (8 << 4) | (8 << 8) | (8 << 12);
	cols[3] = (1 << 12) | (2 << 12) | (3 << 12) | (4 << 12);
	return cols;
}

t_ttr *AlignAndCalc(t_ttr *tetr, unsigned short *cols)
{
	short grid = tetr->grid;
	short w = 0;
	short h = 0;
	unsigned short left = cols[0];
	unsigned short top = cols[1];

	while (!(grid & left))
	{
		grid = move_horizontal(grid, -1);
	}

	while (!(grid & top))
	{
		grid = move_vertical(grid, -1);
	}

	h = 1;
	w = 1;

	top = top << 4;

	while (grid & top && h < 4)
	{
		top = top << 4;
		h++;
	}

	left = left << 1;

	while (grid & left && w < 4)
	{
		left = left << 1;
		w++;
	}

	//printf("\nW: %d H: %d\n", w, h);

	tetr->w = w;
	tetr->h = h;

	tetr->grid = grid;

	return tetr;
}

int BIGIntersect(t_ttr **ttrs, short index)
{
	for (short i = 0; i < index; i++)
	{
		if (AreIntersect(ttrs[index], ttrs[i]))
			return (1);
	}
	return (0);
}

short BIG_PRINT(t_ttr **ttrs, int count, short bnd)
{
	char **matrix = (char **)malloc(sizeof(char *) * bnd);
	for (short i = 0; i < bnd; i++)
	{
		matrix[i] = (char *)malloc(sizeof(char) * bnd);
		for (short j = 0; j < bnd; j++)
		{
			matrix[i][j] = '0';
		}
	}

	short ox;
	short oy;

	for (short c = 0; c < count; c++)
	{
		ox = ttrs[c]->ox;
		oy = ttrs[c]->oy;
		for (short j = 0; j < ttrs[c]->h; j++)
		{
			for (short i = 0; i < ttrs[c]->w; i++)
			{
				if (getFromXY(ttrs[c]->grid, i, j))
					matrix[i + ox][j + oy] = 'A' + c;
				//printf("%c ", getFromXY(ttrs[c]->grid, i, j) ? 'A' + c : '0');
			}
			//printf("\n");
		}
		//printf("---\n");
	}

	for (short j = 0; j < bnd; j++)
	{
		for (short i = 0; i < bnd; i++)
		{
			printf("%c ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	return (0);
}

int sml_solver(t_ttr **ttrs, short index, short bnd, short count)
{
	//printf("\nWORKING WITH %d\n", index);
	BIG_PRINT(ttrs, index, bnd);

	for (short j = 0; j <= bnd - ttrs[index]->h; j++)
	{
		for (short i = 0; i <= bnd - ttrs[index]->w; i++)
		{
			ttrs[index]->ox = i;
			ttrs[index]->oy = j;
			//printf("TRYING %d at %d %d\n", index, i, j);

			if (!BIGIntersect(ttrs, index))
			{
				//printf("%d element fits\n", index);
				if (index == count - 1)
				{
					//printf("\nREACHED COUNT\n");
					return (1);
				}
				if (sml_solver(ttrs, index + 1, bnd, count) == 1)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int big_solver(t_ttr **ttrs, short num)
{
	short index = 0;
	short bound = ttrs[0]->w > ttrs[0]->h ? ttrs[0]->w : ttrs[0]->h;

	while (!sml_solver(ttrs, index, bound, num))
	{
		printf("\nBOUND++\n");
		bound++;
	}
	printf("SUCC(EASS)\n");
	return (bound);
}

unsigned short grid_fill(char *str)
{
	printf("GOT A %s\n", str);
	int i = 0;
	unsigned short res = 0;

	while (i < 16)
	{
		res = res | (str[i] - '0') << i;
		i++;
	}
	return (res);
}

int main(int argc, char **argv)
{
	t_ttr *ttrs[26];
	for (short i = 0; i < 26; i++)
	{
		ttrs[i] = 0;
	}

	unsigned short *cols = (unsigned short *)malloc(sizeof(unsigned short) * 4);

	unsigned short a = grid_fill("1000110001000000");
	unsigned short b = grid_fill("1000110001000000");
	unsigned short c = grid_fill("1000110001000000");
	unsigned short d = grid_fill("1000110001000000");
	unsigned short e = grid_fill("1000110001000000");
	unsigned short f = grid_fill("1000110001000000");
	unsigned short g = grid_fill("1000110001000000");
	unsigned short h = grid_fill("1000110001000000");
	unsigned short i = grid_fill("1000110001000000");
	unsigned short j = grid_fill("1000110001000000");
	unsigned short k = grid_fill("1000110001000000");
	unsigned short l = grid_fill("1000110001000000");
	unsigned short m = grid_fill("1000110001000000");
	unsigned short n = grid_fill("1000110001000000");
	unsigned short o = grid_fill("1000110001000000");
	unsigned short p = grid_fill("1000110001000000");
	unsigned short q = grid_fill("1000110001000000");
	unsigned short r = grid_fill("1000110001000000");
	unsigned short s = grid_fill("1000110001000000");
	unsigned short t = grid_fill("1000110001000000");

	t_ttr *a0 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a1 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a2 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a3 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a4 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a5 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a6 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a7 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a8 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a9 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a10 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a11 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a12 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a13 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a14 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a15 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a16 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a17 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a18 = (t_ttr *)malloc(sizeof(t_ttr));
	t_ttr *a19 = (t_ttr *)malloc(sizeof(t_ttr));

	a0->grid = a;
	a0 = AlignAndCalc(a0, cols);
	a1->grid = b;
	a1 = AlignAndCalc(a1, cols);
	a2->grid = c;
	a2 = AlignAndCalc(a2, cols);
	a3->grid = d;
	a3 = AlignAndCalc(a3, cols);
	a4->grid = e;
	a4 = AlignAndCalc(a4, cols);
	a5->grid = f;
	a5 = AlignAndCalc(a5, cols);
	a6->grid = g;
	a6 = AlignAndCalc(a6, cols);
	a7->grid = h;
	a7 = AlignAndCalc(a7, cols);
	a8->grid = i;
	a8 = AlignAndCalc(a8, cols);
	a9->grid = j;
	a9 = AlignAndCalc(a9, cols);
	a10->grid = k;
	a10 = AlignAndCalc(a10, cols);
	a11->grid = l;
	a11 = AlignAndCalc(a11, cols);
	a12->grid = m;
	a12 = AlignAndCalc(a12, cols);
	a13->grid = n;
	a13 = AlignAndCalc(a13, cols);
	a14->grid = o;
	a14 = AlignAndCalc(a14, cols);
	a15->grid = p;
	a15 = AlignAndCalc(a15, cols);
	a16->grid = q;
	a16 = AlignAndCalc(a16, cols);
	a17->grid = r;
	a17 = AlignAndCalc(a17, cols);
	a18->grid = s;
	a18 = AlignAndCalc(a18, cols);
	a19->grid = t;
	a19 = AlignAndCalc(a19, cols);
	ttrs[0] = a0;
	ttrs[1] = a1;
	ttrs[2] = a2;
	ttrs[3] = a3;
	ttrs[4] = a4;
	ttrs[5] = a5;
	ttrs[6] = a6;
	ttrs[7] = a7;
	ttrs[8] = a8;
	ttrs[9] = a9;
	ttrs[10] = a10;
	ttrs[11] = a11;
	ttrs[12] = a12;
	ttrs[13] = a13;
	ttrs[14] = a14;
	ttrs[15] = a15;
	ttrs[16] = a16;
	ttrs[17] = a17;
	ttrs[18] = a18;
	ttrs[19] = a19;

	cols = InitCols(cols);
	short num = 11;

	printf("SOLVING STARTED\n");
	unsigned short bnd = big_solver(ttrs, num);

	printf("BND: %d\n", bnd);

	/*
	ttrs[0]->ox = 2;
	ttrs[0]->oy = 0;

	ttrs[1]->ox = 0;
	ttrs[1]->oy = 2;

	ttrs[2]->ox = 6;
	ttrs[2]->oy = 0;

	ttrs[3]->ox = 6;
	ttrs[3]->oy = 0;
 */
	//BIG_PRINT(ttrs, 4, 9);

	BIG_PRINT(ttrs, num, bnd);
	/*
	for (size_t i = 0; i < 4; i++)
	{
		print_grid_abc(ttrs[i]->grid, i);
		printf("X: %d Y: %d\n---\n\n", ttrs[i]->ox, ttrs[i]->oy);
	} */
	return (0);
}
