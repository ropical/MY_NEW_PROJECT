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

void init(struct Player *player)
{
	// This function will be executed at the begin of each game, only once.
}

bool judge_move(int target_x, int target_y, struct Player *player)
{
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

int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
struct Point walk(struct Player *player)
{
	// for (int i = 0; i < player->row_cnt; i++) {
	// 	for (int j = 0; j < player->col_cnt; j++) {
	// 		printf("%c", player->mat[i][j]);
	// 	}
	// 	printf("\n");
	// }
	// This function will be executed in each round.
	// printf("pos %d %d\n", player->your_posx, player->your_posy);
	// printf("pos %d %d %d\n", player->your_posx, player->your_posy, player->round_to_shrink);
	for (int i = 0; i < 4; i++)
	{
		if (player->round_to_shrink == 20)
		{
			// printf("dfosijfosdijf");
			step[0][1] = -step[0][1];
			step[1][1] = -step[1][1];
			step[2][0] = -step[2][0];
			step[3][0] = -step[3][0];
		}
		int dx = player->your_posx + step[i][0], dy = player->your_posy + step[i][1];
		if (judge_in_map(dx, dy, player) && judge_move(dx, dy, player))
		{
			for (int j = 0; j < 4; j++)
			{
				int step_judge_x = dx + step[j][0];
				int step_judge_y = dy + step[j][1];
				if (judge_move(step_judge_x, step_judge_y, player))
				{
					if (player->round_to_shrink == 2)
					{
						int wall = player->round / 40;
						if (step_judge_x < 9 - wall && step_judge_x > wall && step_judge_y < 9 - wall && step_judge_y > wall)
						{
							return initPoint(dx, dy);
						}
					}
					else
					{
						return initPoint(dx, dy);
					}
				}
			}
		}
	}
	return initPoint(player->your_posx, player->your_posy);
}
