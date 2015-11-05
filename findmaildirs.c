/*
 * Copyright (c) 2012-2015 Dmitry Marakasov <amdmi3@amdmi3.ru>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void checkdir(const char* path, const char* prefix) {
	DIR *d;
	struct dirent *de;
	struct stat st;

	char fullpath[FILENAME_MAX];
	char fullprefix[FILENAME_MAX];

	if ((d = opendir(path)) == NULL)
		err(1, "opendir(%s)", path);

	while ((de = readdir(d)) != NULL) {
		if (de->d_name[0] == '.') {
			if (de->d_name[1] == '\0')
				continue;
			else if (de->d_name[1] == '.' && de->d_name[2] == '\0')
				continue;
		}

		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, de->d_name);

		if (lstat(fullpath, &st) != 0)
			err(1, "stat(%s)", fullpath);

		if (!S_ISDIR(st.st_mode))
			continue;

		if (strcmp(de->d_name, "cur") == 0)
			printf("+%s ", prefix);

		if (strcmp(de->d_name, "cur") != 0
			&& strcmp(de->d_name, "new") != 0
			&& strcmp(de->d_name, "tmp") != 0) {
			if (prefix == NULL)
				snprintf(fullprefix, sizeof(fullprefix), "%s", de->d_name);
			else
				snprintf(fullprefix, sizeof(fullprefix), "%s/%s", prefix, de->d_name);

			checkdir(fullpath, fullprefix);
		}
	}

	if (closedir(d) != 0)
		errx(1, "closedir");
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		fprintf(stderr, "usage: %s dir\n", argv[0]);
		return 1;
	}

	checkdir(argv[1], NULL);

	return 0;
}
