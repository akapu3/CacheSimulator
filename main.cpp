
#include "CacheSimulator.cpp"
#include <iostream>

int main() {
    int cacheSize = 1024; // Size in bytes
    int blockSize = 64; // Size of each block in bytes
    int associativity = 4; // Set associativity
    std::string replacementPolicy = "LRU"; // Replacement policy (LRU or FIFO)

    CacheSimulator simulator(cacheSize, blockSize, associativity, replacementPolicy);

    // Simulate memory accesses (addresses can be random or a specific pattern)
    unsigned long memoryAccesses[] = { 0x1A2B, 0x3C4D, 0x1A2B, 0x4E5F, 0x5F6E, 0x1A2B };
    int numAccesses = sizeof(memoryAccesses) / sizeof(memoryAccesses[0]);

    for (int i = 0; i < numAccesses; ++i) {
        simulator.accessMemory(memoryAccesses[i]);
    }

    // Print cache performance statistics
    simulator.printStatistics();

    return 0;
}
