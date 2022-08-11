#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jansson.h> 

#define MAX_BLOCK_SIZE 4000000


struct block
{
	char	*height;
	char	*hash;
	char    *data;
	char	*tx;
  	int	tx_cnt;
	int	totfees;
	int	totval;
};

    
void p_query(char * cmdstr, char **str)
{
	FILE			*input;
	char			command [1000] = "bitcoin-cli ";
	char			buf [MAX_BLOCK_SIZE]; //**how many MB is  max block size ?	
	char			c;
	int			i = 0;

	//check return values from bitcoin-cli commands
	strcat(command, cmdstr);

	input = popen(command, "r");
        while ((fread(&c, sizeof(char), 1, input)) != 0) {
		buf[i] = c;
		i++;
	}

        buf[i] = '\0';
	pclose(input);

	*str = malloc (strlen(buf) * sizeof(char));
	strcpy(*str, buf);
}

int list_all_tx(json_t *array) //fiXME
{
	int i;

	for (i = 0; i < json_array_size(array); ++i) {
		json_t *string = json_array_get(array, i);
		printf("%s\n",json_string_value(string));
	}


	return 0;
}

int main(int argc, char *argv[])
{

	struct block		block;
	char			cmdstr[88] = "getblock ";
	char			*chain_tip;
	json_t			*root;
	json_error_t		error;
	int			i;


        p_query("getblockcount", &chain_tip);
        p_query("getblockhash ", &block.hash);
	strcat(cmdstr, block.hash);
	p_query(cmdstr, &block.data);
	printf("%s\n", cmdstr);

	root = json_loads(block.data, 0, &error);
        json_t *array = json_object_get(root, "tx");


        return 0;
}





