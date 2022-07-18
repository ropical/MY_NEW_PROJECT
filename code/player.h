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


void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
} 


int step[4][2] = {0, 1, 0, -1, 1, 0, -1, 0};
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
		int dx = player->your_posx + step[i][0], dy = player->your_posy + step[i][1];

		// 第一步判断， 往哪边走;
		if (dx >= 0 && dx < player->row_cnt && dy >= 0 && dy < player->col_cnt && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O')) {
			for (int j = 0; j < 4; j++) {
				int step_judge_x = dx + step[j][0];
				int step_judge_y = dy + step[j][1];

				// 第一步之后是否进死胡同， 确保蛇活下来;
				if (player->mat[step_judge_x][step_judge_y] != '#' && player->mat[step_judge_x][step_judge_y] != '1') {   
					return initPoint(dx, dy);
				}
			}
		}
	}
	return initPoint(player->your_posx, player->your_posy);
}
