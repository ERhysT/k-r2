/* Excercise 8-5. Modify the fsize program to print the other
   information contained in the inode entry.  */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>		/* for determining username */

void fsize(const char *);
void dirwalk(const char *dir, void (*fcn)(const char *));

/* fsize(): print size of the file with name */
void fsize(const char *name)
{
    struct stat stbuf;
    struct passwd *pwbuf;

    if (stat(name, &stbuf) == -1) {
	fprintf(stderr, "fsize: can't access %s\n", name);
	return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
	dirwalk(name, fsize);
    pwbuf = getpwuid(stbuf.st_uid);
    printf("%8ld\t%6o\t%s\t%s\n",
	   stbuf.st_size, stbuf.st_mode, pwbuf->pw_name, name);
}

/* dirwalk(): apply fcn to all files in dir */
void dirwalk(const char *dir, void (*fcn)(const char *))
{
    char name[PATH_MAX];
    struct dirent *dp;
    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL) {
	fprintf(stderr, "dirwalk: can't open %s\n", dir);
	return;
    }
    while ((dp = readdir(dfd)) != NULL) {
	if (strcmp(dp->d_name, ".") == 0 
	 || strcmp(dp->d_name, "..") == 0)
	    continue;		/* skip self and parent */
	if (strlen(dir)+strlen(dp->d_name)+2 > sizeof name)
	    fprintf(stderr, "dirwalk: name %s/%s too long\n",
		    dir, dp->d_name);
	else {
	    sprintf(name, "%s/%s", dir, dp->d_name);
	    (*fcn)(name);
	}
    }
    closedir(dfd);
}

/* 8-5: prints file sizes the directories provided as arguements, or
   current directory if no arguments provided. */
int main(int argc, char **argv)
{
    if (argc == 1)
	fsize(".");
    else
	while (*++argv)
	    fsize(*argv);
    return 0;
}



