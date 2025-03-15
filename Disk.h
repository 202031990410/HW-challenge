//
// Created by xc on 2025/3/14.
//

#ifndef DISK_H
#define DISK_H
#define MAX_DISK_SIZE (16384 + 1)//硬盘容量上限



#include "Bucket.h"

using namespace std;


class Disk {//磁盘
public:

    int capacity;//每块磁盘的容量
    byte data[MAX_DISK_SIZE];//磁盘位图
    vector<int> labels;//本磁盘分配的标签序号

    Disk(int capacity);

    /**
     * 初始化，给磁盘划分标签块
     * @param label_num 该磁盘的分配标签数量
     * @param label_id 标签序号列表
     * @param Object_num 每个标签的对象数
     */
    void initial(int label_num,int *label_id, int *Object_num);//

    /**
     * 生成一个磁盘阵列
     * @param disk_num 磁盘的块数，即N
     * @param capacity 每块磁盘的容量，即V
     * @return
     */
    static vector<Disk> Diskset(int disk_num,int capacity);


};


#endif //DISK_H
