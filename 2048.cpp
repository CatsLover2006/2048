// 2048 Core Logic
// By Half-Qilin AKA Hail

#include <stdlib.h>

#include "numberManagement.hpp"

namespace game_2048 {

	uint8_t board[4][4];
	uint8_t oldBoard[4][4];
	uint8_t largest_numb;
	int moved[4][4];
	int lastMovement;
	bool generated[4][4];
	unsigned long long score = 0;
	
	void addTile() {
		bool l = false;
		int x, y;
		while (!l) {
			x = rand() % 4;
			y = rand() % 4;
			if (board[x][y] == 0) {
				if (rand()%10 == 0) board[x][y] = 2;
				else board[x][y] = 1;
				generated[x][y] = true;
				l = true;
			}
		}
	}
	
	void setRandom(int seed) {
		largest_numb = 1;
		srand(seed);
		for (int i = 0; i < 2; i++) addTile();
	}
	
	void doMove(int dir) {
		lastMovement = dir;
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
			oldBoard[i][j] = board[i][j];
			moved[i][j] = 0;
		}
		switch(dir) {
			case 1: {
				for(int i = 0; i < 4; i++) {
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == 0) {
							board[i][j-1] = board[i][j];
							board[i][j] = 0;
							moved[i][j]++;
							j--;
						}
					}
					for (int j = 1; j < 4; j++) {
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == board[i][j]) {
							board[i][j-1]++;
							board[i][j] = 0;
							moved[i][j]++;
							j--;
							if (largest_numb < board[i][j]) largest_numb = board[i][j];
							score += pow(2, board[i][j]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == 0) {
							board[i][j-1] = board[i][j];
							board[i][j] = 0;
							moved[i][j]++;
							j--;
						}
					}
				}
				return;
			}
			case 0: {
				for(int i = 0; i < 4; i++) {
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[j][i] == 0) continue;
						if (board[j-1][i] == 0) {
							board[j-1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i]++;
							j--;
						}
					}
					for (int j = 1; j < 4; j++) {
						if (board[j][i] == 0) continue;
						if (board[j-1][i] == board[j][i]) {
							board[j-1][i]++;
							board[j][i] = 0;
							moved[j][i]++;
							j--;
							if (largest_numb < board[j][i]) largest_numb = board[j][i];
							score += pow(2, board[j][i]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[j][i] == 0) continue;
						if (board[j-1][i] == 0) {
							board[j-1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i]++;
							j--;
						}
					}
				}
				return;
			}
			case 3: {
				for(int i = 0; i < 4; i++) {
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j+1] == 0) {
							board[i][j+1] = board[i][j];
							board[i][j] = 0;
							moved[i][j]++;
							j++;
						}
					}
					for (int j = 2; j > -1; j--) {
						if (board[i][j] == 0) continue;
						if (board[i][j+1] == board[i][j]) {
							board[i][j+1]++;
							board[i][j] = 0;
							moved[i][j]++;
							j++;
							if (largest_numb < board[i][j]) largest_numb = board[i][j];
							score += pow(2, board[i][j]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j+1] == 0) {
							board[i][j+1] = board[i][j];
							board[i][j] = 0;
							moved[i][j]++;
							j++;
						}
					}
				}
				return;
			}
			case 2: {
				for(int i = 0; i < 4; i++) {
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[j][i] == 0) continue;
						if (board[j+1][i] == 0) {
							board[j+1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i]++;
							j++;
						}
					}
					for (int j = 2; j > -1; j--) {
						if (board[j][i] == 0) continue;
						if (board[j+1][i] == board[j][i]) {
							board[j+1][i]++;
							board[j][i] = 0;
							moved[j][i]++;
							j++;
							if (largest_numb < board[j][i]) largest_numb = board[j][i];
							score += pow(2, board[j][i]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[j][i] == 0) continue;
						if (board[j+1][i] == 0) {
							board[j+1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i]++;
							j++;
						}
					}
				}
				return;
			}
		}
	}

	bool gameRunning() {
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (board[i][j] == 0) return true;
		bool ret = false;
		int tempBoard[4][4];
		int tempMov[4][4];
		unsigned long long tScore = score;
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
			tempBoard[i][j] = board[i][j];
			tempMov[i][j] = moved[i][j];
		}
		doMove(0);
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (board[i][j] == 0) ret = true;
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) board[i][j] = tempBoard[i][j];
		doMove(1);
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (board[i][j] == 0) ret = true;
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) board[i][j] = tempBoard[i][j];
		doMove(2);
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (board[i][j] == 0) ret = true;
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) board[i][j] = tempBoard[i][j];
		doMove(3);
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (board[i][j] == 0) ret = true;
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
			board[i][j] = tempBoard[i][j];
			moved[i][j] = tempMov[i][j];
		}
		score = tScore;
		return ret;
	}
	
	bool doStep(int dir) {
		int tempBoard[4][4];
		for(int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
			tempBoard[i][j] = board[i][j];
			generated[i][j] = false;
		}
		bool identical = true;
		doMove(dir);
		for(int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (tempBoard[i][j] != board[i][j]) {
					identical = false;
				}
			}
		}
		if (!identical) addTile();
		return !identical;
	}
}