#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "tracdir.h"

static char *tracpath = NULL;

char *get_default_tracpath() {
   if (tracpath == NULL) {
#ifdef _DEBUG
      fprintf(stderr, "constructing path to directory in home\n");
#endif
      char *homedir = getenv("HOME");
      if (homedir == NULL) {
#ifdef _DEBUG
         fprintf(stderr, "Unable to determine home-directory\n");
#endif
         return NULL;
      }
      const char *tracdir = "/.trac";
      tracpath = (char*) malloc((strlen(homedir) + strlen(tracdir) + 1) * sizeof(char));
      strcpy(tracpath, homedir);
      strcat(tracpath, tracdir);
#ifdef _DEBUG
      fprintf(stderr, "default trac directory is: %s\n", tracpath);
#endif
   }
   return tracpath;
}

void *set_custrom_tracpath(char *custom_path) {
   if (tracpath == NULL) {
#ifdef _DEBUG
      fprintf(stderr, "custom trac directory is: %s\n", custom_path);
#endif
      tracpath = strdup(custom_path);
   } else {
      if (strcmp(custom_path, tracpath) != 0) {
#ifdef _DEBUG
         fprintf(stderr, "trac directory is already set to \"%s\". Changing trac directory to \"%s\"\n", tracpath, custom_path);
#endif
         free_tracpath();
         tracpath = strdup(custom_path);
#ifdef _DEBUG
      } else {
         fprintf(stderr, "trac directory is already set to the desired value \"%s\"\n", tracpath);
#endif
      }
   }
   return tracpath;
}

char *get_current_tracpath() {
   return tracpath;
}

void free_tracpath() {
   if (tracpath != NULL) {
      free(tracpath);
      tracpath = NULL;
   }
}

int rek_mkdir(char *path) {
    char *sep = strrchr(path, '/');
    if(sep != NULL) {
        *sep = 0;
        rek_mkdir(path);
        *sep = '/';
    }
    return mkdir(path, S_IRWXU) && errno != EEXIST;
}

int create_tracpath_dir() {
   struct stat sb;
   if (tracpath != NULL) {
      if (stat(tracpath, &sb) == 0) {
#ifdef _DEBUG
         fprintf(stderr, "trac directory %s already exists\n", tracpath);
#endif
         return 0;
      } else {
#ifdef _DEBUG
         fprintf(stderr, "attempting to create trac directory %s ... ", tracpath);
#endif
         int success = rek_mkdir(tracpath);
         if (success) {
#ifdef _DEBUG
            fprintf(stderr, "failed\n");
            fprintf(stderr, "   %s\n", strerror(errno));
#endif
            return -1;
         } else {
#ifdef _DEBUG
            fprintf(stderr, "success\n");
#endif
            return 0;
         }
      }
   } else {
#ifdef _DEBUG
      fprintf(stderr, "trac directory was not defined yet\n");
#endif
      return -1;
   }
}
