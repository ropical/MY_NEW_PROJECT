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

int front, rear;

struct node snake_body_queue[MAX_SIZE];

typedef struct node
{
	int x;
	int y;
}snake;

typedef struct solution
{
	snake now;
	int cost;
	struct solution* before;
} solution;

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

void init(struct Player *player)
{
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
					// printf("next still can move:%d %d\n", step_judge_x, step_judge_y);
					return true;
				}
			}
		}
	}
	return false;
}

int* direction_cost(int x, int y, struct Player *player)
{
	int* direction_cost = (int*)malloc(sizeof(int) * 4);
	for (int k = 0; k < 4; k++)
	{
		int dx = x + step[k][0], dy = y + step[k][1];
		//绝对不能走 直接启用ver0.10
		if (!judge_in_map(dx, dy, player) || !judge_real_move(dx, dy, player))
		{
			direction_cost[k] = INFINITY;
			continue;
		}
		for (int i = 0; i < player->row_cnt; i++)
		{
			for (int j = 0; j < player->col_cnt; j++)
			{
				if (player->mat[i][j] == 'o')
				{
					// printf("direction_cost[%d] += %d ", k, abs(i - player->your_posx) + abs(j - player->your_posy));
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction_cost[k] += manhattan_distance;
				}
				else if (player->mat[i][j] == 'O')
				{
					int manhattan_distance = abs(i - dx) + abs(j - dy);
					direction_cost[k] += manhattan_distance;
				}
				// else if (player->mat[i][j] == '#' || player->mat[i][j] == '1' || player->mat[i][j] == '2')
				// {
				// 	 direction_cost[k] += MANHATAN_DISTANCE(fabs(i - player->your_posx), fabs(j - player->your_posy));
				// }
			}
		}
	}
	return direction_cost;
}

void quick_sort(solution* solution_queue[], int l, int r)
{
    if (l < r)
    {
        //Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
        int i = l, j = r;
		solution* temp = solution_queue[l];
		int temp_cost = temp->cost;
        while (i < j)
        {
            while(i < j && solution_queue[j]->cost >= temp_cost) // 从右向左找第一个小于x的数
                j--;  
            if(i < j) 
                solution_queue[i++] = solution_queue[j];
            
            while(i < j && solution_queue[i]->cost < temp_cost) // 从左向右找第一个大于等于x的数
                i++;  
            if(i < j) 
                solution_queue[j--] = solution_queue[i];
        }
        solution_queue[i] = temp;
        quick_sort(solution_queue, l, i - 1); // 递归调用 
        quick_sort(solution_queue, i + 1, r);
    }
}

snake path[1000];
int path_len = 0;
int bfs(struct Player *player)
{
	solution* solution_queue[10000];
	int queue_l = 0, queue_r = 1;
	solution_queue[0] = (solution*)malloc(sizeof(solution));
	solution_queue[0]->now.x = player->your_posx;
	solution_queue[0]->now.y = player->your_posy;
	solution_queue[0]->before = NULL;
	solution_queue[0]->cost = 0;
	while (queue_l < queue_r)
	{
		solution* handle_solution = solution_queue[queue_l];
		int solution_x = handle_solution->now.x;
		int solution_y = handle_solution->now.y;
		solution* solution_before = handle_solution->before;
		int solution_cost = handle_solution->cost;
		queue_l++;
		// printf("x,y,cost,l,r:%d %d %d %d %d\n",solution_x,solution_y,solution_cost,queue_l,queue_r);
		if (player->mat[solution_x][solution_y] == 'o' || player->mat[solution_x][solution_y] == 'O')
		{
			int handle_x = solution_x;
			int handle_y = solution_y;
			int t = 0;
			while (handle_x != player->your_posx || handle_y != player->your_posy)
			{
				path[t++] = handle_solution->now;
				handle_solution = handle_solution->before;
				handle_x = handle_solution->now.x;
				handle_y = handle_solution->now.y;
			}
			// printf("bfs return:%d\n",t);
			return t;
		}
		int* direction_cost_response = direction_cost(solution_x, solution_y, player);
		// printf("1,2,3,4:%d %d %d %d\n",direction_cost_response[0],direction_cost_response[1],direction_cost_response[2],direction_cost_response[3]);
		for (int i = 0; i < 4; i++)
		{
			int dx = solution_x + step[i][0];
			int dy = solution_y + step[i][1];
			if (judge_in_map(dx, dy, player) && direction_cost_response[i] != INFINITY)
			{
				solution_queue[queue_r] = (solution*)malloc(sizeof(solution));
				solution_queue[queue_r]->now.x = dx;
				solution_queue[queue_r]->now.y = dy;
				solution_queue[queue_r]->before = handle_solution;
				solution_queue[queue_r]->cost = solution_cost + direction_cost_response[i];
				queue_r++;
			}
		}
		free(direction_cost_response);
		quick_sort(solution_queue, queue_l, queue_r - 1);
	}
	return -1;
}


struct Point walk(struct Player *player) {
	// This function will be executed in each round.
	// printf("pos %d %d %d\n", player->your_posx, player->your_posy, path_len);
	if (path_len == 0)
	{
		path_len = bfs(player);
	}
	path_len--;
	// if(path_len = -1) printf("fuck");
	// printf("pos %d %d\n", path[path_len].x, path[path_len].y);
	return initPoint(path[path_len].x, path[path_len].y);
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
