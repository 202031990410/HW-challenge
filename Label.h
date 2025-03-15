//
// Created by xc on 2025/3/15.
//

#ifndef LABEL_H
#define LABEL_H
#include <vector>
#include "BuckeSet.h"


using namespace std;

struct debris {//删除操作产生的空闲碎片
    int distance;//相对于标签块起始位置的相对距离
    int disk_id;//碎片所在的磁盘编号
    int init_local;//碎片在磁盘上的起始位置
    int size;//碎片大小
    struct debris* next;
};
/*
 *以标签块为单位管理删写查，标签操作磁盘进行删写，操作集合来管理查
 */
class Label {
public:
    int s_local[3],e_local[3];//该标签块三个副本在磁盘上的起始位置，结束位置
    int end[3];//实际元素存储在标签块上的最后一个位置-------若分片，end属性对应的磁盘id，是最后一个分片所在的磁盘id，disks_sharding列表的最后一个元素可获取
    int disk_id[3];//标签三个副本所在的磁盘编号
    bool is_sharding;//是否分片,受磁盘容量限制，label的某个副本可能需要分片散布在不同磁盘上，label应当将分片封装起来，所有操作需要对用户隐藏分片细节

    struct debris* debris;//删除操作产生的空闲碎片，可用于重新write--链表
    vector<BuckeSet> bucke_sets;//集合--只管理命中的读请求    标签块分片涉及到多磁盘，所以集合应当只包含第一个片的桶，其余分片不进行划分桶也不涉及集合


    /*
     * Label需要在每次操作前判断是否被分片   封装为私有属性，不暴露在用户面前
     */
private:
    int num_shards[3];//各个副本的分片数（不包含第一个分片，即不包含disk_id[3]对应的磁盘）
    // int *disks_shard[3];//分片的磁盘id链表
    vector<int> disks_sharding[3];//分片的磁盘id列表（不包含第一个分片，即不包含disk_id[3]对应的磁盘）
    //int end_of_disks[3];//end属性对应的磁盘id，一般为最后一个分片所在的磁盘id
    vector<int> s_local_hsarding[3];//分片在对应磁盘上的起始位置

public:
    //提供以下函数
    //1.构造器含数

    //将标签块划分桶

    //.删除对象

    //.写入对象

    //....


};



#endif //LABEL_H
