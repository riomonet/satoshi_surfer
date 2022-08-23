#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jansson.h> 
#define MAX_BLOCK_SIZE 1000000


struct block
{
	char	*hash;
	char    *data;
        int	tx_cnt;
};

    
void p_query(char * cmdstr, char **str)
{
	FILE			*input;
	char			command [1000] = "bitcoin-cli ";
	char			c;
	int			i = 0;
	char			buf [MAX_BLOCK_SIZE]; //**how many MB is  max block size ?

	buf[0] ='\0';
	//check return values from bitcoin-cli commands
	strcat(command, cmdstr);


	input = popen(command, "r");
        while ((fread(&c, sizeof(char), 1, input)) != 0) {
		buf[i] = c;
		i++;
	}
	
        buf[i] = '\0';
	pclose(input);

	*str = malloc ((strlen(buf) + 1) * sizeof(char));
	strcpy(*str, buf);
}

int list_all_tx(json_t *array) //fiXME
{
	int i;

	for (i = 0; i < json_array_size(array); ++i) {
		json_t *string = json_array_get(array, i);
		//printf("%s\n",json_string_value(string));
	}

	return 0;
}

/* int main(int argc, char *argv[]) */
/* { */

/* 	struct block		*block; */
/* 	char			g_blk_str[88]; */
/* 	char			g_b_hash_str[24]; */
/* 	char			*chain_tip, *endptr; */
/* 	json_t			*root; */
/* 	json_error_t		error; */
/* 	int			i, tip; */
/* 	char			tmp_str[8]; */


/* 	p_query("getblockcount", &chain_tip); */
/* 	tip = strtol(chain_tip, &endptr, 10); */
	
/* 	printf("tip-height = %d\n", tip); */

/* 	for (i = 1; i < tip; ++i) { */

/* 		block = malloc(sizeof(struct block)); */
/* //		printf("---------------------------------------------------------------------------------------------------\n"); */


/*                 g_b_hash_str[0] = '\0'; */
/* 		strcpy(g_b_hash_str, "getblockhash "); */

/* 		sprintf(tmp_str, "%d", i); */
/* 		strcat(g_b_hash_str, tmp_str); */
/* 		p_query(g_b_hash_str, &(block->hash)); */
/* //		printf("height = %d\t  hash = %s",i , block->hash); */

/* 		g_blk_str[0] = '\0'; */

/*                 strcpy(g_blk_str, "getblock "); */
/* 		strcat(g_blk_str, block->hash); */
/* 		p_query(g_blk_str, &(block->data)); */
/* 		root = json_loads(block->data, 0, &error); */
/* 		json_t *array = json_object_get(root, "tx"); */
/* 		list_all_tx(array); */
/* 		free(block->hash); */
/* 		free(block->data); */
/* 		free(block); */
/* 	} */
/*         return 0; */
/* } */






