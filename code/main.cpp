#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

#define MAX 0x7fffffff //定义无穷大

//交通系统数据结构设置
int N; //地点数量
int M; //车辆数量

struct Nomove{
    int i;
    int j;
    int t;
}; //记录已发生过替换的道路信息

struct Car{
    int start; //出发地
    int end; //目的地
    int time; //行驶时间
    stack<int> road; //存放车辆行驶路径的栈 ---> 由于在path中车辆行驶路径是逆序遍历的，所以使用栈存放，便于后续输出
    vector<Nomove> no_move;//记录车辆是否在该路径该时间点已经发生过替换
}; //单辆车信息
Car car_num[20]; //存放所有车辆的数组 编号从0开始

struct road { //道路信息
    int f_t[400] = {0}; //每个时间点的流量，预测最多有400个时间点, 初始化为0
    int f; //道路流量
    int l; //道路长度
    stack<int> car_t[400]; //每个时间点道路上的车辆
};
road roads[20][20]; //存放所有道路的数组 编号从0开始

struct r_overf { //道路拥堵信息
    int i; //道路编号
    int j;
    int t; //时间点
    int number; //超载车辆数目
};
stack<r_overf> overf; //存放道路拥堵信息的栈



//交通系统初始化
void init(){
    for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
            roads[i][j].l = 0;
            roads[i][j].f = 0;
        }
    }
    for(int i=0; i<20; i++){
        car_num[i].start = 0;
        car_num[i].end = 0;
        car_num[i].time = 0; //汽车未出发，行驶时间初始化为0
    }
};


//Dijkstra算法求最短路径 --->  由于车辆行驶速度为1，所以最短路径即为最短时间
void Dijkstra_recordf(int start, int end, int car){ //基础功能，只寻找最短路径，并更新流量信息

//Dijkstra参数 
//由于对每辆车进行Dijkstra计算时都需要使用，所以直接定义在函数内。函数调用结束后便于直接释放
    int dist[20]; //存放起点到各点的最短距离
    int path[20]; //存放最短路径
    bool S[20]; //标记是否已经找到最短路径

//初始化Dijkstra参数
    for(int i=0; i<N; i++){
        dist[i] = roads[start][i].l;
        S[i] = false;
        if(dist[i] == 0){
            dist[i] = MAX; //若两点之间无直接道路，则距离为无穷大
            path[i] = -1;
        }else{
            path[i] = start;
        }
    }
    //起点初始化
    dist[start] = 0;
    S[start] = true;

//主循环
    for(int i=0; i<N; i++){ //循环N次，每次找到一个最短路径，从而对N个地点都可以找到最短路径
        int min = MAX; //初始化最近距离
        int u = -1;
        //找到距离起点最近的、未找到最短路径的点
        for(int j=0; j<N; j++){
            if(!S[j] && dist[j] < min){
                u = j;
                min = dist[j];
            }
        }

        //在该点基础上更新dist和path数组
        S[u] = true;
        for(int j=0; j<N; j++){
            if(!S[j] && roads[u][j].l != 0){
                if(dist[u] + roads[u][j].l < dist[j]){
                    dist[j] = dist[u] + roads[u][j].l;
                    path[j] = u;
                }
            }
        }
    }

//根据path数组，逆序存放路径，同时更新道路信息
    int i = end;
    int T = dist[end];
    while(i != start){
        T = dist[i];
        car_num[car].road.push(i);
        for(int t=T; t>dist[path[i]]; t--){
            roads[i][path[i]].f_t[t]++; //更新道路流量信息
            roads[path[i]][i].f_t[t]++; 
            roads[i][path[i]].car_t[t].push(car); //更新道路上的车辆信息
            roads[path[i]][i].car_t[t].push(car); 
        }
        i = path[i];
    }
    car_num[car].road.push(start);

//更新车辆行驶时间
    car_num[car].time = dist[end];

}


void Dijkstra(int start, int end, int car, Car car_temp[20]) {

//Dijkstra参数 
//由于对每辆车进行Dijkstra计算时都需要使用，所以直接定义在函数内。函数调用结束后便于直接释放
    int dist[20]; //存放起点到各点的最短距离
    int path[20]; //存放最短路径
    bool S[20]; //标记是否已经找到最短路径

//初始化Dijkstra参数
    for(int i=0; i<N; i++){
        dist[i] = roads[start][i].l;
        S[i] = false;
        if(dist[i] == 0){
            dist[i] = MAX; //若两点之间无直接道路，则距离为无穷大
            path[i] = -1;
        }else{
            path[i] = start;
        }
    }
    //起点初始化
    dist[start] = 0;
    S[start] = true;

//主循环
    for(int i=0; i<N; i++){ //循环N次，每次找到一个最短路径，从而对N个地点都可以找到最短路径
        int min = MAX; //初始化最近距离
        int u = -1;
        //找到距离起点最近的、未找到最短路径的点
        for(int j=0; j<N; j++){
            if(!S[j] && dist[j] < min){
                u = j;
                min = dist[j];
            }
        }
        //在该点基础上更新dist和path数组
        S[u] = true;
        for(int j=0; j<N; j++){
            if(!S[j] && roads[u][j].l != 0){
                if(dist[u] + roads[u][j].l < dist[j]){
                    dist[j] = dist[u] + roads[u][j].l;
                    path[j] = u;
                }
            }
        }
    }

//根据path数组，逆序存放路径，同时更新道路信息
    int i = end;
    while(i != start){
        car_temp[car].road.push(i);
        i = path[i];
    }
    car_temp[car].road.push(start);

//更新车辆行驶时间
    car_temp[car].time = dist[end];

}


//检查道路超载情况
void check_f(int T){
    for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
            for(int t=0; t<T; t++){
                if(roads[i][j].f_t[t] > roads[i][j].f){
                    r_overf temp;
                    temp.i = i;
                    temp.j = j;
                    temp.t = t;
                    temp.number = roads[i][j].f_t[t] - roads[i][j].f;
                    overf.push(temp);
                    return; //最先发现的拥堵路径
                } //记录超载信息
            }
        }
    }
}


//计算最长行驶时间
int time_end(){ 
    int time = 0;
    for(int i=0; i<M; i++){
        time = (time > car_num[i].time) ? time : car_num[i].time; 
    }
    return time;
}

void sort_car(Car car_temp[20], vector<int> re_car, stack<int>& car){
    // 使用sort 对 re_car 进行排序
    sort(re_car.begin(), re_car.end(), [&car_temp](int a, int b) {
        return car_temp[a].time < car_temp[b].time;
    });

    // 将排序后的车辆依次压入栈中
    for (int i = re_car.size() - 1; i >= 0; --i) {
        car.push(re_car[i]);
    }
} //用栈存放超载车辆，按照行驶时间排序，栈顶是最少耗时的车辆


void compare(vector<int>& fresh_car, r_overf cur, Car car_temp[20], vector<int> re_car){
    int m = cur.number;
    stack<int> temp_car;
    sort_car(car_temp, re_car, temp_car);
    while(m!=0){
        int car = temp_car.top();
        temp_car.pop();
        fresh_car.push_back(car);
        m--;
    }
} //比较超载车辆的行驶时间，选择最少耗时的车辆


void remove_stack(road roads[20][20], int m,int n, int t, int car){
    stack<int> temp;
    while(!roads[m][n].car_t[t].empty()){
        int temp_car = roads[m][n].car_t[t].top();
        if(temp_car != car){
            temp.push(temp_car);
        }
        roads[m][n].car_t[t].pop();
    }
    while(!temp.empty()){
        roads[m][n].car_t[t].push(temp.top());
        temp.pop();
    }
    while(!roads[n][m].car_t[t].empty()){
        int temp_car = roads[n][m].car_t[t].top();
        if(temp_car != car){
            temp.push(temp_car);
        }
        roads[n][m].car_t[t].pop();
    }
    while(!temp.empty()){
        roads[n][m].car_t[t].push(temp.top());
        temp.pop();
    }
} //删除道路上的车辆信息



//重新规划超载路段
void re_plan(){
	while(!overf.empty()){
		r_overf cur = overf.top(); 
        overf.pop();
		vector<int> re_car; //路段上的所有车辆
		Car car_temp[20]; //暂时存放需要重规划的车辆的道路信息
        for(int i=0; i<20; i++){
        car_temp[i].start = 0;
        car_temp[i].end = 0;
        car_temp[i].time = 0; //汽车未出发，行驶时间初始化为0
        } //初始化car_temp
		while(!roads[cur.i][cur.j].car_t[cur.t].empty()){
			re_car.push_back(roads[cur.i][cur.j].car_t[cur.t].top());
            roads[cur.i][cur.j].car_t[cur.t].pop();
		}
        for(std::vector<int>::size_type i=0; i<re_car.size(); i++){
            roads[cur.i][cur.j].car_t[cur.t].push(re_car[i]);
        }
        int L = roads[cur.i][cur.j].l;
        roads[cur.i][cur.j].l = 0; //将道路流量置为0 ---> 重新规划时删除该路
        roads[cur.j][cur.i].l = 0;
        for(std::vector<int>::size_type i=0; i<re_car.size(); i++){
            bool flag = false;
            for(std::vector<int>::size_type j=0; j<car_num[re_car[i]].no_move.size(); j++){
                if(car_num[re_car[i]].no_move[j].i == cur.i && car_num[re_car[i]].no_move[j].j == cur.j && car_num[re_car[i]].no_move[j].t == cur.t){
                    car_temp[re_car[i]].time = MAX;
                    flag = true;
                    break;
                }
            }
            if(flag == false){
                Dijkstra(car_num[re_car[i]].start-1, car_num[re_car[i]].end-1, re_car[i], car_temp);
            }
        }
        roads[cur.i][cur.j].l = L; //恢复道路长度
        roads[cur.j][cur.i].l = L;

        vector<int> fresh_car; //存放需要重新规划的车辆
        compare(fresh_car, cur, car_temp, re_car);

        //更新规划后的路线和车辆信息
        for(std::vector<int>::size_type i=0; i<fresh_car.size(); i++){
            Nomove re_road = {cur.i, cur.j, cur.t};
            car_num[fresh_car[i]].no_move.push_back(re_road);
            int T = 0;
            while(car_num[fresh_car[i]].road.top() != car_num[fresh_car[i]].end-1){
                int m = car_num[fresh_car[i]].road.top();
                car_num[fresh_car[i]].road.pop();
                int n = car_num[fresh_car[i]].road.top();
                for(int t=T; t<=T+roads[m][n].l; t++){
                    roads[m][n].f_t[t]--;
                    roads[n][m].f_t[t]--;
                    remove_stack(roads, m, n, t, fresh_car[i]);
                }
                T = T + roads[m][n].l;
            } //删除原有道路上的车辆信息
            car_num[fresh_car[i]].road.pop();

            car_num[fresh_car[i]].time = car_temp[fresh_car[i]].time;
            stack<int> temp_road;
            T = 0;
            while(car_temp[fresh_car[i]].road.top() != car_num[fresh_car[i]].end-1){
                int m = car_temp[fresh_car[i]].road.top();
                temp_road.push(m);
                car_temp[fresh_car[i]].road.pop();
                int n = car_temp[fresh_car[i]].road.top();
                for(int t=T; t<T+roads[m][n].l; t++){
                    roads[m][n].f_t[t]++;
                    roads[n][m].f_t[t]++;
                    roads[m][n].car_t[t].push(fresh_car[i]);
                    roads[n][m].car_t[t].push(fresh_car[i]);
                }
                T = T + roads[m][n].l;
            } //更新新的道路上的车辆信息
            int m = car_temp[fresh_car[i]].road.top();
            temp_road.push(m);
            car_temp[fresh_car[i]].road.pop();

            while(!temp_road.empty()){
                car_num[fresh_car[i]].road.push(temp_road.top());
                temp_road.pop();
            } //更新新的道路信息
        }
	}
}



//运行主函数
int main(){
    init(); //初始化交通系统

    //读入交通信息
    cin >> N >> M;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cin >> roads[i][j].l;
        }
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            cin >> roads[i][j].f;
        }
    }
    for(int i=0; i<M; i++){
        cin >> car_num[i].start >> car_num[i].end;
    }

    //计算每辆车的最短路径--->基础功能
    for(int i=0; i<M; i++){
        Dijkstra_recordf(car_num[i].start-1, car_num[i].end-1, i);
    }

    //反复规划直至没有道路超载
    int T = time_end();
    check_f(T);
    int i = 1;
    while (!overf.empty()){
	    re_plan();
	    T = time_end();
	    check_f(T);
        i++;
    }

    //计算总行驶时间
    int sum_time = 0;
    for(int i=0; i<M; i++){
        sum_time += car_num[i].time;
    }

    //输出结果
    for(int i=0; i<M; i++){
        cout << car_num[i].road.top()+1; //确保输出编号与地点编号一致
        car_num[i].road.pop(); //保证目的地后不输出空格
        while(!car_num[i].road.empty()){
            cout << " ";
            cout << car_num[i].road.top()+1; //确保输出编号与地点编号一致
            car_num[i].road.pop();
        }
        cout << endl;
    }
    cout << sum_time << endl;

    return 0;
}
