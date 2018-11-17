#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void help(char *argv)
{
    cerr << "Usage: " << argv << " [-h] [-f input_file] [-s search_code] [-l match_count]" << endl;
    cerr << " -h	Help." << endl;
    cerr << " -f	Input file. If not specified, STDIN will be used." << endl;
    cerr << " -s	Decimal code to search for. Default is 0." << endl;
    cerr << " -l	How many consecutive codes must be matched. Default is 50." << endl;
    return;
}

int show_ranges(string filename, unsigned int search, unsigned int search_limit)
{
    FILE *file;
    unsigned char ch;
    //unsigned int search = 0;
    //unsigned int search_limit = 50;
    bool match = false;
    unsigned int match_count = 0;
    long count = 0;
    long start;
    long end;

    file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cerr << "Cannot open input file!" << endl;
        return 2;
    }

    while (fread(&ch, 1, 1, file)) {
        count++;
        if ((int)ch == search) {
            match_count++;
            if (match == false) {
                match = true;
                start = count-1;
            }
        }

        if (match == true && (int)ch != search) {
            match = false;
            end = count-2;
            if (match_count >= search_limit) {
                cout << start << " : " << end << " : " << (end-start) << endl;
            }
            match_count = 0;
        }
    }

    if (match == true) {
        end = count-2;
        if (match_count >= search_limit) {
            cout << start << " : " << end << " : " << (end-start) << endl;
        }
    }

    fclose(file);

    return 0;
}

int main(int argc, char *argv[])
{
    int opt;
    string filename = "/dev/stdin";
    unsigned int search = 0;
    unsigned int search_limit = 50;

    while ((opt = getopt(argc, argv, "hf:s:l:")) != -1) {
        switch (opt) {
            case 'f':
                filename.assign(optarg);
                break;
            case 's':
                search = atoi(optarg);
                break;
            case 'l':
                search_limit = atoi(optarg);
                break;
            case 'h':
                help(argv[0]);
                return 0;
            default:
                help(argv[0]);
                return 1;
        }
    }

    return show_ranges(filename, search, search_limit);
}

