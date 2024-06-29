/*
    Experiemnt

    Hashing function. Here it converts a string into a unsigned integer,
    the modulus of which over an integer N can be used as an index in
    an array of N elements.

    TODO Make this a collection of hashing functions.
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    djb2 by Dan Bernstein.
    Performance is bad!!    
*/
unsigned long hash_Dan_Bernstein(unsigned char* str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static unsigned long sdbm(unsigned char* str)
{
    unsigned long hash = 0;
    int c;
    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

/* FNV, MurmurOAAT, Jenkins, and CRC have much better performance and less collision. */
uint32_t FNV(const void* key, int len, uint32_t h)
{
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    h ^= 2166136261UL;
    const uint8_t* data = (const uint8_t*)key;
    for(int i = 0; i < len; i++)
    {
        h ^= data[i];
        h *= 16777619;
    }
    return h;
}

uint32_t MurmurOAAT_32(const char* str, uint32_t h)
{
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

uint32_t Jenkins_one_at_a_time_hash(const char *str, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/* Least collision but time consuming. */
uint32_t crc32b(const uint8_t *str) {
    // Source: https://stackoverflow.com/a/21001712
    unsigned int byte, crc, mask;
    int i = 0, j;
    crc = 0xFFFFFFFF;
    while (str[i] != 0) {
        byte = str[i];
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--) {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

int main()
{
    size_t CAPACITY = 2048;
    unsigned char* test_list[] = {
        "a83nh", "MN^3m9", "123286", "12865", "Lok_+23&",
        "@uUy&^om___=-", "0992019019", "++==-0#@", "02nGt12865", "____Lok_+23&",
        "abc", "abC", "aBc", "Abc", "aBC", "AbC", "ABc", "ABC"
    };
    int len;
    len =  sizeof(test_list) / sizeof(test_list[0]);

    printf(" *** Dan Bernstein algorithm*** \n");
    for (int i = 0; i < len; i++) {
        unsigned long result;
        result = hash_Dan_Bernstein(test_list[i]);
        printf(" For string %s, the hashed number is %ld, modulus is %d\n", test_list[i], result, result % CAPACITY);
    }

    printf(" *** sdbm algorithm*** \n");
    for (int i = 0; i < len; i++) {
        unsigned long result;
        result = sdbm(test_list[i]);
        printf(" For string %s, the hashed number is %ld, modulue is %d\n", test_list[i], result, result % CAPACITY);
    }

    printf(" *** Murmur algorithm*** \n");
    for (int i = 0; i < len; i++) {
        unsigned long result;
        result = MurmurOAAT_32(test_list[i], 1);    /* What does the last integer do? */
        printf(" For string %s, the hashed number is %ld, modulus is %d\n", test_list[i], result, result % CAPACITY);
    }
}