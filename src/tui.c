
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
#define MAX_TXS 4500
#define TXPAD_PORTAL 20
#define TXPAD_START_Y 3

void getblockdata(struct block *);
void getblockhash(struct block *, int);
void getrawtransaction(WINDOW *, const char *);
const char *retrievetx(struct block *, int);
int print_json(struct block *, WINDOW *, int);
int getmaxheight(void);

int main(int argc, char *argv[])
{
	
	int			max_block = getmaxheight();
	int			blk = max_block;
	int			c, tx_cnt, cur_tx_index = 0, y;
	struct block		*block;
	const char		*transaction;

	
	NCURSES_INIT;
	refresh();

	WINDOW *tx_counter = newwin(1, 0, 0, 0);
	WINDOW *txpad = newpad(MAX_TXS,64);
	WINDOW *parser = newwin(0,50,3,65);

        while((c = getch()) != 'q') {

		if (c == (int) 'j' && blk > 1) {
			blk--;
			cur_tx_index = 1;
		}

		if (c == (int) 'k' && blk < max_block) {
			blk++;
			cur_tx_index = 1;

		}

		if (c == (int) 'c' && cur_tx_index < tx_cnt) {
			cur_tx_index++;
			//	figure out the chgat routine
			//	wmove(txpad, cur_tx_index, 0);
			//	wchgat(txpad, -1, A_REVERSE, 0, NULL);
			
		}
		

		if (c == (int) 'd' && cur_tx_index > 1) {
			cur_tx_index--;
			//implemepent chgat here
		}
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		y = ((cur_tx_index -1) / TXPAD_PORTAL) * TXPAD_PORTAL;
		block = malloc(sizeof(struct block));
		getblockhash(block, blk);
		getblockdata(block);
		wclear(txpad);
		tx_cnt = print_json(block, txpad, cur_tx_index - 1);
		wclear(tx_counter);
		wprintw(tx_counter, "%d Max Block: %d \t Block: %d \t Tx Cnt: %d \t Cur Tx: %d", y, max_block, blk, tx_cnt, cur_tx_index);
		wrefresh(tx_counter);
		wmove(txpad, cur_tx_index -1, 0);
		wchgat(txpad, -1, A_REVERSE, COLOR_PAIR(1), NULL);
		prefresh(txpad, y, 0, TXPAD_START_Y, 0, TXPAD_PORTAL + TXPAD_START_Y, 64);
		transaction = retrievetx (block, cur_tx_index - 1);
		getrawtransaction(parser, transaction);
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

void getrawtransaction(WINDOW *parser, const char *str) {
	
	char			cmd[84];
	char			*buf[10000];

	cmd[0] = '\0';
	strcpy(cmd, "getrawtransaction ");
	strcat(cmd, str);
	p_query(cmd, buf);
	wclear(parser);
	wprintw(parser,"%s",*buf);
	wrefresh(parser);
			
}

int print_json(struct block *block, WINDOW *tx_list, int index) {

	json_t			*root;
	json_error_t		error;

	root = json_loads(block->data, 0, &error);
	json_t *array = json_object_get(root, "tx");
        for (int i = 0; i < json_array_size(array); ++i) { 
	      json_t *string = json_array_get(array, i);
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

const char *retrievetx(struct block *block, int index) {

	char			cmd[84];
	char			tmp_str[64];
	json_t			*root;
	json_error_t		error;

	root = json_loads(block->data, 0, &error);
	json_t *array = json_object_get(root, "tx");
	json_t *string = json_array_get(array, index);


	return  json_string_value(string);
}

