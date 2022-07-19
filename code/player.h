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

// 宏定义
#define INFINITY 0x3f3f3f3f

// 全局变量定义
int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};

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
void init_weight_map(struct Player *player, int **weight_map);

//释放权重图空间 --[done][workable]
void free_map(struct Player *player, int **weight_map);

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
}

bool judge_move(int target_x, int target_y, struct Player *player)
{
	//蛇尾可以走 需要修改
	if (player->mat[target_x][target_y] != '#' && player->mat[target_x][target_y] != '1' && player->mat[target_x][target_y] != '2')
	{
		return true;
	}

	return false;
}

bool judge_in_map(int target_x, int target_y, struct Player *player)
{
	int wall = player->round / 40;
	if (player->round_to_shrink == 1)
	{
		wall++;
	}
	if (target_x >= wall && target_x < player->row_cnt - wall && target_y >= wall && target_y < player->col_cnt - wall)
	{
		return true;
	}

	return false;
}

bool judge_real_move(int target_x, int target_y, struct Player *player)
{
	if (judge_move(target_x, target_y, player))
	{
		for (int j = 0; j < 4; j++)
		{
			int step_judge_x = target_x + step[j][0];
			int step_judge_y = target_y + step[j][1];
			if (judge_in_map(step_judge_x, step_judge_x, player) && judge_move(step_judge_x, step_judge_x, player))
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

int direction_score(struct Player *player)
{
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
					direction[k] += manhattan_distance;
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

struct Point walk(struct Player *player)
{
	// This function will be executed in each round.
	// printf("pos %d %d\n", player->your_posx, player->your_posy);
	int next = direction_score(player);
	printf("pos %d %d %d\n", player->your_posx, player->your_posy, next);
	if (next != -1)
	{
		return initPoint(player->your_posx + step[next][0], player->your_posy + step[next][1]);
	}
	return initPoint(player->your_posx, player->your_posy);
}
