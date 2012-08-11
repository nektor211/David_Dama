/* 
 * File:   dama.h
 * Author: Dixiw
 *
 * Created on 8. srpen 2012, 20:23
 */

#ifndef DAMA_H
#define	DAMA_H

#include <iostream>
#include <cstring>

const int hist_len = 5;

const char Pbp = 'x', Pbq = 'X', Pwp = 'y', Pwq = 'Y'; //Piece -black/white - pawn/qeen
const char Bb = '#', Bw = '*'; //Piece -black/white
const char Etl = '=', Etr = '=', Ebl = '=', Ebr = '='; //Edge -top/botton/ - left/right
const char Elr = '-', Etb = '|'; // Edge -leftright / topbotton

typedef struct position
{
    int pos_x;
    int pos_y;
}pos;
typedef struct stepover
{
    pos from ;
    pos to ;
}step;

typedef enum color_type
{
    white = 0,
    black
}C_type;

typedef enum piece_type
{
    pawn = 0,
    queen
}P_type;

typedef enum game_type
{
    small = 4,
    medium = 8,
    large = 12
}G_type;

class piece
{
private:
    pos place;
    C_type colour;
    P_type type;    
public:
    piece()
    {        
    };
    piece(pos pla, C_type col, P_type typ):place(pla),colour(col),type(typ)
    {        
    };
    ~piece()
    {        
    };
    const int get_colour ()
    {
        return colour;
    };
    bool is_pawn ()
    {
        if(type==0) return true;
        else return false;
    };
    pos get_place()
    {
        return place;
    };
    void set_place(int pos_x, int pos_y)
    {
      place.pos_x= pos_x;
      place.pos_y= pos_y;
    };
};

class board
{
private:
    int b_pcs_cnt;
    int w_pcs_cnt;
    //piece b_pcs_pos [12];
    //piece w_pcs_pos [12];
    //step history [hist_len];
    char* desk ;
    /*************************/
    void p_place(pos where,C_type col,P_type type);
public:
    board()
    {
		desk = NULL;
		b_pcs_cnt = 0;
		w_pcs_cnt = 0;
    };
    void board_start(G_type game);
    ~board()
    {        
    };
    void drawboard();
    
    /*************************************/
    int p_move (step from_to, C_type colour);
    void start(G_type);
    int transform (pos place, C_type colour);
    void win();    
};

/*-----------------------------------------*/

void board::board_start(game_type game)
{
	desk = new char [64];
	int i,j,k;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
			if((i+j)%2 ==0) desk[j+ 8*i]=Bb;
            else desk[j+ 8*i]=Bw;
        }
    }
    b_pcs_cnt = w_pcs_cnt = game;
       
    for (k=0;k<game/4;k++)
    {
        for(j=0;j<4;j++)
        {
            pos w_akt = {k,j*2-(k%2)};
            pos b_akt = {7-k,j*2+1-(k%2)};
            piece w_piece(w_akt,white,pawn);
            piece b_piece(b_akt,white,pawn);
            int akt = 4*k+j;
            //w_pcs_pos[akt] = w_piece; 
            //b_pcs_pos[akt] = b_piece;
            p_place(w_akt,white,pawn);
            p_place(b_akt,black,pawn);
        }
    }
        
};

void board::p_place(pos where, C_type col, P_type typ)
{
    if(col==white)
    {
        if (typ == pawn) desk[8* where.pos_x + where.pos_y] = Pwp;
        else desk[8* where.pos_x + where.pos_y] = Pwq;
    }
    else 
    {
        if (typ == pawn) desk[8* where.pos_x + where.pos_y] = Pbp;
        else desk[8* where.pos_x + where.pos_y] = Pbq;
    }
};

void board::drawboard()
{
    std::cout<<" " << Etl << Elr << Elr << Elr << Elr << Elr << Elr << Elr << Elr << Etr <<" \n";
    int i,j;
    for(i=0;i<8;i++)
        {
        std::cout<< i+1 << Etb; 
            for(j=0;j<8;j++)
            {
                std::cout<< desk[8*i + j];
            }
        std::cout<< Etb << "\n";
        }
   std::cout<<" " << Ebl << Elr << Elr << Elr << Elr << Elr << Elr << Elr << Elr << Ebr <<"\n";
   std::cout<< "  ABCDEFGH"<<std::endl;
};

int board::p_move(step from_to, C_type colour)
{
	int fX =from_to.from.pos_x,fY =from_to.from.pos_y;
	int tX =from_to.to.pos_x,tY =from_to.to.pos_y;
	int from = 8*fX + fY, to = 8*tX + tY;
	if( (fX >7)||(fX <0)||(fY >7)||(fY <0) ) return -1; //je to mimo hraci pole
	if( (tX >7)||(tX <0)||(tY >7)||(tY <0) ) return -1; //je to mimo hraci pole
	if((fX+fY)%2 !=0 ) return -2; //neni to na cernych
	if((tX+tY)%2 !=0 ) return -2; //neni to na cernych
	if(desk[to]!=Bb) return -3 ;//nelze stoupnout na policko kde uz njk figura stoji
	/********************/
	if((desk[from] == Pbp)||(desk[from] == Pwp)) // zda je to pohyb pesce
	{
		if(abs(tX-fX)==abs(tY-fY)) // je to diagonalni pohyb
		{
			if(abs(tX-fX)==1) // pokud je to jen pohyb
			{
				desk[to] = desk[from];
				desk[from] = Bb;		
			}
			else if(abs(tX-fX)==2) // pokud je to sebrani jen jedne
			{
				int tmp = 4*(tX+fX)+(tY+fY); 
				if ((from==Pbp)&&(colour==black)) // jedna se o cerneho pesce
				{
					if ((desk[tmp]!=Pwp)&&(desk[tmp]!=Pwq)) return -4; //nejde sebrat vlastni figurku
					desk[to] = desk[from];
					desk[from] = Bb;	
					desk[tmp] = Bb;
					w_pcs_cnt --;
				}
				else if((from==Pwp)&&(colour==white))
				{
					if ((desk[tmp]!=Pbp)&&(desk[tmp]!=Pbq)) return -4;
					desk[to] = desk[from];
					desk[from] = Bb;	
					desk[tmp] = Bb;
					b_pcs_cnt --;
				}
				else return -5; //hrac hraje za cizi figury
			}
			else // sebrani vice figur se resi separatne jako vice sebrani jedne
			{
				return -6; // skace se o vic poli nez je mozne
			}
		}
		else
		{
			return -7; // neni to diagonalni pohyb
		}

	}
	else // pohyb damou
	{
		if(abs(tX-fX)==abs(tY-fY)) // je to diagonalni pohyb
		{
			int i,pcnt=0,diag = tX-fX;
			
			if((desk[from]==Pbq)&&(colour==black))//jedna se o cernou damu
			{
				for(i=1;i<diag;i++)
				{
					int akt = 8*(fX+i) + (fY+i);
					if((desk[akt]==Pbp)||(desk[akt]==Pbq)) return -8;// nejde sebrat damou vlastni figuru
					if((desk[akt]==Pwp)||(desk[akt]==Pwq)) pcnt++;					
				}
				if(pcnt>1) return -9; //nejde sebrat vic figur najednou
				for(i=1;i<diag;i++)
				{
					int akt = 8*(fX+i) + (fY+i);
					desk[akt] = Bb;
				}
				w_pcs_cnt --;
			}
			else if((desk[from]==Pwq)&&(colour==white)) //jedna se o bilou damu
			{
				for(i=1;i<diag;i++)
				{
					int akt = 8*(fX+i) + (fY+i);
					if((desk[akt]==Pwp)||(desk[akt]==Pwq)) return -8;// nejde sebrat damou vlastni figuru
					if((desk[akt]==Pbp)||(desk[akt]==Pbq)) pcnt++;					
				}
				if(pcnt>1) return -9; //nejde sebrat vic figur najednou
				for(i=1;i<diag;i++)
				{
					int akt = 8*(fX+i) + (fY+i);
					desk[akt] = Bb;
				}
				b_pcs_cnt --;
			}
			else return -5; 
			
			
		}
		else
		{
			return -7; // neni to diagonalni pohyb
		}
	}
	return 0;
}

int board::transform(pos place,C_type colour)
{
	int akt= 8* place.pos_x + place.pos_y;
	if( (place.pos_x >7)||(place.pos_x <0)||(place.pos_y >7)||(place.pos_y <0) ) return -1; //je to mimo hraci pole
	if((place.pos_x+place.pos_y)%2 !=0 ) return -2; //neni to na cernych
	if(desk[akt]==Pbp)
	{
		if(colour!=black) return -3; //neni to pesak odpovidajici barvy
		{
			if(place.pos_y!=0)return -4; //nejde menit jinde nez na konci
			desk[akt]= Pbq;
		}
	}
	else if(desk[akt]==Pwp)
	{
		if(colour!=white) return -3; //neni to pesak odpovidajici barvy
		{
			if(place.pos_y!=7)return -4; //nejde menit jinde nez na konci
			desk[akt]= Pwq;
		}
	}
	else return -5; //tuto figuru uz nelze menit

}

#endif	/* DAMA_H */

