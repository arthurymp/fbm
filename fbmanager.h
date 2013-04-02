#ifndef __FB_MANAGE_H__
#define __FB_MANAGE_H__

struct match_table{
	int tables[30][8][2];
	int best_shooter_match[30][8][2];
	int best_shooter_round[30][2];
	int best_club[30];
	int best_club_total;
	int best_coach;
	int cntshooter;
	int best_shooter[2];
	int best_shooter_total[2];
	int score_list[16];
};

struct club{
	char name[20];
	char coach[20];
	int num;
	int member_shooter[30][11];
	int best_shooter_match[30];
	int score;
	int win;
	int fail;
	int dual;
	int goals[30];
	int pure_goal;
	int cntgoal;
	int cntbest;
	int list;
	int pure_goals[30];
};

#endif
