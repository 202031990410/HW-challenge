//
// Created by xc on 2025/3/15.
//
#define FRE_PER_SLICING (1800)//时间片按1800划分
#include <stdio.h>
#include <vector>

using namespace std;

int T, M, N, V, G; //T:时间片T+105   M：对象标签数      N:硬盘个数     V：每个硬盘存储单元个数     G：每个磁头每个时间片最多消耗的令牌数


void initail(vector<int> write_num) {

}

int timestamp_action(){
    int timestamp;
    scanf("%*s%d", &timestamp);
    printf("TIMESTAMP %d\n", timestamp);

    fflush(stdout);

    return timestamp;
}

void delete_action();

void write_action();

void read_action();

int main() {

    //全局预处理阶段:
    scanf("%d%d%d%d%d", &T, &M, &N, &V, &G);
    int lie = (T - 1) / FRE_PER_SLICING + 1;//预处理阶段的列数

    //删：
    vector<vector<int>> delete_num(M, std::vector<int>(N));
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= lie; j++) {//时间片编号按1800切分，这些信息暂时未利用
            scanf("%*d");
        }
    }

    //写：
    vector<int> write_num(M);//每种标签写入的对象数
    for (int i = 1; i <= M; i++) {
        write_num[i] = 0;
        int temp = 0;
        for (int j = 1; j <= lie; j++) {
            scanf("%*d",temp);
            write_num[i] += temp;
        }
    }

    //读：
    for (int i = 1; i <= M; i++) {
        for (int j = 1; j <= lie; j++) {
            scanf("%*d");
        }
    }

    printf("OK\n");
    fflush(stdout);

    //人机交互阶段：
    for (int t = 1; t <= T + EXTRA_TIME; t++) {
        int timestamp = timestamp_action();
        delete_action();
        write_action();
        read_action();
    }

    clean();

    return 0;
}
