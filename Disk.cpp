//
// Created by xc on 2025/3/14.
//

#include "Disk.h"

Disk::Disk(int capacity) {
    this->capacity = capacity;
}

vector<Disk> Disk::Diskset(int disk_num, int capacity) {
    vector<Disk> disks;
    for (int i = 0; i < disk_num; i++) {
        Disk disk(capacity);
        disks.push_back(disk);
    }
    return disks;
}
