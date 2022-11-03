// 2048 CLI for Linux/Unix
// By Half-Qilin AKA Hail

#include <iostream>
#include <time.h>

#include "2048.cpp"

char move;
int id;
bool end = false;

int main() {
	game_2048::setRandom(time(NULL));
	while(game_2048::gameRunning() && !end) {
		std::cout << "╔══════╤══════╤══════╤══════╗" << std::endl;
		for(int i = 0; i < 4; i++) {
			std::cout << "║";
			for (int j = 0; j < 4; j++) {
				if (game_2048::board[i][j] < 1) std::cout << "      ";
				else if (game_2048::board[i][j] < 4) std::cout << "   \033[1;37m" << (1UL << game_2048::board[i][j]) << "\033[0m  ";
				else if (game_2048::board[i][j] < 7) std::cout << "  \033[1;36m" << (1UL << game_2048::board[i][j]) << "\033[0m  ";
				else if (game_2048::board[i][j] < 10) std::cout << "  \033[1;35m" << (1UL << game_2048::board[i][j]) << "\033[0m ";
				else if (game_2048::board[i][j] == 10) std::cout << " \033[1;32m" << (1UL << game_2048::board[i][j]) << "\033[0m ";
				else if (game_2048::board[i][j] < 13) std::cout << " \033[1;33m" << (1UL << game_2048::board[i][j]) << "\033[0m ";
				else if (game_2048::board[i][j] == 13) std::cout << "\033[1;33;7m " << (1UL << game_2048::board[i][j]) << " \033[0m";
				else if (game_2048::board[i][j] < 16) std::cout << "\033[1;33;7m " << (1UL << game_2048::board[i][j]) << "\033[0m";
				else if (game_2048::board[i][j] < 20) {
					std::cout << "\033[1;7m";
					if (game_2048::board[i][j] == 16) std::cout << " ";
					std::cout << (1UL << game_2048::board[i][j]) << "\033[0m";
				} else {
					std::cout << "\033[1;7m 2^" << (int)game_2048::board[i][j];
					if (game_2048::board[i][j] < 100) std::cout << " ";
					std::cout << "\033[0m";
				}
				if (j != 3) std::cout << "│";
			}
			if (i != 3) std::cout << "║" << std::endl << "╟──────┼──────┼──────┼──────╢" << std::endl;
		}
		std::cout << "║"  << std::endl << "╚══════╧══════╧══════╧══════╝" << std::endl;
		std::cout << "▷ Select your move: ";
		std::cin >> move;
		switch(move) {
			case 'a': {
				id = 1;
				break;
			}
			case 'w': {
				id = 0;
				break;
			}
			case 's': {
				id = 2;
				break;
			}
			case 'd': {
				id = 3;
				break;
			}
			case 'e': {
				end = true;
			}
			default: {
				id = -1;
				break;
			}
		}
		game_2048::doStep(id);
		std::cout << std::endl;
	}
	std::cout << "\033[1;31m╔══════╤══════╤══════╤══════╗" << std::endl;
	for(int i = 0; i < 4; i++) {
		std::cout << "║";
		
			for (int j = 0; j < 4; j++) {
				if (game_2048::board[i][j] < 1) std::cout << "      ";
				else if (game_2048::board[i][j] < 4) std::cout << "   " << (1UL << game_2048::board[i][j]) << "  ";
				else if (game_2048::board[i][j] < 7) std::cout << "  " << (1UL << game_2048::board[i][j]) << "  ";
				else if (game_2048::board[i][j] < 10) std::cout << "  " << (1UL << game_2048::board[i][j]) << " ";
				else if (game_2048::board[i][j] == 10) std::cout << " " << (1UL << game_2048::board[i][j]) << " ";
				else if (game_2048::board[i][j] < 13) std::cout << " " << (1UL << game_2048::board[i][j]) << " ";
				else if (game_2048::board[i][j] == 13) std::cout << " " << (1UL << game_2048::board[i][j]) << " ";
				else if (game_2048::board[i][j] < 16) std::cout << " " << (1UL << game_2048::board[i][j]) << "";
				else if (game_2048::board[i][j] < 20) {
					std::cout << "";
					if (game_2048::board[i][j] == 16) std::cout << " ";
					std::cout << (1UL << game_2048::board[i][j]) << "";
				} else {
					std::cout << " 2^" << (int)game_2048::board[i][j];
					if (game_2048::board[i][j] < 100) std::cout << " ";
				}
				if (j != 3) std::cout << "│";
			}
		if (i != 3) std::cout << "║" << std::endl << "╟──────┼──────┼──────┼──────╢" << std::endl;
	}
	std::cout << "║"  << std::endl << "╚══════╧══════╧══════╧══════╝\033[0m" << std::endl;
	std::cout << "Game Over!\nYou scored " << displayNumbMaxDigits(game_2048::score, 7) << " points!\n";
}