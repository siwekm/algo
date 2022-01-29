#ifndef __PROGTEST__

#include <iostream>
#include "flib.h"
#endif //__PROGTEST__

void bubble(int32_t *numbers, int size){
    bool changed = false;
    int32_t tmp;
    do {
       changed = false;
        for (int i = 0; i < size - 1; i++) {
            if (numbers[i] > numbers[i + 1]){
                tmp = numbers[i];
                numbers[i] = numbers[i + 1];
                numbers[i + 1] = tmp;
                changed = true;
            }
        }

    }while (changed);
}


int cmp (const void * a, const void * b) {
    return *(int*)a - *(int*)b;
}

 class Reader{

    int64_t buffer_size;
    int file_a_index, file_b_index;

    public:
    int i_a = 0, i_b = 0;
    int a_loaded, b_loaded;
    int32_t *input_a_numbers;
    int32_t *input_b_numbers;
    bool end_a = false, end_b = false;

    Reader(int file_a_index, int file_b_index, int64_t buffer_size){
        a_loaded = 0;
        b_loaded = 0;
        this->file_a_index = file_a_index;
        this->file_b_index = file_b_index;
        this->buffer_size = buffer_size/2;
        input_a_numbers = new int32_t[this->buffer_size];
        input_b_numbers = new int32_t[this->buffer_size];
        a_loaded = flib_read(file_a_index, input_a_numbers, this->buffer_size);
        b_loaded = flib_read(file_b_index, input_b_numbers, this->buffer_size);
    }

    //read next max numbers from file - return false when eof reached
    bool readChunk(int file_index, int32_t *numbers_buffer, int buffer_size, int *num_loaded){
            *num_loaded = flib_read(file_index, numbers_buffer, buffer_size);
            if(*num_loaded < buffer_size){
                return false;
            }
        return true;
    }
    ~Reader(){
        delete [] input_a_numbers;
        delete [] input_b_numbers;
    }

    int getNextMin(){
        //end of buffer reached - load new
        if(!end_a && i_a == a_loaded){
            i_a = 0;
            end_a = !readChunk(file_a_index, input_a_numbers, this->buffer_size, &a_loaded);
        }
        if(!end_b && i_b == b_loaded){
            i_b = 0;
            end_b = !readChunk(file_b_index, input_b_numbers, this->buffer_size, &b_loaded);
        }

        if (i_a < a_loaded && i_b < b_loaded){
            if (input_a_numbers[i_a] >= input_b_numbers[i_b]) {
                return input_b_numbers[i_b++];
            } else {
                return input_a_numbers[i_a++];
            }
        }

        //else return remaining
        if (i_a < a_loaded)
        {
            return input_a_numbers[i_a++];
        }
        if (i_b < b_loaded)
        {
            return input_b_numbers[i_b++];
        }
        throw("Shouldnt not happen");
    }
};

/**
 * Take two files and merge sort into out_file
 */
void merge(int file_a_index, int file_b_index, int out_file, int32_t buffer_size) {
    //int64_t buffer_size = bytes / 8;
    int res_size = buffer_size / 2;
    int32_t *res_numbers = new int32_t[res_size];

    flib_open(file_a_index, READ);
    flib_open(file_b_index, READ);
    flib_open(out_file, WRITE);

    int i_res = 0;
    Reader rd(file_a_index, file_b_index, res_size);
    do {
        //TODO: check last el of arr a vs last el of arr b - dont need to check each index if last < first
        if(i_res == res_size - 1){
            flib_write(out_file, res_numbers, i_res);
            i_res = 0;
        }
        try {
             res_numbers[i_res] = rd.getNextMin();
        } catch (char const *e) {
            //IDK why this happens -\_(ツ) _/-
            continue;
        }
        i_res++;
    }while(!rd.end_a || !rd.end_b);
    flib_write(out_file, res_numbers, i_res);

    if(rd.end_a && rd.end_b){
        i_res = 0;
        while (rd.i_a < rd.a_loaded) {
            res_numbers[i_res] = rd.input_a_numbers[rd.i_a];
            i_res++;
            rd.i_a++;
        }
        while (rd.i_b < rd.b_loaded) {
            res_numbers[i_res] = rd.input_b_numbers[rd.i_b];
            i_res++;
            rd.i_b++;
        }
        qsort(res_numbers, i_res, sizeof(int32_t), cmp);
        flib_write(out_file, res_numbers, i_res);
    }

    //clear memory
    flib_close(file_a_index);
    flib_close(file_b_index);
    flib_close(out_file);
    delete [] res_numbers;
}

void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes ){
    flib_open(in_file, READ);
    int file_handling_size = 4*12;
    int64_t buffer_size = (bytes-file_handling_size)/8;
    int32_t *input_numbers = new int32_t[buffer_size];
    int number_loaded = 0;
    int file_index = 2;

    //split input file into multiple sorted files
    do{
        number_loaded = flib_read(in_file, input_numbers, buffer_size);
        if(number_loaded == 0){
            break;
        }
        //sort loaded
        qsort(input_numbers, number_loaded, sizeof(int32_t), cmp);

        flib_open(file_index, WRITE);
        flib_write(file_index, input_numbers, number_loaded);
        flib_close(file_index);
        file_index++;
    }while (number_loaded > 0);

    int max_file_index = file_index;
    file_index = 2;

    //if only 1 file - just write
    if(max_file_index == 3){
        flib_open(out_file, WRITE);
        flib_open(2, READ);
        number_loaded = flib_read(2, input_numbers, buffer_size);
        flib_write(out_file, input_numbers, number_loaded);
        flib_close(2);
        flib_close(out_file);
        flib_close(in_file);
        delete [] input_numbers;
        return;
    }

    delete [] input_numbers;

    //merging neighbouring files
    for (int i = file_index; i < max_file_index-1; i+=2) {
        if(i == max_file_index - 2){
            merge(i, i+1, out_file, buffer_size);
            flib_remove(i);
            flib_remove(i+1);
            break;
        }else{
            merge(i, i+1, max_file_index, buffer_size);
            flib_remove(i);
            flib_remove(i+1);
            max_file_index++;
        }
    }
    flib_close(in_file);
}


#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
    /* srand(time(NULL)); */
#define STEP 100ll
    int val[STEP];
    for(int i=0; i<size; i+=STEP){
        for(int j=0; j<STEP && i+j < size; ++j){
            val[j]=-1000 + (rand()%(2*1000+1));
            total_sum_mod += val[j];
        }
        flib_write(output, val, (STEP < size-i) ? STEP : size-i);
    }
    flib_close(output);
}

void tarant_allegra ( int in_file, int out_file, int bytes );

void check_result ( int out_file, int SIZE ){
    flib_open(out_file, READ);
    int q[30], loaded, last=-(1<<30), total=0;
    uint64_t current_sum_mod=0;
    while(loaded = flib_read(out_file, q, 30), loaded != 0){
        total += loaded;
        for(int i=0; i<loaded; ++i){
            if(last > q[i]){
                printf("the result file contains numbers %d and %d on position %d in the wrong order!\n", last, q[i], i-1);
                exit(1);
            }
            last=q[i];
            current_sum_mod += q[i];
        }
    }
    if(total != SIZE){
        printf("the output contains %d but the input had %d numbers\n", total, SIZE); exit(1);
    }
    if(current_sum_mod != total_sum_mod){
        printf("the output numbers are not the same as the input numbers\n");
        exit(1);
    }
    flib_close(out_file);
}

int main(int argc, char **argv){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 510;

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, 500);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif __PROGTEST__
