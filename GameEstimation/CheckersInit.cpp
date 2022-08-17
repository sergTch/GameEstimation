#include "Checkers.h"

/*

Cell IDs:

  28  29  30  31  
24  25  26  27    
  20  21  22  23  
16  17  18  19  
  12  13  14  15 
08  09  10  11
  04  05  06  07
00  01  02  03


Directions:

0   1
  X
3   2
//*/

std::vector<uint32_t> Checkers::connections[32][4] = {};
int Checkers::field[8][8] = {};

void Checkers::ConnectionsInit() {
	int t = 0;
	for (int i = 0; i < 8; i++)
		for (int j = i % 2; j < 8; j += 2)
			field[i][j] = t++;

	for (int i = 0; i < 8; i++) {
		for (int j = i % 2; j < 8; j += 2) {
			for (int dir = 0; dir < 4; dir++)
				Checkers::connections[0][0] = std::vector<uint32_t>(0);
				
			for (int t = 1; i + t < 8 && j - t >= 0; t++)
				connections[field[i][j]][0].push_back(field[i + t][j - t]);
			for (int t = 1; i + t < 8 && j + t < 8; t++)
				connections[field[i][j]][1].push_back(field[i + t][j + t]);
			for (int t = 1; i - t >= 0 && j + t < 8; t++)
				connections[field[i][j]][2].push_back(field[i - t][j + t]);
			for (int t = 1; i - t >= 0 && j - t >= 0; t++)
				connections[field[i][j]][3].push_back(field[i - t][j - t]);
		}
	}
}