#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<string.h>
#include<ctype.h>

typedef char byte;

struct cache {
    byte address; // This is the address in memory.
    byte value; // This is the value stored in cached memory.
    // State for you to implement MESI protocol.
    byte state;
};

struct decoded_inst {
    int type; // 0 is RD, 1 is WR
    byte address;
    byte value; // Only used for WR 
};

typedef struct cache cache;
typedef struct decoded_inst decoded;


/*
 * This is a very basic C cache simulator.
 * The input files for each "Core" must be named core_1.txt, core_2.txt, core_3.txt ... core_n.txt
 * Input files consist of the following instructions:
 * - RD <address>
 * - WR <address> <val>
 */

byte * memory;

// Decode instruction lines
decoded decode_inst_line(char * buffer){
    decoded inst;
    char inst_type[2];
    sscanf(buffer, "%s", inst_type);
    if(!strcmp(inst_type, "RD")){
        inst.type = 0;
        int addr = 0;
        sscanf(buffer, "%s %d", inst_type, &addr);
        inst.value = -1;
        inst.address = addr;
    } else if(!strcmp(inst_type, "WR")){
        inst.type = 1;
        int addr = 0;
        int val = 0;
        sscanf(buffer, "%s %d %d", inst_type, &addr, &val);
        inst.address = addr;
        inst.value = val;
    }
    return inst;
}

// Helper function to print the cachelines
void print_cachelines(cache * c, int cache_size){
    for(int i = 0; i < cache_size; i++){
        cache cacheline = *(c+i);
        printf("Address: %d, State: %d, Value: %d\n", cacheline.address, cacheline.state, cacheline.value);
    }
}


// This function implements the mock CPU loop that reads and writes data.
void cpu_loop(int num_threads){
    // Initialize a CPU level cache that holds about 2 bytes of data.
    int cache_size = 2;
    cache * c = (cache *) malloc(sizeof(cache) * cache_size);
    
    // Read Input file
    FILE * inst_file = fopen("input_0.txt", "r");
    char inst_line[20];
    // Decode instructions and execute them.
    while (fgets(inst_line, sizeof(inst_line), inst_file)){
        decoded inst = decode_inst_line(inst_line);
        /*
         * Cache Replacement Algorithm
         */
        int hash = inst.address%cache_size;
        cache cacheline = *(c+hash);
        /*
         * This is where you will implement the coherancy check.
         * For now, we will simply grab the latest data from memory.
         */
        if(cacheline.address != inst.address){
            // Flush current cacheline to memory
            *(memory + cacheline.address) = cacheline.value;
            // Assign new cacheline
            cacheline.address = inst.address;
            cacheline.state = -1;
            // This is where it reads value of the address from memory
            cacheline.value = *(memory + inst.address);
            if(inst.type == 1){
                cacheline.value = inst.value;
            }
            *(c+hash) = cacheline;
        }
        switch(inst.type){
            case 0:
                printf("Reading from address %d: %d\n", cacheline.address, cacheline.value);
                break;
            
            case 1:
                printf("Writing to address %d: %d\n", cacheline.address, cacheline.value);
                break;
        }
    }
    free(c);
}

int main(int c, char * argv[]){
    // Initialize Global memory
    // Let's assume the memory module holds about 24 bytes of data.
    int memory_size = 24;
    memory = (byte *) malloc(sizeof(byte) * memory_size);
    cpu_loop(1);
    free(memory);
}