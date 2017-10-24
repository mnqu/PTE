#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>
#include <map>
#define MAX_STRING 10000
using namespace std;

const int hash_table_size = 30000000;

struct ClassVertex {
    double degree;
    char *name;
};

struct bigram
{
    int u;
    int v;
    friend bool operator<(bigram b1, bigram b2)
    {
        if(b1.u == b2.u)
            return b1.v < b2.v;
        return b1.u < b2.u;
    }
};

map<bigram,long long> bgm2cnt;
long long totaltoken = 0;
int min_count = 0, window;
char text_file[MAX_STRING], output_file[MAX_STRING], output_words[MAX_STRING];

struct ClassVertex *vertex;
int *vertex_hash_table;
int max_num_vertices = 1000, num_vertices = 0;

unsigned int Hash(char *key)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*key)
    {
        hash = hash * seed + (*key++);
    }
    return hash % hash_table_size;
}

void InitHashTable()
{
    vertex_hash_table = (int *)malloc(hash_table_size * sizeof(int));
    for (int k = 0; k != hash_table_size; k++) vertex_hash_table[k] = -1;
}

void InsertHashTable(char *key, int value)
{
    int addr = Hash(key);
    while (vertex_hash_table[addr] != -1) addr = (addr + 1) % hash_table_size;
    vertex_hash_table[addr] = value;
}

int SearchHashTable(char *key)
{
    int addr = Hash(key);
    while (1)
    {
        if (vertex_hash_table[addr] == -1) return -1;
        if (!strcmp(key, vertex[vertex_hash_table[addr]].name)) return vertex_hash_table[addr];
        addr = (addr + 1) % hash_table_size;
    }
    return -1;
}

/* Add a vertex to the vertex set */
int AddVertex(char *name)
{
    int length = strlen(name) + 1;
    if (length > MAX_STRING) length = MAX_STRING;
    vertex[num_vertices].name = (char *)calloc(length, sizeof(char));
    strcpy(vertex[num_vertices].name, name);
    vertex[num_vertices].degree = 0;
    num_vertices++;
    if (num_vertices + 2 >= max_num_vertices)
    {
        max_num_vertices += 1000;
        vertex = (struct ClassVertex *)realloc(vertex, max_num_vertices * sizeof(struct ClassVertex));
    }
    InsertHashTable(name, num_vertices - 1);
    return num_vertices - 1;
}

void BuildVocab()
{
    vertex = (struct ClassVertex *)calloc(max_num_vertices, sizeof(struct ClassVertex));
    vertex_hash_table = (int *)malloc(hash_table_size * sizeof(int));
    for (int k = 0; k != hash_table_size; k++) vertex_hash_table[k] = -1;
    
    char word[MAX_STRING];
    FILE *fin;
    fin = fopen(text_file, "rb");
    num_vertices = 0;
    AddVertex((char *)"</s>");
    while (1)
    {
        if (fscanf(fin, "%s", word) != 1) break;
        totaltoken++;
        if (totaltoken % 100000 == 0) {
            printf("%lldK%c", totaltoken / 1000, 13);
            fflush(stdout);
        }
        if (SearchHashTable(word) == -1) AddVertex(word);
    }
    printf("Number of tokens: %lld\n", totaltoken);
    printf("Number of words: %d\n", num_vertices);
    fclose(fin);
}

void ReadWord(char *word, FILE *fin) {
    int a = 0, ch;
    while (!feof(fin)) {
        ch = fgetc(fin);
        if (ch == 13) continue;
        if ((ch == ' ') || (ch == '\t') || (ch == '\n')) {
            if (a > 0) {
                if (ch == '\n') ungetc(ch, fin);
                break;
            }
            if (ch == '\n') {
                strcpy(word, (char *)"</s>");
                return;
            }
            else continue;
        }
        word[a] = ch;
        a++;
        if (a >= MAX_STRING - 1) a--;   // Truncate too long words
    }
    word[a] = 0;
}

void Process()
{
    BuildVocab();
    
    FILE *fi = fopen(text_file, "rb");
    int wid;
    char word[MAX_STRING];
    bigram bgm;
    int *buf = new int [window + 2];
    int pst = 0, exch = 0;
    long long count1 = 0, count2 = 0;
    
    while(!feof(fi))
    {
        ReadWord(word, fi);
        wid = SearchHashTable(word);
        if(wid == -1) continue;
        
        if (count2 - count1 > 10000)
        {
            printf("%cRead file: %.3lf%%", 13, (double)(count2) / totaltoken * 100);
            fflush(stdout);
            count1 = count2;
        }
        
        count2++;
        
        if (wid == 0) { pst = 0; exch = 0; continue; }
        
        for (int k = 0; k != pst; k++)
        {
            bgm.u = wid; bgm.v = buf[k]; bgm2cnt[bgm]++;
            bgm.u = buf[k]; bgm.v = wid; bgm2cnt[bgm]++;
        }
        
        if (pst < window) buf[pst++] = wid;
        else
        {
            buf[exch++] = wid;
            if (exch >= window) exch = 0;
        }
    }
    printf("\n");
    fclose(fi);
    
    FILE *fo = fopen(output_file, "wb");
    long long bgmsize = bgm2cnt.size();
    printf("Number of edges: %lld\n", bgmsize);
    long long written = 0;
    map<bigram, long long>::iterator iter = bgm2cnt.begin();
    while (iter != bgm2cnt.end())
    {
        if (written % 10000 == 0)
        {
            printf("%cWrite file: %.3lf%%", 13, double(written) / bgmsize * 100);
            fflush(stdout);
        }
        fprintf(fo,"%s\t%s\t%lld\tw\n", vertex[(iter->first).u].name, vertex[(iter->first).v].name, (iter->second));
        
        written++;
        iter++;
    }
    printf("\n");
    fclose(fo);
    
    fo = fopen(output_words, "w");
    for (int k = 0; k != num_vertices; k++) fprintf(fo, "%s\n", vertex[k].name);
    fclose(fo);
}

int ArgPos(char *str, int argc, char **argv) {
    int a;
    for (a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
        if (a == argc - 1) {
            printf("Argument missing for %s\n", str);
            exit(1);
        }
        return a;
    }
    return -1;
}

int main(int argc, char **argv) {
    int i;
    if (argc == 1) {
        return 0;
    }
    if ((i = ArgPos((char *)"-text", argc, argv)) > 0) strcpy(text_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-output-ww", argc, argv)) > 0) strcpy(output_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-output-words", argc, argv)) > 0) strcpy(output_words, argv[i + 1]);
    if ((i = ArgPos((char *)"-window", argc, argv)) > 0) window = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-min-count", argc, argv)) > 0) min_count = atoi(argv[i + 1]);
    Process();
    return 0;
}