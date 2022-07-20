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
#include <math.h>

//--------------------------------------------------------宏定义------------------------------------------------------

#define INFINITY 0x3f3f3f3f
#define MAX_SIZE 1000000

//--------------------------------------------------------宏定义------------------------------------------------------


//-----------------------------------------------------全局变量定义----------------------------------------------------

int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};

typedef struct node {
	int x;
	int y;
} snake;

int front, rear;

struct node snake_body_queue[MAX_SIZE];

//-----------------------------------------------------全局变量定义----------------------------------------------------


//-------------------------------------------------------函数声明------------------------------------------------------

// 这个功能在开始的时候执行一次， 初始化蛇身（队列存储） --[done][must]
void init(struct Player *player);

// 更新蛇身，分两种情况
void update_snake(struct Player *player, int flag);

// bfs搜索最大连通块 
int maximum_connected_block(struct Player *player, int start_x, int start_y);

// 判断是否在函数体内 --[done] [in_workable]
bool judge_move(int target_x, int target_y, struct Player *player);

// 是否在地图内 --[done] [workable]
bool judge_in_map(int target_x, int target_y, struct Player *player);

// ver0.10蠢蛇走法应用函数 --[done]
bool judge_real_move(int target_x, int target_y, struct Player *player);

// 曼哈顿距离 --[bug][done]
int direction_score(struct Player *player);

// 最重要的函数，判断下一步走哪里 --[hang_in]
struct Point walk(struct Player *player);

// 初始化权重图 --[done] [workable] [not to use]
void init_weight_map(struct Player *player, int **weight_map); 

// 释放权重图空间 --[done][workable] [not to use]
void free_map(struct Player *player, int **weight_map);

//-------------------------------------------------------函数声明------------------------------------------------------


//-------------------------------------------------------函数定义------------------------------------------------------

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
	front = 0; rear = 0;
	struct node tmp = {player->your_posx, player->your_posy};
	snake_body_queue[rear++] = tmp;
}


void update_snake(struct Player *player, int status) {
	//其中的status用来判断四种状态
	
}


int maximum_connected_block(struct Player *player, int start_x, int start_y) {
	// 开辟占空间和visit数组
	int front = 0, rear = 0, cnt = 0;
	struct node *queue = (struct node *)malloc(sizeof(struct node)*(player->row_cnt * player->col_cnt));
	int **vis = (int **)malloc(sizeof(int *)*player->row_cnt);
	for(int i = 0; i < player->row_cnt; i++) {
		vis[i] = (int *)malloc(sizeof(int)*player->col_cnt);
	}
	memset(vis, 0, sizeof(vis));

	// bfs求最大连通块大小
	struct node tmp = {start_x, start_y};
	queue[rear++] = tmp;
	cnt++;
	vis[start_x][start_y] = 1;
	while(front < rear) {
		struct node now = queue[front++];
		for(int i = 0; i < 4; i++) {
			int direction_x = now.x + step[i][0];
			int direction_y = now.y + step[i][1];
			if(!vis[direction_x][direction_y] && judge_in_map(direction_x, direction_y, player) && judge_move(direction_x, direction_y, player)) {  // --[bug] <judge_move()> 蛇尾可以走
				vis[direction_x][direction_y] = 1;
				struct node tmp = {direction_x, direction_y};
				cnt++;
			}
 		}
	}

	// 释放动态分配内存
	for(int i = 0; i < player->row_cnt; i++) {
		for(int j = 0 ; j < player->col_cnt; j++) {
			free(vis[i]);
		}
	}
	free(vis);
	free(queue);

	return cnt; // **
}


bool judge_move(int target_x, int target_y, struct Player *player) {
	//蛇尾可以走 需要修改 --[hang in]
	if (player->mat[target_x][target_y] != '#' && player->mat[target_x][target_y] != '1' && player->mat[target_x][target_y] != '2') {
		return true;
	}

	return false;
}


bool judge_in_map(int target_x, int target_y, struct Player *player) {
	int wall = player->round / 40;
	if (player->round_to_shrink == 1) {
		wall++;
	}

	if (target_x >= wall && target_x < player->row_cnt - wall && target_y >= wall && target_y < player->col_cnt - wall) {
		return true;
	}

	return false;
}


bool judge_real_move(int target_x, int target_y, struct Player *player) {
	if (judge_move(target_x, target_y, player))
	{
		for (int j = 0; j < 4; j++)
		{
			int step_judge_x = target_x + step[j][0];
			int step_judge_y = target_y + step[j][1];
			if (judge_in_map(step_judge_x, step_judge_y, player) && judge_move(step_judge_x, step_judge_y, player))
			{
				if (player->round_to_shrink == 2)
				{
					int wall = player->round / 40;
					if (step_judge_x < 9 - wall && step_judge_x > wall && step_judge_y < 9 - wall && step_judge_y > wall)
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
	}
	return false;
}


int direction_score(struct Player *player) {
	int direction[4] = {0};
	for (int k = 0; k < 4; k++)
	{
		int dx = player->your_posx + step[k][0], dy = player->your_posy + step[k][1];
		//绝对不能走 直接启用ver0.10
		if (!judge_in_map(dx, dy, player) || !judge_real_move(dx, dy, player))
		{
			direction[k] = INFINITY;
			continue;
		}
		for (int i = 0; i < player->row_cnt; i++)
		{
			for (int j = 0; j < player->col_cnt; j++)
			{
				if (player->mat[i][j] == 'o')
				{
					// printf("direction[%d] += %d ", k, abs(i - player->your_posx) + abs(j - player->your_posy));
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction[k] += manhattan_distance * manhattan_distance;
				}
				else if (player->mat[i][j] == 'O')
				{
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction[k] += manhattan_distance;
				}
				// else if (player->mat[i][j] == '#' || player->mat[i][j] == '1' || player->mat[i][j] == '2')
				// {
				// 	 direction[k] += MANHATAN_DISTANCE(fabs(i - player->your_posx), fabs(j - player->your_posy));
				// }
			}
		}
	}
	int min = INFINITY;
	int next = -1;
	for (int i = 0; i < 4; i++)
	{
		printf("direction[%d] = %d ", i, direction[i]);
		if (direction[i] < min)
		{
			min = direction[i];
			next = i;
		}
	}
	printf("minscore:%d ",direction[next]);
	return next;
}


struct Point walk(struct Player *player) {
	// This function will be executed in each round.
	
	// printf("pos %d %d\n", player->your_posx, player->your_posy);
	// printf("%d %d\n", snake_body_queue[0].x, snake_body_queue[0].y);
	// printf("%d %d\n", player->your_posx, player->your_posy);

	int next = direction_score(player);
	printf("pos %d %d %d\n", player->your_posx, player->your_posy, next);
	if (next != -1)
	{
		return initPoint(player->your_posx + step[next][0], player->your_posy + step[next][1]);
	}
	return initPoint(player->your_posx, player->your_posy);
}


// void init_weight_map(struct Player *player ,int **weight_map) {

// 	//动态分配内存空间
// 	weight_map = (int **)malloc(sizeof(int *) * player->your_posx);
// 	for(int i = 0; i < player->your_posx; i++) {
// 		weight_map[i] = (int *)malloc(sizeof(int) * player->your_posy);
// 	}

// 	memset(weight_map, 0, sizeof(weight_map));

// 	for(int i = 0; i < player->your_posx; i++) {
// 		for(int j = 0; j < player->your_posy; j++) {
// 			if (player->mat[i][j] == '#') {
// 				weight_map[i][j] = MIN;
// 			} else if (player->mat[i][j] == 'o' || player->mat[i][j] == 'O') {
// 				weight_map[i][j] = MANHATAN_DISTANCE(fabs(i - player->your_posx), fabs(j - player->your_posy));
// 			} else {
				
// 			}
// 		}
// 	}
// }


// void void free_map(struct Player *player, int **weight_map) {
// 	for(int i = 0; i < player->your_posx; i++) {
// 		for(int j = 0; j < player->your_posy; j++) {
// 			free(map[i][j]);
// 		}
// 	}

// 	free(map);
// }


// struct Point walk(struct Player *player) {
// 		// This function will be executed in each round.
//  	printf("pos %d %d\1n", player->your_posx, player->your_posy);

// 		return initPoint(player->your_posx, player->your_posy);
// }

//-------------------------------------------------------函数定义------------------------------------------------------
