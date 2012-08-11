#include "dama.h"
/* 
 * File:   main.cpp
 * Author: Dixiw
 *
 * Created on 8. srpen 2012, 20:13
 */



int main(int argc, char** argv)
{
	char opt1;
	int opt2=0;
	bool end=false, repair = false;
	char* tah = new char [100];
	board G_dama;
    std::cout<<"Welcome to program DAMA \n\n";
	while(repair==false)
	{
		std::cout<<"Choose game type: \n";
		std::cout<<"(type s for small [4 pieces])\n";
		std::cout<<"(type m for small [8 pieces])\n";
		std::cout<<"(type l for small [12 pieces])\n";
		std::cout<<"Game type is :"; std::cin>> opt1 ; std::cout<<"\n";
		switch (opt1)
		{
		case 's':
			G_dama.board_start(small);
			break;
		case 'm':
			G_dama.board_start(medium);
			break;
		case 'l':
			G_dama.board_start(large);
			break;
		default:
			std::cout<<"Type correct option next time please.";
		}
		if((opt1=='s')||(opt1=='m')||(opt1=='l')) repair = true;
		else repair = false;
	}
	std::cout<<"Lets the game begin.\n";
	while(end!= true)
	{
		int i;
		C_type col;
		for(i=0;i<2;i++)
		{
			G_dama.drawboard();
			if(i==0)
			{
				col = white;
				std::cout<< "White on move:\n";
			}
			else
			{
				col = black;
				std::cout<<"Black on move:\n";
			}			
			std::cin>> tah;
			std::string s(tah);
			std::string::iterator it1= s.begin();
			switch (s[0])
			{
			case'm':
				int act;
				while(it1!=s.end())
				{
					if(*it1==';')
					{
						pos from, to;
						it1++;
						from.pos_x = toupper(*it1) - 'A'; it1++;
						from.pos_y = *it1 -'1';it1++;it1++;
						to.pos_x = toupper(*it1) - 'A'; it1++;
						to.pos_y = *it1 -'1';it1++;
						if((from.pos_x<0)||(from.pos_x>7)) return -1;
						if((from.pos_y<0)||(from.pos_y>7)) return -1;
						if((to.pos_x<0)||(to.pos_x>7)) return -1;
						if((to.pos_y<0)||(to.pos_y>7)) return -1;
						step from_to ={from,to};
						G_dama.p_move(from_to,col);
					}
					else it1++;
				}
				break;
			case 't':
				pos place;
				place.pos_x = toupper(s[2]) - 'A';
				place.pos_y = s[3] - 1;
				G_dama.transform(place,col);
				break;
			default:
				std::cout<<"Ilegal action - Game over"<< std::endl;
				break;
			}
		}
		G_dama.drawboard();
	}
	system("pause");
    return 0;
}

