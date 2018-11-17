#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <magic.h>

using namespace std;

magic_t magic;

void help(char *argv)
{
    cerr << "Usage: " << argv << " [-h] [-f input_file] [-s buffer_size]" << endl;
    cerr << " -h	Help." << endl;
    cerr << " -f	Input file. If not specified, STDIN will be used." << endl;
    cerr << " -s	Input buffer size. Default is 1024." << endl;
    return;
}

int magicscan(string filename, long buffersz)
{
    FILE *file;
    char buffer[buffersz];
    long pos = 0;
    string magic_str;

    file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cerr << "Cannot open input file!" << endl;
        return 2;
    }

    while (fread(&buffer, buffersz, 1, file)) {
        magic_str = magic_buffer(magic, &buffer, buffersz);
        cout << pos << ": " << magic_str << endl;
        pos++;
        fseek(file, pos, SEEK_SET);
    }

    fclose(file);

    return 0;
}

int main(int argc, char *argv[])
{
    int opt;
    string filename = "/dev/stdin";
    long buffersz = 1024;

    while ((opt = getopt(argc, argv, "hf:s:")) != -1) {
        switch (opt) {
            case 'f':
                filename.assign(optarg);
                break;
            case 's':
                buffersz = atoi(optarg);
                break;
            case 'h':
                help(argv[0]);
                return 0;
            default:
                help(argv[0]);
                return 1;
        }
    }

    magic = magic_open(MAGIC_NONE);
    magic_load(magic, NULL);

    return magicscan(filename, buffersz);
}

