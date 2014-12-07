/*
 ============================================================================
 Name        : otp_hack.c
 Author      : Evandro Sombrio
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK	(unsigned char)0xC0
#define	NUM_CYPHER_TEXT 7

typedef enum { IS_LETTER, UNDEFINED } char_type;
typedef struct {
	int is_char;
	int can_be_space;
	char guess_byte;
} char_status;

char str[NUM_CYPHER_TEXT][63];
char_status status[NUM_CYPHER_TEXT][31];


void init(void) {
	strcpy(str[0], "BB3A65F6F0034FA957F6A767699CE7FABA855AFB4F2B520AEAD612944A801E");
	strcpy(str[1], "BA7F24F2A35357A05CB8A16762C5A6AAAC924AE6447F0608A3D11388569A1E");
	strcpy(str[2], "A67261BBB30651BA5CF6BA297ED0E7B4E9894AA95E300247F0C0028F409A1E");
	strcpy(str[3], "A57261F5F0004BA74CF4AA2979D9A6B7AC854DA95E305203EC8515954C9D0F");
	strcpy(str[4], "BB3A70F3B91D48E84DF0AB702ECFEEB5BC8C5DA94C301E0BECD241954C831E");
	strcpy(str[5], "A6726DE8F01A50E849EDBC6C7C9CF2B2A88E19FD423E0647ECCB04DD4C9D1E");
	strcpy(str[6], "BC7570BBBF1D46E85AF9AA6C7A9CEFA9E9825CFD5E3A0047F7CD009305A71E");

	int i;
	for (i = 0; i < NUM_CYPHER_TEXT; i++) {
		puts(str[i]);
	}

	int j;
	for (i = 0; i < NUM_CYPHER_TEXT; i++) {
		for (j = 0; j < 31; j++) {
			status[i][j].is_char = 0;
			status[i][j].can_be_space = 0;
			status[i][j].guess_byte = '?';
		}
	}
}

char_type check_type(unsigned char b1, unsigned char b2) {
	unsigned char mask1 = (b1 & MASK) >> 6;
	unsigned char mask2 = (b2 & MASK) >> 6;
	unsigned char xor = mask1 ^ mask2;
	printf(">> check_type => %02X ^ %02X = %02X <<", mask1, mask2, xor);
	return (char_type) ((xor)? UNDEFINED: IS_LETTER);
}


int main(void) {

	init();

	int column = 0;
	int total_iter = 0;

	int start_column = 0; //0;
	int end_column = 63; //63;
	for (column = start_column; column < end_column; column+=2) {

	    int col = column / 2;
		printf("------------- COLUMN %d ----------\n", col);
		unsigned int i, j;
		unsigned int b1, b2;
		for (i = 0; i < NUM_CYPHER_TEXT; i++) {
			sscanf(&str[i][column], "%02X", &b1);
			for (j = 0; j < NUM_CYPHER_TEXT; j++) {
				 sscanf(&str[j][column], "%02X", &b2);
				 if (b1 == b2)
					continue;


				 char_type t = check_type((unsigned char)b1, (unsigned char)b2);
				 if (t == IS_LETTER && !status[i][col].is_char) {
					 status[i][col].is_char = 1;
				 } else if (t == UNDEFINED && !status[i][col].can_be_space) {
					 status[i][col].can_be_space = 1;

					 char guess_byte = b1 ^ b2 ^ ' ';
					 if (status[i][col].guess_byte == '?')
						 status[i][col].guess_byte = guess_byte;
				 }

				 printf("%02X XOR %02X = (is_char=%d, can_be_space=%d, guess_byte=%c)\n",
						 b1, b2,
						 status[i][col].is_char, status[i][col].can_be_space, status[i][col].guess_byte
						 );

				 if (status[i][col].is_char && status[i][col].can_be_space) {
					 break;
				 }

				 total_iter++;
			}

			printf ("*********** [%.2s]= [%c]\n", &str[i][column], status[i][col].guess_byte);
		}
	}

	printf("Total iterations: %d\n", total_iter);

	// print what was found
	int i, j;
	for (i = 0; i < NUM_CYPHER_TEXT; i++) {
		printf("[");
		for (j = 0; j < 31; j++) {
			//printf("%c", status[i][j].guess_byte);
			//printf("%c", (status[i][j].is_char)? status[i][j].guess_byte: ' ');

			if (status[i][j].guess_byte == '?' && status[i][j].can_be_space) {
				printf(" ");
			} else {
				printf("%c", (status[i][j].is_char)? status[i][j].guess_byte: ' ');
			}
		}
		printf("]\n");
	}

	return EXIT_SUCCESS;
}
