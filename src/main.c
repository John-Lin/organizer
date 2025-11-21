#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 1024

typedef struct {
  char *ext;
  char *folder;
} Rule;

Rule rules[] = {
    {".txt", "Documents"}, {".pdf", "Documents"},
    {".jpg", "Images"},    {".docx", "Documents"},
    {".png", "Images"},    {".gif", "Images"},
    {".doc", "Documents"}, {".jpeg", "Images"},
    {".mkv", "Videos"},    {".mp4", "Videos"},
    {".mp3", "Music"},     {".zip", "Archives"},
    {".tar", "Archives"},  {".gz", "Archives"},
    {".epub", "Books"},    {".m4a", "Videos"},
    {".wav", "Videos"},    {".yml", "Documents"},
    {".md", "Documents"},
    {NULL, NULL} // Sentinel value
};

// Get file extension from filename
char *get_extension(const char *filename) {
  // Returns pointer to the file extension in filename
  char *dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return NULL;
  return dot;
}

// Find the folder for a given extension
char *find_folder(const char *ext) {
  if (!ext)
    return NULL;

  for (int i = 0; rules[i].ext != NULL; i++) {
    if (strcasecmp(ext, rules[i].ext) == 0) {
      return rules[i].folder;
    }
  }
  return "Others";
}

// Create directory if it doesn't exist
int create_dir(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == 0) {
    // path exists, check if it's a directory
    return S_ISDIR(st.st_mode) ? 0 : -1;
  }
  return mkdir(path, 0755);
}

// Move file from src_path to dest_path
int move_file(const char *src_path, const char *dest_path) {
  if (!src_path || !dest_path)
    return -1;
  return rename(src_path, dest_path);
}

// Remove trailing slash from path if present
void cleanup_path(char *path) {
  if (!path)
    return;
  size_t len = strlen(path);
  while (len > 0 && path[len - 1] == '/') {
    path[--len] = '\0';
  }
}

// Organize files in the target directory
int organize_directory(const char *target_dir) {
  if (!target_dir) {
    fprintf(stderr, "Error: null directory path\n");
    return -1;
  }

  DIR *dir = opendir(target_dir);
  if (!dir) {
    perror("Failed to open directory");
    return -1;
  }

  char work_dir[MAX_PATH];
  strncpy(work_dir, target_dir, MAX_PATH - 1);
  work_dir[MAX_PATH - 1] = '\0';
  cleanup_path(work_dir);

  struct dirent *entry;
  int moved = 0, failed = 0;

  while ((entry = readdir(dir)) != NULL) {
    // Skip current and parent directory entries
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s/%s", work_dir, entry->d_name);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1 || S_ISDIR(file_stat.st_mode)) {
      continue; // Skip directories
    }

    char *ext = get_extension(entry->d_name);
    char *folder = find_folder(ext);

    if (!folder) {
      failed++;
      continue;
    }

    char dest_folder[MAX_PATH];
    snprintf(dest_folder, MAX_PATH, "%s/%s", work_dir, folder);

    if (create_dir(dest_folder) == -1) {
      fprintf(stderr, "Failed to create directory: %s\n", dest_folder);
      failed++;
      continue;
    }

    char dest_path[MAX_PATH];
    snprintf(dest_path, MAX_PATH, "%s/%s", dest_folder, entry->d_name);

    if (move_file(full_path, dest_path) == 0) {
      printf("Moved: %s -> %s\n", entry->d_name, folder);
      moved++;
    } else {
      fprintf(stderr, "Failed to move file: %s\n", entry->d_name);
      failed++;
    }
  }

  closedir(dir);
  printf("\nDone! Moved: %d, Failed: %d\n", moved, failed);
  return 0;
}

int main(int argc, char *argv[]) {
  const char *target = (argc > 1) ? argv[1] : ".";
  printf("Organizing files in: %s\n", target);

  if (organize_directory(target) != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
