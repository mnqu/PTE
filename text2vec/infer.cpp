#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#define MAX_STRING 10000

const int hash_table_size = 30000000;

typedef float real;                    // Precision of float numbers

struct ClassVertex {
    double degree;
    char *name;
};

char infer_file[MAX_STRING], vector_file[MAX_STRING], output_file[MAX_STRING];
struct ClassVertex *vertex;
int binary = 0;
int *vertex_hash_table;
int max_num_vertices = 1000, num_vertices = 0, doc_size = 0;
int vector_size;
real *vec;
clock_t start;

std::vector< std::vector<int> > doc;

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
    
    FILE *fi = fopen(vector_file, "rb");
    char ch, word[MAX_STRING];
    real f_num;
    int size;
    
    fscanf(fi, "%d %d", &size, &vector_size);
    
    vec = (real *)malloc(size * vector_size * sizeof(real));
    
    for (long long k = 0; k != size; k++)
    {
        fscanf(fi, "%s", word);
        ch = fgetc(fi);
        ch++;
        AddVertex(word);
        for (int c = 0; c != vector_size; c++)
        {
            fread(&f_num, sizeof(real), 1, fi);
            vec[c + k * vector_size] = (real)f_num;
        }
    }
    fclose(fi);
    
    printf("Number of words: %d\n", num_vertices);
    printf("Vector dimension: %d\n", vector_size);
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

void ReadData()
{
    FILE *fi = fopen(infer_file, "rb");
    std::vector<int> vt;
    int wid;
    char word[MAX_STRING];
    doc_size = 0;
    while (1)
    {
        ReadWord(word, fi);
        if (feof(fi))
        {
            if (vt.size() != 0)
            {
                vt.push_back(0);
                doc.push_back(vt);
                doc_size++;
            }
            break;
        }
        
        wid = SearchHashTable(word);
        if (wid != -1) vt.push_back(wid);
        if (strcmp(word, "</s>") == 0)
        {
            doc.push_back(vt);
            doc_size++;
            vt.clear();
        }
    }
    fclose(fi);
    printf("Number of docs to infer: %d\n", doc_size);
}

void Process()
{
    BuildVocab();
    ReadData();
    
    FILE *fo;
    real *emb = (real *)calloc(vector_size, sizeof(real));
    fo = fopen(output_file, "wb");
    fprintf(fo, "%d %d\n", doc_size, vector_size);
    for (int a = 0; a < doc_size; a++)
    {
        fprintf(fo, "%d ", a);
        long long len = doc[a].size();
        for (int b = 0; b != vector_size; b++) emb[b] = 0;
        for (int k = 0; k != len; k++) for (int b = 0; b != vector_size; b++) emb[b] += vec[doc[a][k] * vector_size + b];
        for (int b = 0; b != vector_size; b++) emb[b] /= len;
        if (binary) for (int b = 0; b < vector_size; b++) fwrite(&emb[b], sizeof(real), 1, fo);
        else for (int b = 0; b < vector_size; b++) fprintf(fo, "%lf ", emb[b]);
        fprintf(fo, "\n");
    }
    fclose(fo);
    free(emb);
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
    output_file[0] = 0;
    if ((i = ArgPos((char *)"-infer", argc, argv)) > 0) strcpy(infer_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-vector", argc, argv)) > 0) strcpy(vector_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-output", argc, argv)) > 0) strcpy(output_file, argv[i + 1]);
    if ((i = ArgPos((char *)"-binary", argc, argv)) > 0) binary = atoi(argv[i + 1]);
    Process();
    return 0;
}