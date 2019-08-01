#include "fillit.h"

int print_grid(unsigned short grid)
{
	for (unsigned short j = 0; j < 4; j++)
	{
		for (unsigned short i = 0; i < 4; i++)
		{
			printf("%d ", getFromXY(grid, i, j));
		}
		printf("\n");
	}
	return (0);
}

short BIG_PRINT(t_ttr **ttrs, int count, short bnd)
{
	short i;
	short j;
	char **matrix = (char **)malloc(sizeof(char *) * bnd);

	j = 0;
	while (j < bnd)
	{
		matrix[j] = (char *)malloc(sizeof(char) * bnd);
		i = 0;
		while (i < bnd)
		{
			matrix[j][i] = '0';
			i++;
		}
		j++;
	}

	short ox;
	short oy;
	short c;

	c = 0;
	while (c < count)
	{
		ox = ttrs[c]->ox;
		oy = ttrs[c]->oy;
		j = 0;
		while (j < ttrs[c]->h)
		{
			i = 0;
			while (i < ttrs[c]->w)
			{
				if (getFromXY(ttrs[c]->grid, i, j))
					matrix[i + ox][j + oy] = 'A' + c;
				printf("%d ", getFromXY(ttrs[c]->grid, i, j));
				i++;
			}
			j++;
			printf("\n");
		}
		c++;
	}

	j = 0;
	while (j < bnd)
	{
		i = 0;
		while (i < bnd)
		{
			printf("%c ", matrix[j][i]);
			i++;
		}
		j++;
		printf("\n");
	}
	printf("\n");

	return (0);
}

unsigned short *InitCols(unsigned short *cols)
{
	cols[0] = (1) | (1 << 4) | (1 << 8) | (1 << 12);
	cols[1] = (1) | (2) | (3) | (4);
	cols[2] = (8) | (8 << 4) | (8 << 8) | (8 << 12);
	cols[3] = (1 << 12) | (2 << 12) | (3 << 12) | (4 << 12);
	return cols;
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
