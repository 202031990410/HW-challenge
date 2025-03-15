//
// Created by xc on 2025/3/14.
//

#ifndef BUCKET_H
#define BUCKET_H
#include "queue"
#include "Request.h"
using namespace std;

struct lifeNode {//生命周期队列元素---每个桶维护一个生命周期队列，维护被挂起的请求的寿命
    int request_id;//对应的请求对象id
    int birth;//请求对应的时间片
    //lifeNode(int request_id, int birth);//构造函数

};
struct listNode {//悬挂列表元素---每个桶维护一个悬挂链表，存放被挂起的请求
    int request_id;//对应的请求对象id
    int local;//请求对象的存储位置
    struct listNode *next = nullptr;
    struct listNode *prev = nullptr;

    //listNode(int request_id,int local, struct listNode *next, struct listNode *prev);//构造函数
};

class Bucket {
    public:
        int weight_life_evaluate = 0;//加权寿命指标：应与每个元素的寿命及距离有关
        int shoot_count = 0;//命中计数
        double dis_degree;//离散程度
        //double wei_time,wei_count,wei_length,wei_dis;//权重
        double score = 0;//得分

        int head;
        int tail;
        int initial_local;

        bool is_head;//是否是集合的第一个桶
        Bucket *next_in_set = nullptr;//集合

        queue <struct lifeNode> life_queue;//生命周期队列
        struct listNode *head_node = nullptr;//悬挂列表_将没有响应的请求挂起

    public:

        Bucket(int initial_local);

        void insert(Request request, int birth);
};

#endif //BUCKET_H
