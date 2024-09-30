//
// Created by kapur on 30/09/2024.
//

#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

struct CacheBlock {
    bool valid;
    unsigned long tag;
    int lastAccessTime;

    CacheBlock() : valid(false), tag(0), lastAccessTime(0) {}
};

class CacheSimulator {
public:
    int cacheSize;
    int blockSize;
    int numSets;
    int associativity;
    int cacheHits;
    int cacheMisses;
    std::vector<std::vector<CacheBlock>> cache;
    int currentTime;
    std::string replacementPolicy;
    int indexBits;
    int blockOffsetBits;

    CacheSimulator(int cacheSize, int blockSize, int associativity, const std::string& replacementPolicy);
    void accessMemory(unsigned long address);
    void replaceBlock(int index, unsigned long tag);
    unsigned long getTag(unsigned long address, int indexBits, int blockOffsetBits);
    unsigned long getIndex(unsigned long address, int blockOffsetBits);
    void printStatistics();
};

CacheSimulator::CacheSimulator(int cacheSize, int blockSize, int associativity, const std::string& replacementPolicy)
        : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity),
          cacheHits(0), cacheMisses(0), currentTime(0), replacementPolicy(replacementPolicy) {

    numSets = cacheSize / (blockSize * associativity);
    cache.resize(numSets, std::vector<CacheBlock>(associativity));
    indexBits = log2(numSets);
    blockOffsetBits = log2(blockSize);
}

unsigned long CacheSimulator::getTag(unsigned long address, int indexBits, int blockOffsetBits) {
    return address >> (indexBits + blockOffsetBits);
}

unsigned long CacheSimulator::getIndex(unsigned long address, int blockOffsetBits) {
    return (address >> blockOffsetBits) & ((1 << indexBits) - 1);
}

void CacheSimulator::accessMemory(unsigned long address) {
    int index = getIndex(address, blockOffsetBits);
    unsigned long tag = getTag(address, indexBits, blockOffsetBits);

    bool hit = false;
    for (int i = 0; i < associativity; ++i) {
        if (cache[index][i].valid && cache[index][i].tag == tag) {
            hit = true;
            cache[index][i].lastAccessTime = currentTime++;
            cacheHits++;
            break;
        }
    }

    if (!hit) {
        cacheMisses++;
        replaceBlock(index, tag);
    }
}

void CacheSimulator::replaceBlock(int index, unsigned long tag) {
    int replaceIndex = 0;

    if (replacementPolicy == "LRU") {
        int oldestTime = INT_MAX;
        for (int i = 0; i < associativity; ++i) {
            if (cache[index][i].lastAccessTime < oldestTime) {
                oldestTime = cache[index][i].lastAccessTime;
                replaceIndex = i;
            }
        }
    }

    cache[index][replaceIndex].tag = tag;
    cache[index][replaceIndex].valid = true;
    cache[index][replaceIndex].lastAccessTime = currentTime++;
}

void CacheSimulator::printStatistics() {
    std::cout << "Cache Hits: " << cacheHits << std::endl;
    std::cout << "Cache Misses: " << cacheMisses << std::endl;
    std::cout << "Hit Rate: " << (double)cacheHits / (cacheHits + cacheMisses) << std::endl;
}
