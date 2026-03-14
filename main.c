#include "file_search_engine.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <directory> <word_to_search>\\n", argv[0]);
        return 1;
    }

    index *idx = init_index();
    scan_directory(idx, argv[1]);
    printFile(idx, argv[2]);
    free_index(idx);

    return 0;
}

