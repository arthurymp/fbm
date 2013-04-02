#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fbmanager.h" 


int isok(int *a, int *b, int k)
{	
	int i;
	for(i = 0;i < 8;i++) {
		if(a[i] == k || b[i] == k) {
			return -1;
		}
	}
	return 1;
}

void init_match_table(struct match_table *p)
{
	int i, j, randm, k, flag, t1, t2, cnt;
	int a[8], b[8];
	int situ[16][16];
	printf("正在初始化对阵表，请稍等...\n");
	for(i = 0;i < 16;i++) {
		for(j = 0;j < 16;j++) {
			situ[i][j] = -1;
		}
		p->score_list[i] = i;
	}
	t1 =0;
	for(i = 0;i < 30;i++) {     //初始化对阵表前7次比赛
		for(j = 0;j < 8;j++) {
			a[j] = -1;
			b[j] = -1;
		}
		for(j = 0;j < 8;) {
			randm = rand() % 16;
			if(isok(a, b, randm) == 1) {
				a[j] = randm;
				cnt = 0;
				while(1) {
					cnt++;
					if(cnt == 50) {
						//printf("--1 \n");
						if(t1 == 500) {
							//printf("clear\n");
							for(i = 0;i < 16;i++) {
								for(j = 0;j < 16;j++) {
									situ[i][j] = -1;
								}
							}
						t1 = 0;
						i = 0;
						j = 0;
						}else if(j == 0) {
							i--;
							situ[p->tables[i][j][0]][p->tables[i][j][1]] = -1;
							j = 8;
						}else {
							j--;
							situ[a[j]][b[j]] = -1;
						}
						t1++;
						break;
					}
					randm = rand() % 16;
					if(isok(a, b, randm) == 1 && situ[a[j]][randm] == -1) {
						b[j] = randm;
						situ[a[j]][randm] = 1;
						p->tables[i][j][0] = a[j];
						p->tables[i][j][1] = b[j];
						j++;
						break;
					}
				}
			}
		}
	}
}

void show_match_table(struct match_table *p)
{
	int i, j;
	for(i = 0;i < 30;i++) {
		printf("Round%2d\t" ,i + 1);
		for(j = 0;j < 8;j++) {
			printf("%-2d:%2d   ", p->tables[i][j][0], p->tables[i][j][1]);
		}
		printf("\n");
	}
}

int find_best_player(int *p, int n)
{
	int i, t;
	t = rand() % 11;
	for(i = 0;i < n;i++) {
		if(p[t] < p[i]) {
			t = i;
		}
	}
	return t;
}
float count = 0;

void play_match(struct club *a, struct club *b, int n, int m, struct match_table *p)
{
	int a_goal = 0;
	int b_goal = 0;
	int i, bestplayer;
	for(i = 0;i <(((15 - a->num) - a->num + b->num)/ 5);i++) {
		a_goal += (rand() % 2) /* (rand() % 2)*/;
	}

	for(i = 0;i < (((15 - b->num) - b->num + a->num)/ 5);i++) {
		b_goal += rand() % 2;
	}
	a_goal += rand() % 2 + 
			(rand() % 2) * (rand() % 2) * (rand() % 2) + 
			(rand() % 2) * (rand() % 2) * (rand() % 2) + 
		//	(rand() % 2) * (rand() % 2) +
		//	(rand() % 2) * (rand() % 2) + 
			(rand() % 2) * (rand() % 2) * ((rand() % 5) * (rand() % 2) * (rand() % 2) * (rand() % 2)) ;
	a->goals[n] = a_goal;
	a->cntgoal += a->goals[n];
	a->pure_goals[n] = a_goal - b_goal;
	a->pure_goal += a->pure_goals[n];
	b_goal += rand() % 2 +
			(rand() % 2) * (rand() % 2) * (rand() % 2) + 
			(rand() % 2) * (rand() % 2) * (rand() % 2) + 
		//	(rand() % 2) * (rand() % 2) +
		//	(rand() % 2) * (rand() % 2) + 
			(rand() % 2) * (rand() % 2) * ((rand() % 5) * (rand() % 2) * (rand() % 2) * (rand() % 2) * (rand() % 2)) ;
	b->goals[n] = b_goal;
	b->cntgoal += b->goals[n];
	b->pure_goals[n] = b_goal - a_goal;
	b->pure_goal += b->pure_goals[n];
	for(i = 0;i < a_goal;i++) {
		a->member_shooter[n][rand() % 11] ++;
	}
	for(i = 0;i < b_goal;i++) {
		b->member_shooter[n][rand() % 11] ++;
	}
		a->best_shooter_match[n] = find_best_player(a->member_shooter[n], 11);
		b->best_shooter_match[n] = find_best_player(b->member_shooter[n], 11);
	if(a_goal == 0 && b_goal == 0){
		a->score += 1; a->dual += 1;
		b->score += 1; b->dual += 1;
		p->best_shooter_match[n][m][0] = (rand() % 2 == 0 ? a->num:b->num);
		p->best_shooter_match[n][m][1] = rand()%11;
	}else if(a_goal > b_goal){
		a->score += 3; a->win += 1;
		b->score += 0; b->fail += 1;
		p->best_shooter_match[n][m][0] = a->num;
		p->best_shooter_match[n][m][1] = a->best_shooter_match[m];
	}else if(a_goal == b_goal) {
		a->score += 1; a->dual += 1;
		b->score += 1; b->dual += 1;
		p->best_shooter_match[n][m][0] = b->num;
		p->best_shooter_match[n][m][1] = b->best_shooter_match[m];
	}else {
		a->score += 0; a->fail += 1;
		b->score += 3; b->win += 1;
		p->best_shooter_match[n][m][0] = b->num;
		p->best_shooter_match[n][m][1] = b->best_shooter_match[m];
	}
	count +=a_goal + b_goal;
//	printf("%dvs%d  %d:%d\t",a->num,b->num,a_goal,b_goal);
	printf("%s vs %s  %d:%d\n",a->name,b->name,a_goal,b_goal);
}

void round_best(struct club *clu, struct match_table *p,int round)
{
	//int i, j;
	int i, j, tmp, coach_tmp = 0, shooter[30] = {0};
	j = 0;
	for(i = 0;i < 16;i++) {
		if(clu[j].pure_goals[round] < clu[i].pure_goals[round]) {
			j = i;
		}else if(clu[j].pure_goals[round] == clu[i].pure_goals[round]) {
			if(clu[j].goals[round] < clu[i].goals[round]) {
				j = i;
			} else {
				j = rand()%2 == 0 ? j:i;
			}
		}
	}
	p->best_club[round] = j;
	p->best_shooter_round[round][0] = j;
	p->best_shooter_round[round][1] = clu[j].best_shooter_match[round];
	clu[j].cntbest++;
	tmp = 0;          //////////////////////////////////////////
	for(i = 0;i < 15;i++) {
		tmp = i;
		if(clu[i].cntbest > clu[coach_tmp].cntbest) {
			coach_tmp = i;
		}else if(clu[i].cntbest == clu[coach_tmp].cntbest) {
			if(clu[i].score > clu[coach_tmp].score) {
				coach_tmp = i;
			} else if(clu[i].score == clu[coach_tmp].score) {
				if(clu[i].pure_goal > clu[coach_tmp].pure_goal) {
					coach_tmp = i;
				} else if(clu[i].pure_goal == clu[coach_tmp].pure_goal) {
					if(clu[i].cntgoal > clu[coach_tmp].cntgoal) {
						coach_tmp =i;
					} else if(clu[i].cntgoal == clu[coach_tmp].cntgoal) {
						coach_tmp = (rand() % 2 == 0 ? i:coach_tmp);
					}
				}
			}
		}
		for(j = i + 1;j < 16;j++) {


			if(clu[p->score_list[j]].score > clu[p->score_list[tmp]].score) { 
				tmp = j;
			} else if(clu[p->score_list[j]].score == clu[p->score_list[tmp]].score) {
				if(clu[p->score_list[j]].pure_goal > clu[p->score_list[tmp]].pure_goal)
					tmp = j;
				else if(clu[p->score_list[j]].pure_goal == clu[p->score_list[tmp]].pure_goal) {
					if(clu[p->score_list[j]].cntgoal > clu[p->score_list[tmp]].cntgoal)
						tmp = j;
					else if(clu[p->score_list[j]].cntgoal == clu[p->score_list[tmp]].cntgoal) {
						if(rand() % 2 == 0) {
							tmp = j;
						}
					}
				}
			}
		}
			if(tmp != i){
				j = p->score_list[i];
				p->score_list[i] = p->score_list[tmp];
				p->score_list[tmp] = j;
				clu[p->score_list[i]].list = i;
				clu[p->score_list[tmp]].list = tmp;
			}
	}
	p->best_coach = coach_tmp;
	for(i = 0;i < round;i++) {
		for(j = 0;j < i;j++) {
			if(p->best_shooter_round[i][0] == p->best_shooter_round[j][0] && p->best_shooter_round[i][1] == p->best_shooter_round[j][1]) {
				shooter[j]++;
			}else {
				shooter[i]++;
			}
		}
	}
	tmp = 0;
	for(i = 0;i < round;i++) {
		if(shooter[i] > shooter[tmp]) {
			tmp = i;
		}else if(shooter[i] == shooter[tmp]) {
			if(clu[p->best_shooter_round[i][0]].list > clu[p->best_shooter_round[tmp][0]].list) {
				tmp = i;
			}
		}
	}
	p->best_shooter_total[0] = p->best_shooter_round[tmp][0];
	p->best_shooter_total[1] = p->best_shooter_round[tmp][1];
}

void init_clubs(struct club *p)
{
	int i, j, t;
	int flag = 0;
	FILE *fp;
	fp = fopen("data","r");
	if(!fp) {
		printf("无法打开数据文件，请手动输入\n");
		flag = 1;
	}
	for(i = 0;i < 16;i++) {
		p[i].num = i;
		p[i].list = i;
		p[i].win = 0;
		p[i].fail = 0;
		p[i].dual = 0;
		p[i].score = 0;
		p[i].cntgoal = 0;
		p[i].pure_goal = 0;
		p[i].cntbest = 0;
		if(flag == 1) {
			printf("The club name and the coach\n");
			scanf("%s",p[i].name);
			scanf("%s",p[i].coach);
		} else {	
			fscanf(fp,"%s",p[i].name);
			fscanf(fp,"%s",p[i].coach);
		}
		for(j = 0;j < 30;j++) {
			p[i].best_shooter_match[j] = 0;
			p[i].goals[j] = 0;
			p[i].pure_goals[j] = 0;
			for(t = 0;t < 11;t++)
				p[i].member_shooter[j][t] = 0;
		}
	}
	fclose(fp);
}

void show_best_shooter_match(int round, const struct match_table *tab, const struct club *p)
{
	int match;
	printf("\n  比赛场次    最佳球员\n");
	for(match = 0;match < 8;match++) {
		printf("%s VS %s\t %d号(%s)\n", p[tab->tables[round][match][0]].name,  p[tab->tables[round][match][1]].name 
								, tab->best_shooter_match[round][match][1], p[tab->best_shooter_match[round][match][0]].name);
	}
}
void show_best_shooter_round(const struct match_table *tab, const struct club *p, int round)
{
	printf("最佳球员     球队\n");
 	printf("  %d号      %s\n",tab->best_shooter_round[round][1], p[tab->best_shooter_round[round][0]].name);
	//printf("   %d号  \t%d\n",tab->best_shooter_round[round][1], p[tab->best_shooter_round[round][0]].num);
}
void show_best_shooter_total(const struct match_table *tab, const struct club *p)
{
	printf("最佳球员     球队\n");
	printf("  %d号     %s\n",tab->best_shooter_total[1], p[tab->best_shooter_total[0]].name);
//	printf("   %d号  \t%d\n",tab->best_shooter_total[1], p[tab->best_shooter_total[0]].num);
}
void show_best_club_round(const struct match_table *tab, const struct club *p, int round)
{
	printf("最佳球队    教练\n");
	printf("%s    %s\n", p[tab->best_club[round]].name, p[tab->best_club[round]].coach);
}
void show_best_club_total(const struct match_table *tab, const struct club *p)
{
	printf("最佳球队  主教练 积分\n");
	printf("%s  %s %d\n", p[tab->score_list[0]].name, p[tab->score_list[0]].coach, p[tab->score_list[0]].score);
}
void show_best_coach(const struct match_table *tab, const struct club *p)
{
	printf("最佳教练  次数  球队\n");
	printf("  %s     %d    %s\n",p[tab->best_coach].coach, p[tab->best_coach].cntbest, p[tab->best_coach].name);
//	printf(" %d  \t%d\t%d\n",p[tab->best_coach].num, p[tab->best_coach].cntbest, p[tab->best_coach].num);
}
void show_club_list(const struct match_table *tab, const struct club *p, int round)
{
	int i;
	printf("\t\t\t积分榜\n球队名称\t轮数\t胜\t平\t负\t进球\t失球\t积分\n");
	for(i = 0;i < 16;i++) {
	//	printf("%d\n", tab->score_list[i]);
		printf("%s\t %d \t %d \t %d \t %d \t %d \t %d \t %d \n", p[tab->score_list[i]].name, round + 1,
																	p[tab->score_list[i]].win, p[tab->score_list[i]].dual, 
																	p[tab->score_list[i]].fail, p[tab->score_list[i]].cntgoal, 
																	p[tab->score_list[i]].cntgoal - p[tab->score_list[i]].pure_goal,
																	p[tab->score_list[i]].score);
	}
}

void clear(void)
{
	char c;
	while((c = getchar()) != '\n' && c != EOF);
}
char menu_round(void)
{
	char select;
	printf("请选择操作，继续下一轮直接按回车：\n");
	printf("A.本轮最佳球员\tB.本轮最佳球队\nC.最新积分榜\n");
	while((select = getchar()) != 'A'
			&& select != 'B' 
			&& select != 'C' 
			&& select != '\n'
			&& select != 'b' 
			&& select != 'c' 
			&& select != 'a'){
		clear();
		printf("输入错误,请重新选择:");
	}
	if(select != '\n')
		clear();
	return select;
}
char menu_total(void)
{
	char select;
	printf("请选择操作退出直接按回车：\n");
	printf("A.联赛最佳射手     B.联赛最佳球队\nC.联赛最佳主教练   D.联赛最终排行榜\n");
	while((select = getchar()) != 'A' 
			&& select != 'B'
			&& select != 'C'
			&& select != 'D'
			&& select != '\n'
			&& select != 'a'
			&& select != 'b'
			&& select != 'c'
			&& select != 'd'){
		clear();
		printf("输入错误,请重新选择:");
	}
	if(select != '\n')
		clear();
	return select;
}
int main(void)
{
	struct match_table tab;
	struct club clubs[16];
	int i, j, next;
	char opt;
	init_clubs(clubs);
	srand(time(NULL));
	init_match_table(&tab);
//	show_match_table(&tab);
	printf("请按回车启动中超联赛:");
	while(getchar() != '\n') {
		clear();
	}
	for(i = 0;i < 30;i++) {
		printf("第%2d轮\n",i + 1);
		next = 0;
		for(j = 0;j < 8;j++) {
			play_match(&clubs[ tab.tables[i][j][0] ], &clubs[ tab.tables[i][j][1] ], i, j, &tab);
		}
		round_best(clubs, &tab, i);
		while(next != 1) {
			switch(menu_round()) {
				case 'A':
				case 'a':
					show_best_shooter_round(&tab, clubs, i);
					break;
				case 'B':
				case 'b':
					show_best_club_round(&tab, clubs, i);
					break;
				case 'C':
				case 'c':
					show_club_list(&tab, clubs, i);
					break;
				case '\n':
					next = 1;
					break;
				default:
					printf("程序异常！\n");
					exit(-1);
					break;
			}
		}
	}
	next = 0;
	printf("%.2f \n",count / (8 * 30));
	while(next != 1) {
		switch(menu_total()) {
			case 'A':
			case 'a':
				show_best_shooter_total(&tab, clubs);
				break;
			case 'B':
			case 'b':
				show_best_club_total(&tab, clubs);
				break;
			case 'C':
			case 'c':
				show_best_coach(&tab, clubs);
				break;
			case 'D':
			case 'd':
				show_club_list(&tab, clubs, 29);
				break;
			case '\n':
				next = 1;
				break;
			default:
				printf("程序异常！\n");
				exit(-1);
				break;
		}
	}
	return 0;
}
