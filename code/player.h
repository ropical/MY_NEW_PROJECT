/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 宏定义
#define MIN -0x3f3f3f3f
#define MANHATAN_DISTANCE(X, Y) ((X) + (Y))

// 全局变量定义
int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
int **weight_map;


// 函数声明
// 这个功能在开始的时候执行一次 --[done][must]
void init(struct Player *player);  

//判断是否在函数体内 --[done] [in_workable]
bool judge_move(int target_x, int target_y, struct Player *player);

//是否在地图内 --[done] [workable]
bool judge_in_map(int target_x, int target_y, struct Player *player);

//最重要的函数，判断下一步走哪里 --[hang_in]
struct Point walk(struct Player *player);

//初始化权重图 --[done] [workable]
void init_weight_map(struct Player *player ,int **weight_map);

//释放权重图空间 --[done][workable]
void free_map(struct Player *player, int **weight_map);


//函数定义
void init(struct Player *player) {

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


void init_weight_map(struct Player *player ,int **weight_map) {

	//动态分配内存空间
	weight_map = (int **)malloc(sizeof(int *) * player->your_posx);
	for(int i = 0; i < player->your_posx; i++) {
		weight_map[i] = (int *)malloc(sizeof(int) * player->your_posy);
	}

	memset(weight_map, 0, sizeof(weight_map));

	for(int i = 0; i < player->your_posx; i++) {
		for(int j = 0; j < player->your_posy; j++) {
			if (player->mat[i][j] == '#') {
				weight_map[i][j] = MIN;
			} else if (player->mat[i][j] == 'o' || player->mat[i][j] == 'O') {
				weight_map[i][j] = MANHATAN_DISTANCE(fabs(i - player->your_posx), fabs(j - player->your_posy));
			} else {
				
			}
		}
	}
}


void void free_map(struct Player *player, int **weight_map) {
	for(int i = 0; i < player->your_posx; i++) {
		for(int j = 0; j < player->your_posy; j++) {
			free(map[i][j]);
		}
	}

	free(map);
}


struct Point walk(struct Player *player) {
	// 	// This function will be executed in each round.
 	// printf("pos %d %d\1n", player->your_posx, player->your_posy);

	// 	return initPoint(player->your_posx, player->your_posy);
}


// struct Point walk(struct Player *player) {
// 	// for (int i = 0; i < player->row_cnt; i++) {
// 	// 	for (int j = 0; j < player->col_cnt; j++) {
// 	// 		printf("%c", player->mat[i][j]);
// 	// 	}
// 	// 	printf("\n");
// 	// }


// 	for (int i = 0; i < 4; i++) {
// 		int direction_x = player->your_posx + step[i][0], direction_y = player->your_posy + step[i][1];

// 		// 第一步判断， 往哪边走;
// 		if (judge_in_map(direction_x, direction_y, player) && judge_move(direction_x, direction_y, player)) {
// 			for (int j = 0; j < 4; j++) {
// 				int step_judge_x = direction_x + step[j][0];
// 				int step_judge_y = direction_y + step[j][1];
// 				// 第一步之后是否进死胡同， 确保蛇活下来;
// 				if (judge_move(step_judge_x, step_judge_y, player)) {   
// 					return initPoint(direction_x, direction_y);
// 				}
// 			}
// 		}
// 	}

// }




