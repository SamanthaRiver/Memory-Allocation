#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include<time.h>

#define frand() (rand()/(double)RAND_MAX)
#define nrand() (sqrt(-2*log(frand()))*cos(2*M_PI*frand()))


double avg_utilization = 0;
int number_of_examined_holes = 0;
int head = 0;                           //head of the linked list of holes
int current_hole = 0;


void initialize(int n, int d, int v, int** mem, int**blocks){
    srand(time(NULL));
    int max_blocks = n/(d-v);           //why?!
    *mem = (int*)malloc(sizeof(int)*n);
    (*mem)[0] = (*mem)[n-1] = -n+2;
    (*mem)[1] = (*mem)[2] = 0;
    *blocks = (int*)malloc(max_blocks*sizeof(int));
}


int nrand_gen(int d, double v, int n){
    double rv;
    do{
        rv=  nrand() * v + d;           //use rand function here
    }while(rv < 3 || rv > n-4);
    return rv;
}


int first_fit(int* blocks, int* block_count, int* mem, int d, double v, int n){
    if(head == -1)                      //no hole is found!
        return 0;
    int size = nrand_gen(d, v, n);
    int iterator = head, pred, succ, new_size, block_address;

    while(size > -mem[iterator]){
        number_of_examined_holes++;
        iterator = mem[iterator + 2];   //iterator = iterator->next
        if(head == iterator)
            return 0;                   //unsuccessful
    }
    if(abs(size+mem[iterator]) <= 4){   //fill completely
        //request size is almost equal to the hole size
        size = -mem[iterator];          //block fills hole completely
        mem[iterator] = mem[iterator + size + 1] = size;

        //delete the current hole since it is full now
        pred = mem[iterator+1];
        succ = mem[iterator+2];
        if (pred == iterator)           //current hole is the only hole
            head = -1;
        else{
            mem[pred+2] = succ;         //current->prev->next = current->next
            mem[succ+1] = pred;         //current->next->prev = current->prev
        }
        block_address = iterator;
    }else{                              //fill partially
        mem[iterator] += (size+2);

        //adding pos by neg to make it less negative
        new_size = -mem[iterator];
        mem[iterator + new_size + 1] = mem[iterator];

        //block starts at iterator + new_size + 2
        block_address = iterator + new_size + 2;
        mem[block_address] =
        mem[block_address + size + 1] = size;
    }
    blocks[*block_count] = block_address;
    (*block_count)++;
    return 1;                           //successful
}


int next_fit(int* blocks, int* block_count, int* mem, int d, double v, int n){
    if(head == -1)                      //no hole is found!
        return 0;
    int size = nrand_gen(d, v, n);
    int iterator = head, pred, succ, new_size, block_address;

    while(size > -mem[iterator]){
        number_of_examined_holes++;
        iterator = mem[iterator + 2];   //iterator = iterator->next
        if(head == iterator)
            return 0;                   //unsuccessful
    }
    if(abs(size+mem[iterator]) <= 4){   //fill completely
        //request size is almost equal to the hole size
        size = -mem[iterator];          //block fills hole completely
        mem[iterator] = mem[iterator + size + 1] = size;

        //delete the current hole since it is full now
        pred = mem[iterator+1];
        succ = mem[iterator+2];
        if (pred == iterator)           //current hole is the only hole
            head = -1;
        else{
            mem[pred+2] = succ;         //current->prev->next = current->next
            mem[succ+1] = pred;         //current->next->prev = current->prev
        }
        block_address = iterator;
    }else{                              //fill partially
        mem[iterator] += (size+2);

        //adding pos by neg to make it less negative
        new_size = -mem[iterator];
        mem[iterator + new_size + 1] = mem[iterator];

        //block starts at iterator + new_size + 2
        block_address = iterator + new_size + 2;
        mem[block_address] =
        mem[block_address + size + 1] = size;
    }
    blocks[*block_count] = block_address;
    (*block_count)++;
    return 1;                           //successful
}


int best_fit(int* blocks, int* block_count, int* mem, int d, double v, int n){
    if(head == -1)                      //no hole is found!
        return 0;
    int size = nrand_gen(d, v, n);
    int iterator = head, pred, succ, new_size, block_address;

    while(size > -mem[iterator]){
        number_of_examined_holes++;
        iterator = mem[iterator + 2];   //iterator = iterator->next
        if(head == iterator)
            return 0;                   //unsuccessful
    }
    if(abs(size+mem[iterator]) <= 4){   //fill completely
        //request size is almost equal to the hole size
        size = -mem[iterator];          //block fills hole completely
        mem[iterator] = mem[iterator + size + 1] = size;

        //delete the current hole since it is full now
        pred = mem[iterator+1];
        succ = mem[iterator+2];
        if (pred == iterator)           //current hole is the only hole
            head = -1;
        else{
            mem[pred+2] = succ;         //current->prev->next = current->next
            mem[succ+1] = pred;         //current->next->prev = current->prev
        }
        block_address = iterator;
    }else{                              //fill partially
        mem[iterator] += (size+2);

        //adding pos by neg to make it less negative
        new_size = -mem[iterator];
        mem[iterator + new_size + 1] = mem[iterator];

        //block starts at iterator + new_size + 2
        block_address = iterator + new_size + 2;
        mem[block_address] =
        mem[block_address + size + 1] = size;
    }
    blocks[*block_count] = block_address;
    (*block_count)++;
    return 1;                           //successful
}


void release(int* blocks, int* block_count, int* mem){
    if(!*block_count)
        return;
    int to_be_released = rand()%(*block_count);
    printf("releasing block at location (address) %d\n", blocks[to_be_released]);

    //remove an integer at index to_be_released from blocks array...
    blocks[to_be_released] = blocks[(*block_count)-1];
    (*block_count)--;
}


void update_memory_utilization(int* blocks, int block_count, int* mem, int n, int x){
    double utilization = 0;
    for(int i = 0;i < block_count;i++)
        utilization += mem[blocks[i]];
    utilization /= n;
    //printf("%f\t",utilization);
    avg_utilization += utilization/x;
}


int main(int argc, char** argv) {
    int x,n,d;
    double v;
    while(*++argv){
        //./main -x 1000 -n 16000000 -d 1000000 -v 256000
        if(**argv != '-')
            return 1;
        switch((*argv)[1]){
            case 'x':
                x = atoi(*++argv);
                break;
            case 'n':
                n = atoi(*++argv);
                break;
            case 'd':
                d = atoi(*++argv);
                break;
            case 'v':
                v = atof(*++argv);
                break;
            default: //error
                return 1;
        }
    }
    int* mem;                       //the memory
    int* blocks;
    int block_count = 0;
    printf("Running the simulation with x=%d n=%d d=%d v=%.2f\n", x,n,d,v);
    initialize(n, d, v, &mem, &blocks);
    while(x > 0){
        x--;
        while(first_fit(blocks, &block_count, mem, d, v, n));
        update_memory_utilization(blocks, block_count, mem, n, x);
        release(blocks,&block_count,mem);
    }
    printf("avg utilization is %.3f", avg_utilization);
    return 0;
}
