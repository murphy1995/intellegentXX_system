#include "main.h"

int gallery()
{
	find_bmp("./photo");
	int bmp_loc = 0;
	while(1)
	{
		int x = 0;
		int y = 0;
		get_ts(&x , & y);
		if(x >=0 && x<= 100 && y >=190 && y<=290 )
		{
			if (bmp_loc == bmp_list_num)
			{
				bmp_loc = 0;
			}
			show_bmp(bmp_list[bmp_loc++]);
		}
		if(x >=700 && x<= 800 && y >=190 && y<=290 )
		{
			if (bmp_loc == -1)
			{
				bmp_loc = bmp_list_num-1;
			}
			show_bmp(bmp_list[bmp_loc--]);
		}
		// while(1)
		// {
		// 	int x = 0;
		// 	int y = 0;
		// 	//get_ts(&x , & y);
			
		// 	if(i==bmp_list_num)
		// 	{
		// 		i=0;
		// 	}
		// 	printf("%s\n",bmp_list[i] );
		// 	show_bmp(bmp_list[i]);
		// 	i++;

			
		// 	sleep(1);
		// }
	}
}

int main(int argc, char const *argv[])
{
	init_ts();

	while(1)
	{
		show_bmp("./ui/01.bmp");
		int x = 0;
		int y = 0;
		get_ts(&x , &y);
		if(x >=190 && x<= 295 && y >=205 && y<=287 )
		{
			gallery();
		}

	}
	
	close_ts();
	return 0;
}