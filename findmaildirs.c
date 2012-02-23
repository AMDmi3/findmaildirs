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
		err(1, "opendir");

	while ((de = readdir(d)) != NULL) {
		if (de->d_name[0] == '.') {
			if (de->d_name[1] == '\0')
				continue;
			else if (de->d_name[1] == '.' && de->d_name[2] == '\0')
				continue;
		}

		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, de->d_name);

		if (stat(fullpath, &st) != 0)
			err(1, "stat");

		if (!S_ISDIR(st.st_mode))
			continue;

		if (strcmp(de->d_name, "cur") == 0 || strcmp(de->d_name, "new") == 0 || strcmp(de->d_name, "tmp") == 0) {
			printf("+%s ", prefix);
			if (closedir(d) != 0)
				errx(1, "closedir");
			return;
		} else {
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
	if (argc <= 1)
		errx(1, "usage: %s dir\n", argv[0]);

	checkdir(argv[1], NULL);

	return 0;
}
