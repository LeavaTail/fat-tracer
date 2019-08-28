/*
 * main.c
 *
 * Print data to Hexadecimal (like 'xxd').
 *
 *
 * MIT License
 *
 * Copyright (c) 2019 LeavaTail
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <config.h>
#include <getopt.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "print.h"

/**
 * Special Option(no short option)
 */
enum
{
	GETOPT_HELP_CHAR = (CHAR_MIN - 2),
	GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};

/* option data {"long name", needs argument, flags, "short name"} */
static struct option const longopts[] =
{
	{"help",no_argument, NULL, GETOPT_HELP_CHAR},
	{"version",no_argument, NULL, GETOPT_VERSION_CHAR},
	{0,0,0,0}
};

/**
 * usage - print out usage.
 * @status: Status code
 */
void usage(int status)
{
	FILE *out;
	switch (status) {
	case CMDLINE_FAILURE:
		out = stderr;
		break;
	default:
		out = stdout;
	}
	fprintf(out, "Usage: %s [OPTION]... [FILE]\n",
										PROGRAM_NAME);

	exit(status);
}

/**
 * version - print out program version.
 * @command_name: command name
 * @version:      program version
 * @author:       program authoer
 */
void version(const char *command_name, const char *version,
			const char *author)
{
	FILE *out = stdout;

	fprintf(out, "%s %s\n", command_name, version);
	fprintf(out, "Written by %s.\n", author);
}


/**
 * main - make a hexdump or do the reverse
 * @argc: count of arguments
 * @argv: command line argument vector
 */
int main(int argc, char *argv[])
{
	int opt;
	int longindex;
	bool infile = false;
	int fd = 0;
	size_t len;
	char data[BUFSIZE + 1];

	/**
	 * Initialize Phase.
	 * parse option, argument. set flags.
	 */
	while ((opt = getopt_long(argc, argv,
		"abc:g:iEpul:s:r",
		longopts, &longindex)) != -1) {
		switch (opt) {
		case 'a':
		case 'b':
		case 'c':
		case 'g':
		case 'i':
		case 'E':
		case 'p':
		case 'u':
		case 'l':
		case 's':
		case 'r':
		case GETOPT_HELP_CHAR:
			usage(EXIT_SUCCESS);
			break;
		case GETOPT_VERSION_CHAR:
			version(PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_AUTHOR);
			exit(EXIT_SUCCESS);
			break;
		default:
			usage(CMDLINE_FAILURE);
		}
	}
	if (argc > optind) {
		infile = true;
		if ((fd = open(argv[optind], O_RDONLY)) == -1) {
			perror("file open error");
			exit(EXIT_FAILURE);
		}
	}

	/**
	 * Main Phase.
	 * read input file(or stdio), output data.
	 */
	while ((len = read(fd, data, BUFSIZE)) > 0) {
		data[len] = '\0';
		printmsg(data, len);
	}

	/**
	 * Terminate Phase
	 */
	if (infile)
		close(fd);
	return 0;
}
