/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdbool.h>

// 宏定义


// 全局变量定义
int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};


// 函数声明
void init(struct Player *player); 

bool judge_move(int target_x, int target_y, struct Player *player);

bool judge_in_map(int target_x, int target_y, struct Player *player);

struct Point walk(struct Player *player);


void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
} 


bool judge_move(int target_x, int target_y, struct Player *player) {
	if(player->mat[target_x][target_y] != '#' && player->mat[target_x][target_y] != '1') {
		return true;
	}

	return false;
}


bool judge_in_map(int target_x, int target_y, struct Player *player) {
	if(target_x >= 0 && target_x < player->row_cnt && target_y >= 0 && target_y < player->col_cnt) {
		return true;
	}

	return false;
}


struct Point walk(struct Player *player) {
	// for (int i = 0; i < player->row_cnt; i++) {
	// 	for (int j = 0; j < player->col_cnt; j++) {
	// 		printf("%c", player->mat[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// This function will be executed in each round.
	// printf("pos %d %d\1n", player->your_posx, player->your_posy);

	for (int i = 0; i < 4; i++) {
		int direction_x = player->your_posx + step[i][0], direction_y = player->your_posy + step[i][1];

		// 第一步判断， 往哪边走;
		if (judge_in_map(direction_x, direction_y, player) && judge_move(direction_x, direction_y, player)) {
			for (int j = 0; j < 4; j++) {
				int step_judge_x = direction_x + step[j][0];
				int step_judge_y = direction_y + step[j][1];
				// 第一步之后是否进死胡同， 确保蛇活下来;
				if (judge_move(step_judge_x, step_judge_y, player)) {   
					return initPoint(direction_x, direction_y);
				}
			}
		}
	}
	return initPoint(player->your_posx, player->your_posy);
}
