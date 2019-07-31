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
