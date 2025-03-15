//
// Created by xc on 2025/3/14.
//

#ifndef REQUEST_H
#define REQUEST_H
#include "Bucket.h"
class Request {
public:
    int object_id;
    int prev_id;
    bool is_done;

    struct lifeNode *lifeNode;//指向生命周期队列中的元素
    struct listNode *listNode;//指向悬挂列表中的元素

public:
    // Request(int object_id, int prev_id, bool is_done);

};

#endif //REQUEST_H
