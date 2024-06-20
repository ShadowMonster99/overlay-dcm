#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("unable to open input file, ensure the cwd is steams root dir");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

void create_file(int num_bytes, const char *filename) {
    FILE *file;
    char *data;

    data = (char *)malloc(num_bytes * sizeof(char));
    if (data == NULL) {
        fprintf(stderr, "error allocating memory\n");
        return;
    }

    for (int i = 0; i < num_bytes; i++) {
        data[i] = 'A' + (i % 26);
    }

    file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "error opening file %s for writing\n", filename);
        free(data);
        return;
    }

    size_t written = fwrite(data, sizeof(char), num_bytes, file);
    if (written != num_bytes) {
        fprintf(stderr, "error writing data to file\n");
        fclose(file);
        free(data);
        return;
    }

    fclose(file);
    free(data);

    printf("successfully wrote %d bytes to %s\n", num_bytes, filename);
}

static const int rename_file(const char* filename) {
    char result[256];   

    strcpy(result, filename);
    strcat(result, ".old");

    return rename(filename, result);
}

static inline int patch_x86_64(const char* filename, long file_size) {

    if (file_size != -1) {
        printf("%s is %ld bytes.\n", filename, file_size);

        rename_file(filename);
        create_file(file_size, filename);
    }
}

static inline int patch_i686(const char* filename, long file_size) {

    if (file_size != -1) {
        printf("%s is %ld bytes.\n", filename, file_size);

        rename_file(filename);
        create_file(file_size, filename);
    }
}

static const int delete_cache() {

    const char *filename = "steam.signatures";

    if (remove(filename) == 0) {
        printf("file deleted successfully.\n");
    } else {
        perror("error deleting the file");
    }
}

int main() {

    delete_cache();

    const char *x86_64 = "GameOverlayRenderer64.dll";
    const char *i686 = "GameOverlayRenderer.dll";

    patch_x86_64(x86_64, get_file_size(x86_64));
    patch_i686(i686, get_file_size(i686));

    return 0;
}