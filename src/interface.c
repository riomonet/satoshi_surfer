#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
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

int main(int argc, char *argv[])
{

	struct block		block;
	char			cmdstr[88] = "getblock ";
	char			*chain_tip;
	json_t			*root;
	json_error_t		error;
	int			x, i = 0;


        p_query("getblockcount", &chain_tip);
        p_query("getblockhash 990000", &block.hash);
	strcat(cmdstr, block.hash);
	p_query(cmdstr, &block.data);

	root = json_loads(block.data, 0, &error);
        json_t *array = json_object_get(root, "tx");
	json_t *string = json_array_get(array, 0);
	printf("%s\n",json_string_value(string));


	

        return 0;
}





