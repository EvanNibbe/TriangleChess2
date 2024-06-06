/*
January 4, 2020
Copyright 2020 Evan Nibbe
This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

        You should have recieved a copy of the notice within the repository:
        https://github.com/EvanNibbe/FastDebugForC_Cpp
This is for using in conjunction with ./dependency.out deppid.bin
*/
#ifndef DEBUG_HELPER
#define DEBUG_HELPER
#ifndef SIG_ERR
#include <signal.h>
#endif

#include <stdio.h>

#include <sys/types.h>

#include <unistd.h>

void signal26(int arg) {
	; //end waiting
}
pid_t debug_helper_pid=0;
void println(int line) {
	if (debug_helper_pid==0) {
		FILE *fp=fopen("deppid.bin", "r");
		fread(&debug_helper_pid, sizeof(pid_t), 1, fp);
		fclose(fp);
		signal(26, signal26);
		fp=fopen("calling_pid.bin", "w");
		pid_t temp=getpid();
		fwrite(&temp, sizeof(pid_t), 1, fp);
		fclose(fp);
	}
	kill(debug_helper_pid, 23); //start of number
	sleep(1); //synchronize with the dependency program to not get ahead of it.
	for (int i=ceil(log(line)/log(2)); i>=0; i--) {
		if ((line & (1<<i))==0) {
			kill(debug_helper_pid, 24);
		} else {
			kill(debug_helper_pid, 25); //there was a 1 in binary
		}
		sleep(1);
	}
	kill(debug_helper_pid, 26); //end of number
	sleep(1);
}
#endif
