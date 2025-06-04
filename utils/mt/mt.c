/****************************************************
    mt - Multi-Tool
    a project-aware shell tool for cais-rl.
    written by: @Tarek-Sallam
****************************************************/

#include <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PROJECTS 50
#define MAX_PATH_LEN 512
#define MAX_NAME_LEN 64

// a struct for each project
typedef struct {
    char name[MAX_NAME_LEN];
    char path[MAX_PATH_LEN];
} Project;

// the multi-tool struct
typedef struct {
    char repo_root[MAX_PATH_LEN];
    Project projects[MAX_PROJECTS];
    int project_count;
} MT;

// finds the root of the git repository
int find_repo_root(char *repo_root) {
    char cwd[MAX_PATH_LEN];
    char git_path[MAX_PATH_LEN];
    struct stat st;

    if (!getcwd(cwd, sizeof(cwd))) {
        return -1;
    }

    strcpy(repo_root, cwd);

    while (strlen(repo_root) > 1) {
        snprintf(git_path, sizeof(git_path), "%s/.git", repo_root);
        if (stat(git_path, &st) == 0) {
            return 0;
        }

        char *last_slash = strrchr(repo_root, '/');
        if (last_slash && last_slash != repo_root) {
            *last_slash = '\0';
        } else {
            break;
        }
    }

    return 0;
}

int load_projects_config(MT *mt);

int which_project(MT *mt) {}

int find_script_in_project(MT *mt, const char *script_name, char *script_path) {
}

int find_script_in_repo(MT *mt, const char *script_name, char *script_path) {}

int find_script(MT *mt, const char *script_name, char *script_path) {}

int list_project_scripts(MT *mt) {}

int run_script(MT *mt, const char *script_name) {}

int run_script(MT *mt, const char *script_name) {}

int get_project_path(MT *mt, const char *project_name, char *project_path) {}
