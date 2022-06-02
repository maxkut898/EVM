#include "sat.h"

char str2command(char *str)
{
	char ret;

	if (strcmp(str, "READ") == 0)
		ret = 10;
	else if (strcmp(str, "WRITE") == 0)
		ret = 11;
	else if (strcmp(str, "LOAD") == 0)
		ret = 20;
	else if (strcmp(str, "STORE") == 0)
		ret = 21;
	else if (strcmp(str, "ADD") == 0)
		ret = 30;
	else if (strcmp(str, "SUB") == 0)
		ret = 31;
	else if (strcmp(str, "DIVIDE") == 0)
		ret = 32;
	else if (strcmp(str, "MUL") == 0)
		ret = 33;
	else if (strcmp(str, "JUMP") == 0)
		ret = 40;
	else if (strcmp(str, "JNEG") == 0)
		ret = 41;
	else if (strcmp(str, "JZ") == 0)
		ret = 42;
	else if (strcmp(str, "HALT") == 0)
		ret = 43;
	else if (strcmp(str, "JNP") == 0)
		ret = 59;
	else
		ret = -1;
	
	return ret;
}

void print_error(char *line, int line_cnt, int err)
{
	switch (err) {
		case ERR_ARG1:
			fprintf(stderr, "%d: first argument isn't a number\n", line_cnt);
			break;

		case ERR_ARG2:
			fprintf(stderr, "%d: unknown command\n", line_cnt);
			break;

		case ERR_ARG3:
			fprintf(stderr, "%d: third argument isn't a number\n", line_cnt);
			break;

		case EMPTY_STR:
			fprintf(stderr, "%d: should be three aguments\n", line_cnt);
			break;

		case ERR_MANY:
			fprintf(stderr, "%d: too many arguments\n", line_cnt);
			break;
		case ERR_ENCD:
			fprintf(stderr, "%d: error encode commend\n", line_cnt);
		default:break;
	}
	fprintf(stderr, "%s", line);
}

int test_argv_asm(char *argv[])
{
	char *ptr1, *ptr2;

	ptr1 = strrchr(argv[1], '.');
	ptr2 = strrchr(argv[2], '.');
	if ((strcmp(ptr1, ".sa") != 0) || (strcmp(ptr2, ".o") != 0))
		return -1;
	else
		return 0;
}

int decod_val(char *buf, int_least16_t *cell) {
	if (buf[0] == '+') {
		if (strlen(buf) > 5) {
			return -1;
		}
		int_least16_t val = (int_least16_t) atoi(&buf[1]);
		if ((val >= 0x1fff) || (val <= -0x1fff)) {
			return -1;
		}
		int flg = sc_valueEncode(val, cell);
		if (flg != OK) {
			return -1;
		}
	} else {
		return -1;
	}
	return 0;
}

int pars_line(char *str, int *addr, int_least16_t *value)
{
	char *ptr;
	ptr = strchr(str, ';');
	if (ptr != NULL)
		*ptr = '\0';
	ptr = strchr(str, '\n');
	if (ptr != NULL)
		*ptr = '\0';


	ptr = strtok(str, " \t");
	if (ptr == NULL)
		return EMPTY_STR;
	if (sscanf(ptr, "%d", addr) != 1)
		return ERR_ARG1;
	if ((*addr < 0) || (*addr > MEMSIZE))
		return ERR_ARG1;


	ptr = strtok(NULL, " \t");
	if (ptr == NULL)
		return EMPTY_STR;
	if (strcmp(ptr, "=") == 0) {
		ptr = strtok(NULL, " \t");
		if (ptr == NULL)
			return EMPTY_STR;
		int flg = decod_val(ptr, value);
		if (flg != 0)
			return ERR_ARG3;
		return 0;
	}
	char command = str2command(ptr);
	if (command == -1)
		return ERR_ARG2;


	ptr = strtok(NULL, " \t");
	if (ptr == NULL)
		return EMPTY_STR;
	int oper;
	if (sscanf(ptr, "%d", &oper) != 1)
		return ERR_ARG3;
	char operand = (char) oper;
	if(sc_commandEncode(command, operand, value))
		return ERR_ENCD;

	ptr = strtok(NULL, " \t");
	if (ptr != NULL)
		return ERR_MANY;
	return 0;
}

int sat(int argc, char **argv)
{
	if (argc != 3) {
		perror("Incorrect arguments!\n");
        return -1;
	}
	if (test_argv_asm(argv) != 0) {
		printf("Incorrect arguments!\n");
        return -1;
	}
	FILE *input, *output;
	if ((input = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "Cannot open file:%s", argv[1]);
        return -1;
	}
	if ((output = fopen(argv[2], "wb")) == NULL) {
		fprintf(stderr, "Cannot open file:%s", argv[2]);
        return -1;
	}
	sc_memoryInit();
	char buf[256], line[256];
	int flg = 0;
	int addr;
	int_least16_t value;
	int number_str = 0;
	while (fgets(line, 255, input)) {
		strcpy(buf, line);
		flg = pars_line(buf, &addr, &value);
		if (flg) {
			print_error(line, number_str, flg);
            return -1;
		}
		memory[addr] = value;
		number_str++;
	}
	fwrite(memory, 1, MEMSIZE * sizeof(int_least16_t), output);
	fclose(input);
	fclose(output);
	return 0;
}
