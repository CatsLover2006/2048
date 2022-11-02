// 2048 Core Logic
// By Chance Parsons AKA Half-Qilin AKA Hail

#include <stdlib.h>

#include "numberManagement.hpp"

namespace game_2048 {

	uint8_t board[4][4];
	bool moved[4][4];
	bool generated[4][4];
	unsigned long long score = 0;
	
	void setRandom(int seed) {
		srand(seed);
	}
	
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
	
	void doMove(int dir) {
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) moved[i][j] = false;
		switch(dir) {
			case 1: {
				for(int i = 0; i < 4; i++) {
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == 0) {
							board[i][j-1] = board[i][j];
							board[i][j] = 0;
							moved[i][j] = true;
							j--;
						}
					}
					for (int j = 1; j < 4; j++) {
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == board[i][j]) {
							board[i][j-1]++;
							board[i][j] = 0;
							moved[i][j] = true;
							j--;
							score += pow(2, board[i][j]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j-1] == 0) {
							board[i][j-1] = board[i][j];
							board[i][j] = 0;
							moved[i][j] = true;
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
							moved[j][i] = true;
							j--;
						}
					}
					for (int j = 1; j < 4; j++) {
						if (board[j][i] == 0) continue;
						if (board[j-1][i] == board[j][i]) {
							board[j-1][i]++;
							board[j][i] = 0;
							moved[j][i] = true;
							j--;
							score += pow(2, board[j][i]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 1; j < 4; j++) {
						if (j == 0) continue;
						if (board[j][i] == 0) continue;
						if (board[j-1][i] == 0) {
							board[j-1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i] = true;
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
							moved[i][j] = true;
							j++;
						}
					}
					for (int j = 2; j > -1; j--) {
						if (board[i][j] == 0) continue;
						if (board[i][j+1] == board[i][j]) {
							board[i][j+1]++;
							board[i][j] = 0;
							moved[i][j] = true;
							j++;
							score += pow(2, board[i][j]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[i][j] == 0) continue;
						if (board[i][j+1] == 0) {
							board[i][j+1] = board[i][j];
							board[i][j] = 0;
							moved[i][j] = true;
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
							moved[j][i] = 2;
							j++;
						}
					}
					for (int j = 2; j > -1; j--) {
						if (board[j][i] == 0) continue;
						if (board[j+1][i] == board[j][i]) {
							board[j+1][i]++;
							board[j][i] = 0;
							moved[j][i] = true;
							j++;
							score += pow(2, board[j][i]);
						}
					}
					for (int t = 0; t < 2; t++) for (int j = 2; j > -1; j--) {
						if (j == 3) continue;
						if (board[j][i] == 0) continue;
						if (board[j+1][i] == 0) {
							board[j+1][i] = board[j][i];
							board[j][i] = 0;
							moved[j][i] = true;
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
	
	void doStep(int dir) {
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
	}
}