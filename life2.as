#include "std.as"
int SIZEX = 40;
int SIZEY = 20;
array<array<bool>> tmp (SIZEX, array<bool>(SIZEY,false)), scr(SIZEX, array<bool>(SIZEY,false));

void init2()
{
	turn.change_texture("nl.png", b.base_tank_());
	create_field(0,0,SIZEX,SIZEY);
	for(int i=4;i<7;i++)
		scr[26][i] = true;
	scr[27][5] = true;
	for(int i=5;i<8;i++)
		scr[28][i] = true;
}
int func2(int i,int size)
{
	if (i == -1)
		return (size - 1);
	if ((i != -1) && (i != (size)))
		return i;
	return 0;
}
int func(array<array<bool>>tmp,int x,int y)
{
	int z = 0;
	for (int i = x - 1;i < x + 2;i++)
		for (int j = y - 1;j < y + 2;j++)
			if (tmp[func2(i,SIZEX)][func2(j,SIZEY)]!= false)
				z++;
	return z;
}
int main()
{
	if(std_init(null,init2)==1)
		return 0;
	tmp = scr;
	for (int i = 0;i < SIZEX;i++)
		for (int j = 0;j < SIZEY;j++)
		{
			int z=func(tmp, i, j);
			if(z==3 or (z==4 and tmp[i][j] == true))
				scr[i][j] = true;
			else
				scr[i][j] = false;
			turn.set_visible(i,j,scr[i][j]);
		}
	return 0;
}