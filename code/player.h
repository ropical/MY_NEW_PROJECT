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

#define MAX_SIZE 1000000
#define DISTANCE_WEIGHT(X) ((10 * (X)) * (10 * (X)))
#define KEY 4
#define SINGLE_SNAKE_WEIGHT 4
#define APPLE_WEIGHT 2
#define DEFENCE_WEIGHT 4
#define SINGLE_OPPONENT_WEIGHT 2

//--------------------------------------------------------宏定义------------------------------------------------------

//-----------------------------------------------------全局变量定义----------------------------------------------------

typedef struct node
{
	int x;
	int y;
} snake;


int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};

//权值赋值
double INF = (double)(-0x3f3f3f3f);

//-----------------------------------------------------全局变量定义----------------------------------------------------

//-------------------------------------------------------函数声明------------------------------------------------------

// 这个功能在开始的时候执行一次， 初始化蛇身（队列存储） --[done][must]
void init(struct Player *player);

//蛇的长度
int snake_len(struct Player *player, int flag);

// bfs搜索最大连通块
int maximum_connected_block(struct Player *player, int start_x, int start_y);

// 判断是否在函数体内 --[done] [in_workable]
bool judge_no_barrier(int target_x, int target_y, struct Player *player);

// 是否在地图内 --[done] [workable]
bool judge_in_map(int target_x, int target_y, struct Player *player);

// ver0.10蠢蛇走法应用函数 --[done]
bool judge_real_move(int target_x, int target_y, struct Player *player);

// 曼哈顿距离 --[bug][done]
int direction_score(struct Player *player);

// 最重要的函数，判断下一步走哪里 --[hang_in]
struct Point walk(struct Player *player);

//-------------------------------------------------------函数声明------------------------------------------------------

//-------------------------------------------------------函数定义------------------------------------------------------

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
}

bool judge_no_barrier(int target_x, int target_y, struct Player *player)
{
	//蛇尾可以走 需要修改 --[hang in]
	if (player->mat[target_x][target_y] != '#' && player->mat[target_x][target_y] != '1' && player->mat[target_x][target_y] != '2')
	{
		return true;
	}
	return false;
}

int snake_len(struct Player *player, int flag)
{
	int cnt = 0;
	if (flag)
	{
		for (int i = 0; i < player->row_cnt; i++)
		{
			for (int j = 0; j < player->col_cnt; j++)
			{
				if (player->mat[i][j] == '1')
				{
					cnt++;
				}
			}
		}
		return cnt;
	}
	else
	{
		for (int i = 0; i < player->row_cnt; i++)
		{
			for (int j = 0; j < player->col_cnt; j++)
			{
				if (player->mat[i][j] == '2')
				{
					cnt++;
				}
			}
		}
		return cnt;
	}
}

int maximum_connected_block(struct Player *player, int start_x, int start_y)
{
	if (!judge_no_barrier(start_x, start_y, player))
	{
		return 0;
	}
	// 开辟占空间和visit数组
	int front = 0, rear = 0, cnt = 0;
	struct node queue[100];
	int vis[10][10];

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			vis[i][j] = 0;
		}
	}

	// bfs求最大连通块大小
	struct node tmp = {start_x, start_y};
	queue[rear++] = tmp;
	cnt++;
	vis[start_x][start_y] = 1;
	while (front < rear)
	{
		struct node now = queue[front++];
		for (int i = 0; i < 4; i++)
		{
			int direction_x = now.x + step[i][0];
			int direction_y = now.y + step[i][1];
			if (!vis[direction_x][direction_y] && judge_in_map(direction_x, direction_y, player) && judge_no_barrier(direction_x, direction_y, player)) // --[bug] <judge_barrier()> 蛇尾可以走
			{
				vis[direction_x][direction_y] = 1;
				struct node tmp = {direction_x, direction_y};
				queue[rear++] = tmp;
				cnt++;
			}
		}
	}
	return cnt;
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

int dead_snake_x = INF;
int dead_snake_y = INF;
int dead_snake_block = 0;

bool judge_real_move(int target_x, int target_y, struct Player *player)
{

	if (!judge_no_barrier(target_x, target_y, player))
	{
		return false;
	}
	int target_connect_block = maximum_connected_block(player, target_x, target_y);

	// printf("x,y,block:%d %d %d\n",target_x,target_y,target_connect_block);
	if (target_connect_block >= snake_len(player, 1))
	{
		if (target_x == 7 && target_y == 6)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					printf("%c", player->mat[i][j]);
				}
				printf("\n");
			}
			printf("bugbugbugbugbugbugbugbugbugbugbug%d\n", target_connect_block);
		}
		if (player->round_to_shrink == 2)
		{
			int wall = player->round / 40;
			if (target_x < 9 - wall && target_x > wall && target_y < 9 - wall && target_y > wall)
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		printf("tar:%d %d block:%d snake_len:%d dead_snake_block:%d\n", target_x, target_y, target_connect_block, snake_len(player, 1), dead_snake_block);
		if (target_connect_block > dead_snake_block)
		{
			printf("dead:%d %d tar:%d %d\n", dead_snake_x, dead_snake_y, target_x, target_y);
			dead_snake_x = target_x;
			dead_snake_y = target_y;
			dead_snake_block = target_connect_block;
		}
	}
	return false;
}

int direction_score(struct Player *player)
{
	double direction[4] = {0};
	dead_snake_block = 0;

	for (int k = 0; k < 4; k++)
	{
		int dx = player->your_posx + step[k][0], dy = player->your_posy + step[k][1];
		//绝对不能走 直接启用ver0.10
		if (!judge_in_map(dx, dy, player) || !judge_real_move(dx, dy, player))
		{
			if (player->your_posx == 7 && player->your_posy == 6)
			{
				printf("IN%d\n", k);
			}
			direction[k] = INF;
			continue;
		}

		for (int i = 0; i < player->row_cnt; i++)
		{
			for (int j = 0; j < player->col_cnt; j++)
			{
				if (player->mat[i][j] == 'o')
				{
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction[k] += (double)(KEY * snake_len(player, 1) * SINGLE_SNAKE_WEIGHT * APPLE_WEIGHT) / (DISTANCE_WEIGHT(manhattan_distance));
				}
				else if (player->mat[i][j] == 'O')
				{
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction[k] += (double)(KEY * snake_len(player, 1) * DEFENCE_WEIGHT) / (DISTANCE_WEIGHT(manhattan_distance));
				}
				else if (player->mat[i][j] == '2')
				{
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction[k] -= (double)(KEY * snake_len(player, 0) * SINGLE_OPPONENT_WEIGHT) / (DISTANCE_WEIGHT(manhattan_distance));
				}
			}
		}
	}
	
	double max = INF;
	int next = -1;
	for (int i = 0; i < 4; i++)
	{
		if (direction[i] > max)
		{
			max = direction[i];
			next = i;
		}
	}
	return next;
}

struct Point walk(struct Player *player)
{
	// This function will be executed in each round.
	// printf("pos %d %d\n", player->your_posx, player->your_posy);
	int next = direction_score(player);
	if (next != -1)
	{
		return initPoint(player->your_posx + step[next][0], player->your_posy + step[next][1]);
	}
	// printf("realdead:%d %d\n",dead_snake_x,dead_snake_y);
	return initPoint(dead_snake_x, dead_snake_y);
}

//-------------------------------------------------------函数定义------------------------------------------------------
