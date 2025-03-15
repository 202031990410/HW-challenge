#include <cstdio>
#include <cassert>
#include <cstdlib>

#define MAX_DISK_NUM (10 + 1)//硬盘数量上限
#define MAX_DISK_SIZE (16384 + 1)//硬盘容量上限
#define MAX_REQUEST_NUM (30000000 + 1)//每次读取操作的对象数量上限
#define MAX_OBJECT_NUM (100000 + 1)//每次删除和写入操作的对象数量上限
#define REP_NUM (3)//数据备份
#define FRE_PER_SLICING (1800)//时间片按1800划分
#define EXTRA_TIME (105)//时间片T+105

typedef struct Request_ {//读取请求
    int object_id;
    int prev_id;//上一次读取请求编号
    bool is_done;
} Request;

typedef struct Object_ {
    int replica[REP_NUM + 1];//所有副本对应的磁盘编号
    int* unit[REP_NUM + 1];//二维数组，指明每个副本所有对象块写入的存储单元编号
    int size;
    int last_request_point;//该对象的 最后一次读取请求编号
    bool is_delete;
} Object;

Request request[MAX_REQUEST_NUM];//读取请求
Object object[MAX_OBJECT_NUM];
//T:时间片T+105   M：对象标签数      N:硬盘个数     V：每个硬盘存储单元个数     G：每个磁头每个时间片最多消耗的令牌数
int T, M, N, V, G;
int disk[MAX_DISK_NUM][MAX_DISK_SIZE];
int disk_point[MAX_DISK_NUM];

void timestamp_action()
{
    int timestamp;
    scanf("%*s%d", &timestamp);
    printf("TIMESTAMP %d\n", timestamp);

    fflush(stdout);
}

void do_object_delete(const int* object_unit, int* disk_unit, int size)
{
    for (int i = 1; i <= size; i++) {
        disk_unit[object_unit[i]] = 0;
    }
}

void delete_action()
{
    int n_delete;//删除的对象个数
    int abort_num = 0;//读取请求中止数
    static int _id[MAX_OBJECT_NUM];//删除的对象编号

    scanf("%d", &n_delete);
    for (int i = 1; i <= n_delete; i++) {
        scanf("%d", &_id[i]);
    }

    for (int i = 1; i <= n_delete; i++) {
        int id = _id[i];
        int current_id = object[id].last_request_point;
        while (current_id != 0) {//把访问该对象的所有请求都访问一边，查看这些请求是否被响应
            if (request[current_id].is_done == false) {
                abort_num++;
            }
            current_id = request[current_id].prev_id;
        }
    }

    printf("%d\n", abort_num);
    for (int i = 1; i <= n_delete; i++) {
        int id = _id[i];
        int current_id = object[id].last_request_point;
        while (current_id != 0) {
            if (request[current_id].is_done == false) {
                printf("%d\n", current_id);
            }
            current_id = request[current_id].prev_id;
        }
        for (int j = 1; j <= REP_NUM; j++) {
            do_object_delete(object[id].unit[j], disk[object[id].replica[j]], object[id].size);
        }
        object[id].is_delete = true;
    }

    fflush(stdout);
}
//参数列表：对象块列表（记录写入的存储单元编号）  当前副本所在的磁盘的存储单元列表  对象大小    对象id
void do_object_write(int* object_unit, int* disk_unit, int size, int object_id)
{
    int current_write_point = 0;
    for (int i = 1; i <= V; i++) {
        if (disk_unit[i] == 0) {//判断存储单元是否空闲
            disk_unit[i] = object_id;//空闲则存储当前对象
            object_unit[++current_write_point] = i;//同一对象的对象块连续存储
            if (current_write_point == size) {
                break;
            }
        }
    }

    assert(current_write_point == size);
}

void write_action()
{
    int n_write;
    scanf("%d", &n_write);
    for (int i = 1; i <= n_write; i++) {
        int id, size;
        scanf("%d%d%*d", &id, &size);
        object[id].last_request_point = 0;
        for (int j = 1; j <= REP_NUM; j++) {
            object[id].replica[j] = (id + j) % N + 1;//副本所在磁盘编号
            object[id].unit[j] = static_cast<int*>(malloc(sizeof(int) * (size + 1)));//副本j的对象块数组
            object[id].size = size;
            object[id].is_delete = false;
            do_object_write(object[id].unit[j], disk[object[id].replica[j]], size, id);//存磁盘的动作
        }
        //完成写入事件之后与控制台的交互
        printf("%d\n", id);
        for (int j = 1; j <= REP_NUM; j++) {
            printf("%d", object[id].replica[j]);
            for (int k = 1; k <= size; k++) {
                printf(" %d", object[id].unit[j][k]);
            }
            printf("\n");
        }
    }

    fflush(stdout);
}

void read_action()
{
    int n_read;
    int request_id, object_id;//
    scanf("%d", &n_read);
    for (int i = 1; i <= n_read; i++) {
        scanf("%d%d", &request_id, &object_id);
        request[request_id].object_id = object_id;
        request[request_id].prev_id = object[object_id].last_request_point;
        object[object_id].last_request_point = request_id;
        request[request_id].is_done = false;
    }

    static int current_request = 0;
    static int current_phase = 0;//局部静态变量​初始化仅一次，但是作用范围是局部的，超出范围不可见
    if (!current_request && n_read > 0) {
        current_request = request_id;//为读取交互事件的最后一个读请求编号。该程序当次时间片前面的读取请求包括后面几个时间片的读取请求全部放弃，
    }
    if (!current_request) {
        for (int i = 1; i <= N; i++) {
            printf("#\n");
        }
        printf("0\n");
    } else {
        current_phase++;//本程序磁头移动只执行jump动作，控制当前时间片执行jump动作还是读动作，两者交替进行
        object_id = request[current_request].object_id;

        //前N行表示磁头的运动输出，第i行白哦是编号为i的磁盘的磁头运动方式
        for (int i = 1; i <= N; i++) {//遍历所有硬盘
            if (i == object[object_id].replica[1]) {//只读第一个副本的数据

                if (current_phase % 2 == 1) {
                    printf("j %d\n", object[object_id].unit[1][current_phase / 2 + 1]);//跳跃，跳越固定消耗G个令牌（每个磁头每个时间片最多消耗G个令牌），且只能子啊每个时间片的开始执行jump动作（即jump消耗的令牌不能借用下一个时间片的）
                } else {
                    printf("r#\n");//读取当前存储单元x
                }
            } else {
                printf("#\n");//其他磁盘不操作
            }
        }

        if (current_phase == object[object_id].size * 2) {//两者相等时，所有的对象块已经读完
            if (object[object_id].is_delete) {
                printf("0\n");
            } else {
                printf("1\n%d\n", current_request);
                request[current_request].is_done = true;
            }
            current_request = 0;//读完上次的读取请求才开始读下次的请求
            current_phase = 0;
        } else {
            printf("0\n");
        }
    }

    fflush(stdout);
}

void clean()
{
    for (auto& obj : object) {
        for (int i = 1; i <= REP_NUM; i++) {
            if (obj.unit[i] == nullptr)
                continue;
            free(obj.unit[i]);
            obj.unit[i] = nullptr;
        }
    }
}

int main()
{
    scanf("%d%d%d%d%d", &T, &M, &N, &V, &G);
    //T:时间片T+105   M：对象标签数      N:硬盘个数     V：每个硬盘存储单元个数     G：每个磁头每个时间片最多消耗的令牌数

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= (T - 1) / FRE_PER_SLICING + 1; j++) {//时间片编号按1800切分，这些信息暂时未利用
            scanf("%*d");
        }
    }

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= (T - 1) / FRE_PER_SLICING + 1; j++) {
            scanf("%*d");
        }
    }

    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= (T - 1) / FRE_PER_SLICING + 1; j++) {
            scanf("%*d");
        }
    }

    printf("OK\n");
    fflush(stdout);

    for (int i = 1; i <= N; i++) {
        disk_point[i] = 1;//标记有多少硬盘
    }

    for (int t = 1; t <= T + EXTRA_TIME; t++) {
        timestamp_action();
        delete_action();
        write_action();
        read_action();
    }
    clean();

    return 0;
}