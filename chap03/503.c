/*
 * BSD-3-Clause
 *
 * Copyright (c) 2020 Wonpyo Kim, 
 * Hallym University Software Convergence School, Hongpub. 
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this 
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this 
 *    list of conditions and the following disclaimer in the documentation and/or other 
 *    materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without specific 
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void signal_handler(int sig);

void
main(int argc, char *argv[])
{
	int count = 0;
	struct sigaction sigact;
	struct sigaction sigact_def;

	if (argc != 2 || argv[1][0] != '-') {
		perror("Usage: ./a.out -[a]");
		exit(1);
	}

	getopt(argc, argv, "a");

	switch (argv[1][1]) {
	case 'a':
		sigact.sa_handler = signal_handler;
		sigact.sa_flags = 0;
		if (sigfillset(&sigact.sa_mask) == -1) {
			perror("sigfillset() failed");
			exit(1);
		}
		if (sigaction(SIGINT, &sigact, &sigact_def) == -1) {
			perror("sigaction() failed");
			exit(1);
		}
		if (sigaction(SIGQUIT, &sigact, &sigact_def) == -1) {
			perror("sigaction() failed");
			exit(1);
		}
		for (;;) {
			printf("I'm still running...\n");
			sleep(1);
			count++;
			if (count == 10) {
				if (sigaction(SIGINT, &sigact_def, NULL) == -1) {
					perror("sigaction() failed");
					exit(1);
				}
			}
		}
		break;
	}

	exit(0);
}

static void 
signal_handler(int sig)
{
	static int count = 0;

	if (sig == SIGINT) {
		count++;
		printf("SIGINT detected %d times.\n", count);
	} else {
		printf("Another signal is detected.\n");
	}

	return;
}

