
// acpi battery check
//nmcli

/* File path: basics/simple_key.c */
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "interface.c"

#define NCURSES_INIT	initscr();\
		        cbreak();\
			noecho();\
			curs_set(0);\
			start_color()

#define WINDOW_FILL_C(n, f, b)  	init_pair(n, f, b);\
					wbkgd(win, COLOR_PAIR(n))

#define FREE_BLOCK 		free(block->hash);\
				free(block->data);\
				free(block)
#define MIN_BLOCK 1

void getblockdata(struct block *);
void getblockhash(struct block *, int);
char *getrawtransaction(char *string);
int print_json(struct block *, WINDOW *);
int getmaxheight(void);

int main(int argc, char *argv[])
{
	
	int			max_block = getmaxheight();
	int			blk = max_block;
	int			c, tx_cnt;
	struct block		*block;

	
	NCURSES_INIT;
	refresh();
	
	WINDOW *field_blk_num = newwin(1, 9, 1, 0);
	WINDOW *header = newwin(1, 0, 0, 0);
	WINDOW *tx_list = newwin(0, 64, 3, 0);

	
	wprintw(header,"Last Block Mined: %d", max_block);
	wrefresh(header);
	wprintw(field_blk_num, "%d", blk);
	wrefresh(field_blk_num);

	while((c = getch()) != 'q') {

		
		if (c == (int) 'j' && blk > 1) {
			blk--;
			wclear(field_blk_num);
			mvwprintw(field_blk_num, 0,0, "%d", blk);
			wrefresh(field_blk_num);
		}

		if (c == (int) 'k' && blk < max_block) {
			blk++;
			wclear(field_blk_num);
			mvwprintw(field_blk_num, 0,0, "%d", blk);
			wrefresh(field_blk_num);

		}

                block = malloc(sizeof(struct block));
		getblockhash(block, blk);
		getblockdata(block);
		wclear(tx_list);
		tx_cnt = print_json(block, tx_list);
		wrefresh(tx_list);
		FREE_BLOCK;

	}
	endwin();
	return 0;
}

void getblockdata(struct block *block) {


	char			g_blk_str[88];

	g_blk_str[0] = '\0';
	strcpy(g_blk_str, "getblock ");
	strcat(g_blk_str, block->hash);
	p_query(g_blk_str, &(block->data));

}


void getblockhash(struct block *block, int blk) {
	
	char			g_b_hash_str[24];
	char			tmp_str[8];

	g_b_hash_str[0] = '\0';
	strcpy(g_b_hash_str, "getblockhash ");
	sprintf(tmp_str, "%d", blk);
	strcat(g_b_hash_str, tmp_str);
	p_query(g_b_hash_str, &(block->hash));

			
}

int print_json(struct block *block, WINDOW *tx_list) {

	json_t			*root;
	json_error_t		error;

	root = json_loads(block->data, 0, &error);
	json_t *array = json_object_get(root, "tx");
        for (int i = 0; i < json_array_size(array); ++i) { 
	      json_t *string = json_array_get(array, i);
	      if(i == 0)
		      wattron(tx_list,A_REVERSE);
	      else wattroff(tx_list, A_REVERSE);
	      wprintw(tx_list, "%s", json_string_value(string));
	}

        return json_array_size(array);
}


int getmaxheight() {

	char *chain_tip, *endptr;
	int tip;
	
	p_query("getblockcount", &chain_tip);
	return strtol(chain_tip, &endptr, 10);
		
}

char *getrawtransaction(char *string) {

	char			cmd[84];
	char			tmp_str[64];
	char			**raw;

	cmd[0] = '\0';
	strcpy(cmd, "getrawtransaction ");
	strcat(cmd, string);
	p_query(cmd,  raw);
	return *raw;
	
}


