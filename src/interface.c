#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <jansson.h> 

/*
get block count.
 enter block number
get block hash from block number
get raw-transaction list from blockhash
decode raw transaction
*/


#define MAX_BLOCK_SIZE 4000000

struct block
{
  char *height;
  char *hash;
  char *tx;
  int  tx_cnt;
  int  totfees;
  int  totval;
};

struct tx
{
  //inputs
  //outputs
  
};   

int main(int argc, char *argv[])
{

	json_t *root;
	json_error_t error;
	FILE *input;
	char *ptr;
	char buf[MAX_BLOCK_SIZE]; //**how many MB is  max block size ?
	char c;
        int i = 0;
	int x;
	struct block block;
	char cmdstr[88] = "bitcoin-cli getblock ";

        /* get block count routine */
        input = popen("bitcoin-cli getblockcount", "r");
        while ((fread(&c,sizeof(char),1,input)) != 0) {
		buf[i] = c;
		i++;
	}
        buf[i] = '\0';
	pclose(input);

	
	/* get block hash from block number */
	i = 0;
	input = popen("bitcoin-cli getblockhash 990000", "r");
        while ((fread(&c,sizeof(char),1,input)) != 0) {
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';

	block.hash = malloc(64 * sizeof(char));
	strcpy(block.hash, buf);
	pclose(input);

        /* get block */
	i = 0;
	strcat(cmdstr,block.hash);
        input = popen(cmdstr, "r");
        while ((fread(&c,sizeof(char),1,input)) != 0) {
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';
	pclose(input);
	block.tx = malloc(strlen(buf));
	strcpy(block.tx,buf);
	root = json_loads(block.tx, 0, &error);
	printf("%s\n",block.tx);



        json_t *array = json_object_get(root, "tx");
	json_t *string = json_array_get(array, 0);
	printf("%s\n",json_string_value(string));


        return 0;
}
