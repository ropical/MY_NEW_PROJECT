# 任务

**`void init(struct Player *player)`**

初始化，只执行一次

**`struct Point walk(struct Player *player)`**

重点函数，返回值为蛇头下一步走的位置。

需要通过各种判断确认下一步走的位置，吃更多的道具，获得尽可能高的分数

# 附加题|选做

杀死对方的蛇，获取对方的分数

# 蛇的结构体
struct Player {
    char **mat;  // Game Data

    int row_cnt;  // Count of rows in current map

    int col_cnt;  // Count of columns in current map

    int your_posx;  // X-corordinate of your AI's position, index from 0
    int your_posy;  // Y-corordinate of your AI's position, index from 0

    int opponent_posx;  // X-corordinate of opponent AI's position, index from 0
    int opponent_posy;  // Y-corordinate of opponent AI's position, index from 0

    int your_status;  // Your AI's status, -1 means died, x(x > 0) means your AI's
                      // super status will sustain in x more rounds, 0 means normal status.

    int opponent_status;  // Opponent AI's status, -1 means died, x(x > 0) means
                          // opponent AI's super status will sustain in x more rounds, 0 means normal status.

    int your_score;  // Your AI's score

    int opponent_score;  // Opponent AI's score
    
    int round;   // current round, begin from 1

    int round_to_shrink;  // current round to shrink, 1 means shrinking next round
};


