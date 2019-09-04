#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int complement(int val);

int main(int argc, char *argv[]) {
	FILE *file;
	char instruction[4];
	int counter = 0;
	int i;
	//strtol here
	long int li;
	char *pEnd;
	int opcode;
	int dr;
	int sr1;
	int sr2;
	int imm;
	int imm5;
	int immsign;
	int n;
	int z;
	int p;
	int brsign;
	int broffset;
	long int start;
	start = strtol(argv[2], NULL, 16);
	file = fopen(argv[1], "r");
	//printf("start: 0x%x\n", start);
  do {
		char c = fgetc(file);
		if (c == EOF) break;
		memset (instruction, 0, 4); //clear instruction array
		while ((c != '\n') && (c != EOF)) {
			instruction[counter++] = c;
			c = fgetc(file);
		}
		//printf("pc counter: 0x%x | ", start);
		//does everything below here once per line
		start = start + 1;
		counter = 0;
		li = strtol (instruction, &pEnd, 16);
		opcode = li >> 12;
		dr = (li >> 9) & 0x7;
		sr1 = (li >> 6) & 0x7;
		sr2 = li & 0x7;
		imm = (li >> 5) & 0x1;
		immsign = (li >> 4) & 0x1;
		imm5 = li & 0xf;
		n = (li >> 11) & 1;
		z = (li >> 10) & 1;
		p = (li >> 9) & 1;
		brsign = (li >>  & 1;
		broffset = li & 0xff;
		if (opcode == 1) {
			if (imm == 0) {
				printf("add r%d, r%d, r%d\n", dr, sr1, sr2);
			} else if (imm == 1) {
				if (immsign == 1) {
					printf("add r%d, r%d, %d\n", dr, sr1, complement(imm5));
				} else if (immsign == 0) {
					printf("add r%d, r%d, %d\n", dr, sr1, imm5);
				}
			}
		}
		else if (opcode == 5) {
			if (imm == 0) {
				printf("and r%d, r%d, r%d\n", dr, sr1, sr2);
			} else if (imm == 1) {
				if (immsign == 1) {
					printf("and r%d, r%d, %d\n", dr, sr1, complement(imm5));
				} else if (immsign == 0) {
					printf("and r%d, r%d, %d\n", dr, sr1, imm5);
				}
			}
		}
		else if (opcode == 12) {
			printf("jmp r%d\n", sr1);
		}
		else if (opcode == 0) {
			printf("br");
			if (n == 1) {
				printf("n");
			}
			if (z == 1) {
				printf("z");
			}
			if (z == 1) {
				printf("p");
			}
			printf(" ");
			if (brsign == 1) {
				printf("0x%x", (start + complement(broffset)));
			} else if (brsign == 0) {
				printf("0x%x", (start + broffset));
			}
			printf("\n");
		}
	} while (true);
	fclose(file);
}

int complement(int val) {
	val = val | 0xFFFFFFF0;
	return val;
}
