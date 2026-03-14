#include "file_search_engine.h"
#include <ctype.h>
#include <string.h> // for strdup

index* init_index(void){
    index* Index = (index*)malloc(sizeof(index));
    if(Index == NULL){
        printf("Impossibile allocare lo spazio");
        exit(0);
    }
    Index->head = NULL;
    return Index;
}

void scan_directory(index* index, const char* path){
    DIR* dir = opendir(path); //apre una directory 
    if(dir == NULL){ //se il puntatore risulta nullo restituisce un errore
        printf("Impossibile aprire la directory");
        exit(0);
    }
    struct dirent* direntry; //puntatore alla entry della directory

    while((direntry = readdir(dir)) != NULL){ //inserisce in direntry un puntatore alla directory su cui è settato il puntatore di readdir
        if(strcmp(direntry->d_name, ".") ==0 || strcmp(direntry->d_name , "..") == 0 ){
            continue; //ignoriamo . e .. 
        }
        char filepath[512]; // Larger buffer to avoid sprintf overflow
        snprintf(filepath, sizeof(filepath), "%s/%s", path, direntry->d_name); // Safe concat with / separator

        index_file(index, filepath); //inserisce il file nell'indice
    }
    closedir(dir);
}

void index_file(index* Index, const char *filename){
    FILE* file = fopen(filename, "r"); //apre il file da leggere
    if(file==NULL){
        return;
    }
    char word[100]; //buffer per ogni parola

    while(fscanf(file, "%s", word)!=EOF){ //legge la parola 

        normalize_word(word); //pulisce la parola
        add_word(Index, word, filename); //aggiunge la parola all'indice
    }
    fclose(file); //chude il file
}

void normalize_word(char *word) {

    int len = strlen(word); //lunghezza della parola
    int j = 0; 
    int i; // Declared outside loop to avoid redeclaration bloat

    for (i = 0; i < len; i++) { 
        if (isalpha(word[i])) { //controlla se il carattere è alfanumerico
            word[j] = tolower(word[i]); //se il carattere è alfanumerico lo scrive in lowercase e oi incrementa il counter
            //in questo modo scriviamo soltanto i caratteri in lower case della parola e ignoriamo punteggiatura o spazi
            j++;
        }
    }
    word[j] = '\0'; //a fine stringa inseriamo il teminatore
}

void add_word(index* Index, const char* word, const char *filename){
    wordentry *current = Index->head; //scorre le wordentry fino a trovare la parola se è presente
    while(current){
        if(strcmp(current->word, word)==0){
            break;
        }
        current=current->next;
    }
    if(current == NULL){ //se non è presente crea una nuova wordentry e ci scrive la parola
        wordentry *new_word = (wordentry*)malloc(sizeof(wordentry));
        if (!new_word) return;
        new_word->word = strdup(word); // Use strdup for safe copy + free later
        new_word->files = NULL;
        new_word->next = Index->head;
        Index->head = new_word;
        current = new_word; 
    }

    //a questo punto current è sempre sulla wordentry della parola
    fileNode* file = current->files;
    while(file){ //scorre i file in cui compare la parola per trovare quello dove è stata letta la parola
        if(strcmp(file->filename, filename) == 0){
            file->count++; //se il file è stato trovato ed esiste incrementa il contatore di occorrenze
            return;
        }
        file = file->next;
    }
    fileNode *nf = (fileNode*)malloc(sizeof(fileNode)); //se siamo arrivati qui significa che il file non è stato trovato
    strcpy(nf->filename, filename); //alloca un nuovo fileNode e copi il filename in esso
    nf->count = 1; //inizializza il count
    nf->next = current->files;
    current->files = nf;  

}

wordentry* search_word(index *Index, const char *word){
    wordentry *current = Index->head;
    while(current){
        if(strcmp(current->word,word)==0){
            return current;
        }
        current = current->next;
    }
    printf("Parola non trovata.");
    return NULL;
}

void printFile(index *Index, const char* word){
    wordentry* we = search_word(Index, word);
    if(!we) return;
    fileNode* fn = we->files;
    while(fn){
        printf("La parola compare %d volte nel file %s \n", fn->count, fn->filename);
        fn = fn->next;
    }
}

void free_index(index *Index){
    if (!Index) return;
    wordentry *we = Index->head;
    while (we) {
        wordentry *we_next = we->next;
        fileNode *fn = we->files;
        while (fn) {
            fileNode *fn_next = fn->next;
            free(fn);
            fn = fn_next;
        }
        free(we->word);
        free(we);
        we = we_next;
    }
    free(Index);
}

int count_total_words(index* Index){
    wordentry *we = Index->head;
    int count=0;
    while(we){
        count++;
    }
    return count;
}
