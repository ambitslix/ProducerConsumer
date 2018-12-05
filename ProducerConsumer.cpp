#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <thread>
#include <cstdlib>
#include <mutex>

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "ThreadPool.h"

#define MAX_ITEM_LENGTH         100
#define MAX_VALUE_SPACE         10          /** Only 10 different values of digits are possible */
#define MAX_THREADS             4
#define SPACE_DELAY             1000000     /** Delay in us between spaces */

using namespace std;

/** Error codes */
enum PC_ERRORS{

    ERR_SUCCESS =           0,
    ERR_INVALID_INPUT =     1,
    ERR_SYSTEM =            2
};

enum PC_SORT{

    SORT_NONE =             0,
    SORT_INSERT =           1,
    SORT_LOOKUP =           2
};

std::mutex OSL;           // Lock for protecting output stream

/** Write sorted vector to stream
*/
void write_token( ofstream &os, const vector<char> &v, const char *separator = ","){

    for( vector<char>::const_iterator i = v.begin(); i != v.end(); ++i){

        os << *i;

        if( separator && i + 1 != v.end()) os << separator;
    }
}

void insertionSorttoken( vector<char> &v){

    for( auto i = v.begin(); i != v.end(); i++){

        auto const insertion_point = upper_bound( v.begin(), i, *i);

        rotate( insertion_point, i, i + 1);
    }
}

void lookupSorttoken( vector<char> &v){

    int        a[MAX_VALUE_SPACE] = {0};

    for( auto i = v.begin(); i != v.end(); i++){

        a[(int)*i - 0x30]++;
    }

    auto i = v.begin();

    for( int j = 0; j < MAX_VALUE_SPACE; j++){

        int n = a[(int)j];

        if( n){

            for( int k = 0; k < n; k++){

                *i = char( j + 0x30);
                i++;
            }
        }
    }
}

/** Algorithm selector */
void sort_token( vector<char> &v, int algo = SORT_INSERT){

    if( algo == SORT_INSERT)
        insertionSorttoken( v);
    else
        lookupSorttoken( v);
}

/** Threads executes the following
*/
void tokenize( string item, off_t offset, int algo, ofstream &os){

    vector<char>    v;

    for( char& c : item) {

        if( isdigit( c)) v.push_back( c);
        else usleep( rand()%10000);
    }

    sort_token( v, algo);

/** Lock stream and perform seek and write
*/
    OSL.lock();

    os.seekp( offset, ios::beg);

    write_token( os, v); os << endl;

    OSL.unlock();
}

int main( int argc, char **argv){

    if( argc < 4){

        cout << "Insufficient parameters! Usage: " << argv[0] << " /inputFile /outputFile Sorting-method (1|2)" << endl;

        return( ERR_INVALID_INPUT);
    }

    ifstream        Input( argv[1]);
    ofstream        Output;
    string          sort_algorithm( argv[3]), algo_insert( "INSERT"), algo_heap( "LOOKUP");
    int             algo;

    if( Input.fail()){

        cerr << "Error: " << strerror( errno) << ", opening " << argv[1] << endl;

        return( ERR_SYSTEM);
    }

    Output.open( argv[2], ios::out|ios::binary);

    if( Output.fail()){

        cerr << "Error: " << strerror( errno) << ", opening " << argv[2] << endl;

        return( ERR_SYSTEM);
    }

    if( sort_algorithm == algo_insert) algo = SORT_INSERT;
    else
    if( sort_algorithm == algo_heap) algo = SORT_LOOKUP;
    else{

        cerr << "Error: Invalid sort algorithm " << sort_algorithm << ". Valid choices are: " << algo_insert << "|" << algo_heap << endl;

        return( ERR_INVALID_INPUT);
    }

    off_t           offset = 0;
    ThreadPool      pool( MAX_THREADS);

    for( string item; getline( Input, item);){

        size_t spaces = count( item.begin(), item.end(), ' ');

        if( item.size() - spaces > 0){

            pool.work( bind( tokenize, item, offset, algo, ref( Output)));

            offset += (item.size() - spaces) << 1;
        }
    }

    cout << "Sorted " << offset << " bytes using " << sort_algorithm << endl;

    return( ERR_SUCCESS);
}
