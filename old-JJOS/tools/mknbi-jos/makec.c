/*
 * makec.c  -  Create C file from binary image
 *
 * Copyright (C) 1996-1998 Gero Kuhlmann   <gero@gkminix.han.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>


void main(argc, argv)
int argc;
char **argv;
{
  int nr, pos;
  char arrayname[16];
  unsigned char buf[256];
  unsigned char *cp;

  if (argc > 1)
	strncpy(arrayname, argv[1], 15);
  else
	strcpy(arrayname, "first");
  arrayname[15] = '\0';

  printf("/* This file has been generated automatically by makec */\n");
  printf("/* DO NOT EDIT! */\n\n");
  printf("unsigned char %s_data[] = {\n", arrayname);
  pos = 0;
  while ((nr = read(STDIN_FILENO, buf, 256)) > 0) {
	cp = buf;
	for (; nr > 0; nr--) {
		printf("0x%02x,", *cp++);
		if (pos++ >= 12) {
			printf("\n");
			pos = 0;
		}
	}
  }
  printf("0};\n\n");
  printf("unsigned int %s_data_size = sizeof(%s_data) - 1;\n",
							arrayname, arrayname);
  exit(0);
}
