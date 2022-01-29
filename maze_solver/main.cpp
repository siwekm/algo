#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct coord{
    short x;
    short y;
};
struct lever{
    short pos;
    short* change_vector;
};

//basic vector impl. with help of internet
template <typename T> class vectorClass{
    T* arr;
    int capacity;
    int current;

public:
    vectorClass(){
        arr = new T[1];
        capacity = 1;
        current = 0;
    }

    void push_back(T data){
        if (current == capacity) {
            T* temp = new T[2 * capacity];

            //copy old array
            for (int i = 0; i < capacity; i++) {
                temp[i] = arr[i];
            }

            //delete previous array
            delete[] arr;
            capacity *= 2;
            arr = temp;
        }

        arr[current] = data;
        current++;
    }

    // function to add element at any index
    void push(T data, int index){
        if (index == capacity)
            push_back(data);
        else
            arr[index] = data;
    }

    // function to extract element at any index
    T get(int index){
        // if index is within the range
        if (index < current)
            return arr[index];
        else
            return T{};
    }

    //delete last
    void pop() { current--; }

    int size() { return current; }

    int getcapacity() { return capacity; }

    ~vectorClass(){
        delete[] arr;
    }
    vectorClass& operator=(const vectorClass& o){
        arr = new T[o.capacity];
        capacity = o.capacity;
        current = o.current;
        for (int i = 0; i < capacity; i++) {
            arr[i] = o.arr[i];
        }
        return *this;
    }

    //todo add copy const

    vectorClass(const vectorClass &o) {
        arr = new T[o.capacity];
        capacity = o.capacity;
        current = o.current;
        for (int i = 0; i < capacity; i++) {
            arr[i] = o.arr[i];
        }
    }

};

struct QNode {
    coord data;
    QNode* next;
    QNode(){
        data = {-1, -1};
        next = nullptr;
    };
    QNode(coord d){
        data = d;
        next = nullptr;
    };

};

template <typename T> class Queue {
private:
    QNode *front, *rear;
public:
    Queue(){
        front = rear = nullptr;
    }

    bool isEmpty(){
        if (front == nullptr)
            return true;
        else
            return false;
    }

    T getFront(){
        if (isEmpty()) {
            abort();
        }
        return front->data;
    }

    //add to end
    void enQueue(T x){
        QNode* temp = new QNode(x);

        if (rear == nullptr) {
            front = rear = temp;
            return;
        }

        rear->next = temp;
        rear = temp;
    }

    //remove first
    void deQueue(){
        if (front == nullptr) {
            return;
        }

        QNode* temp = front;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete (temp);
    }
    ~Queue(){
        if(front) {
            while (front->next) {
                auto tmp = front->next;
                delete front;
                front = tmp;
            }
        }
        if(rear){
            delete rear;
        }
    }
};

short* line_to_num(const char* line, int line_size){
    short* num_line;
    num_line = new short[line_size];
    for (int i = 0; i < line_size; ++i) {
        //printf("%d", line[i] - '0');
        num_line[i] = line[i] - '0';
    }
    return num_line;
}

void print_maze(int** maze,int maze_size) {
    printf("\nMaze\n");
    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            printf("%d", maze[i][j]);
        }
        printf("\n");
    }
}
vectorClass<coord> get_neighbours(short** maze,int maze_size, coord pos){
    vectorClass<coord> ngb;

    if((pos.x - 1 >= 0) && maze[pos.x - 1][pos.y] == 0)
        ngb.push_back(coord{short(pos.x - 1), pos.y});
    if((pos.x + 1 < maze_size) && maze[pos.x + 1][pos.y] == 0)
        ngb.push_back(coord{short(pos.x + 1), pos.y});
    if((pos.y - 1 >= 0) && maze[pos.x][pos.y - 1] == 0)
        ngb.push_back(coord{pos.x, short(pos.y - 1)});
    if((pos.y + 1 < maze_size) && maze[pos.x][pos.y + 1] == 0)
        ngb.push_back(coord{pos.x, short(pos.y + 1)});

    return ngb;
}

coord ** solveBFS(short** maze, int maze_size, coord maze_end, coord maze_start) {
    bool **visited = new bool*[maze_size];
    for (int i = 0; i < maze_size; ++i) {
        visited[i] = new bool[maze_size];
    }

    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            visited[i][j] = false;
        }
    }

    Queue<coord> dfs_queue;
    coord **parents = new coord*[maze_size];
    for (int i = 0; i < maze_size; ++i) {
        parents[i] = new coord[maze_size];
    }

    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            parents[i][j] = {-1, -1};
        }
    }
    dfs_queue.enQueue(maze_start);
    visited[maze_start.x][maze_start.y] = true;
    coord dfs_node;

    while(!dfs_queue.isEmpty()){
        dfs_node = dfs_queue.getFront();
        dfs_queue.deQueue();
        if (dfs_node.x == maze_end.x && dfs_node.y == maze_end.y){
            //printf("FIN\n");
            break;
        }
        vectorClass<coord> ngb = get_neighbours(maze, maze_size, dfs_node);
        for(int i =0; i != ngb.size(); i++){
            auto current_ngb = ngb.get(i);
            if(!visited[current_ngb.x][current_ngb.y]){
                visited[current_ngb.x][current_ngb.y] = true;
                dfs_queue.enQueue(current_ngb);
                parents[current_ngb.x][current_ngb.y] = dfs_node;
            }
        }
        //todo del ngb
    }

    //cleanup
    for (int i = 0; i < maze_size; ++i) {
        delete [] visited[i];
    }
    delete [] visited;
    return parents;
}

vectorClass<coord> reconstruct(coord ** parents, coord end_node, coord start_node){
    coord current_path = end_node;
    vectorClass<coord> path;
    while(current_path.x != start_node.x || current_path.y != start_node.y){
        path.push_back(current_path);
        current_path = parents[current_path.x][current_path.y];
    }
    path.push_back(start_node);
    return path;
}

bool contains_lever(const int *used_levers_ind, int num_of_used_levers, int i) {
    for (int j = 0; j < num_of_used_levers; ++j) {
        if(i == used_levers_ind[j]){
            return true;
        }
    }
    return false;
}

int get_levers_path_len(const lever *levers, const int *used_levers_ind, int num_of_used_levers) {
    int levers_path_len = 0;
    int max_lever_path = 0, min_lever_path = 0;
    for (int i = 0; i < num_of_used_levers; ++i) {
        if(levers[used_levers_ind[i]].pos == -1){
            min_lever_path = 1;
            continue;
        }
        if(max_lever_path < levers[used_levers_ind[i]].pos || i == 0){
            max_lever_path = levers[used_levers_ind[i]].pos;
        }
    }
    levers_path_len = max_lever_path + min_lever_path;
    levers_path_len *= 2;
    return levers_path_len;
}

void printLevers(int number_of_levers, const int *used_levers_ind, int num_of_used_levers) {
    for (int i = 0; i < number_of_levers; ++i) {
        if(contains_lever(used_levers_ind, num_of_used_levers, i)){
            printf("1");
        }else{
            printf("0");
        }
    }
    if(number_of_levers > 0){
        printf("\n");
    }
}

//printing y,x!
void printPath(vectorClass<coord> path, lever *levers, int number_of_levers, int used_levers_ind[], int num_of_used_levers){
    int levers_path_len = get_levers_path_len(levers, used_levers_ind, num_of_used_levers);

    printf("%d\n", path.size() - 1 + levers_path_len);
    printLevers(number_of_levers, used_levers_ind, num_of_used_levers);

    for(auto i = path.size() - 1; i > 0; i--){
        printf("[%d;%d],", path.get(i).y + 1, path.get(i).x + 1);
    }
    printf("[%d;%d]\n", path.get(0).y + 1, path.get(0).x + 1);
}

//find better way?
short* int_arr_xor(const short* a, const short* b, int size){
    short* tmp = new short[size];
    for (int i = 0; i < size; ++i) {
        tmp[i] = 0;
    }
    for (int i = 0; i < size; ++i) {
        tmp[i] = a[i]^b[i];
    }
    return tmp;
}

void apply_change_vec_maze(short** maze, short* change_vec, int maze_size){
    for (int i = 0; i < maze_size; ++i) {
        for (int j = 0; j < maze_size; ++j) {
            maze[j][i] ^= change_vec[i];
        }
    }
}

void free_all(short** maze, int maze_size, lever *levers, short* change_vector ){
    delete [] change_vector;
    delete [] levers;
    for (int i = 0; i < maze_size; ++i) {
        delete [] maze[i];
    }
    delete [] maze;
}

void load_levers(int maze_size, int num_levers, char *maze_line, lever *levers) {
    for (int i = 0; i < num_levers; i++) {
        int lever_pos;
        scanf("%d %s", &lever_pos, maze_line);
        levers[i].pos = lever_pos;
        levers[i].change_vector = line_to_num(maze_line, maze_size);
    }
}

void load_maze(int maze_size, short **maze, char *maze_line) {
    for (int i = 0; i < maze_size; ++i) {
        scanf("%s ", maze_line);
        maze[i] = line_to_num(maze_line, maze_size);
    }
}

short *set_change_vector(int maze_size, int num_levers, const lever *levers, short *change_vector,
                         const int *shortest_used_levers_ind, int shortset_num_of_used_levers) {
    for (int j = 0; j < num_levers; ++j) {
        if(contains_lever(shortest_used_levers_ind, shortset_num_of_used_levers, j)){
            change_vector = int_arr_xor(change_vector, levers[j].change_vector, maze_size);
        }
    }
    return change_vector;
}

void print_result(int num_levers, const coord &maze_start, const lever *levers, const int *used_levers_ind,
                  int num_of_used_levers) {
    printf("%d\n", get_levers_path_len(levers, used_levers_ind, num_of_used_levers));
    printLevers(num_levers, used_levers_ind, num_of_used_levers);
    printf("[%d;%d]\n", maze_start.y + 1, maze_start.x + 1);
}

void set_lever_comb(int maze_size, int num_levers, const lever *levers, int *used_levers_ind, int i, short *&change_vector,
               int &num_of_used_levers) {
    for (int j = 0; j < num_levers; ++j) {
        if(i>>j &  1){
            //xor levers here
            auto tmp = int_arr_xor(change_vector, levers[j].change_vector, maze_size);
            delete [] change_vector;
            change_vector = tmp;
            used_levers_ind[num_of_used_levers] = j;
            num_of_used_levers++;
        }
    }
}

int main() {
    int maze_size, num_levers;
    short** maze;
    coord maze_start = {0, 0};

    scanf("%d %d", &maze_size, &num_levers);
    maze = new short*[maze_size];

    char *maze_line = new char[maze_size + 2];
    lever *levers = new lever[num_levers];

    //load levers configurations
    load_levers(maze_size, num_levers, maze_line, levers);
    load_maze(maze_size, maze, maze_line);
    delete [] maze_line;

    int end_x, end_y;
    //load coord from y/x to x/y
    scanf("%d %d", &end_y, &end_x);
    //convert coordinates to start 0/0
    coord maze_end{short(end_x - 1), short(end_y - 1)};

    //for all lever comb.
    short* change_vector = new short[maze_size];

    coord** parents;
    //save shortest
    vectorClass<coord> shortest_path;
    int used_levers_ind[10];
    int num_of_used_levers = 0;
    int shortest_used_levers_ind[10];
    int shortset_num_of_used_levers = 0;

    //test all lever combiantions
    for (int i = 0; i < pow(2, num_levers); i++) {
        for (int i = 0; i < maze_size; ++i) {
            change_vector[i] = 0;
        }

        num_of_used_levers = 0;
        set_lever_comb(maze_size, num_levers, levers, used_levers_ind, i, change_vector, num_of_used_levers);
        apply_change_vec_maze(maze, change_vector, maze_size);

        //run
        //check if start == end
        if(maze[maze_start.x][maze_start.y] == 0 && maze_end.x == maze_start.x && maze_start.y == maze_end.y){
            print_result(num_levers, maze_start, levers, used_levers_ind, num_of_used_levers);
            return 1;
        }

        parents = solveBFS(maze, maze_size, maze_end, maze_start);
        if(maze[maze_start.x][maze_start.y] == 1 || (parents[maze_end.x][maze_end.y].x == -1 && parents[maze_end.x][maze_end.y].y == -1)){
            //restore to default
            for (int k = 0; k < maze_size; ++k) {
                delete [] parents[k];
            }
            delete [] parents;
            apply_change_vec_maze(maze, change_vector, maze_size);
            continue;
        }
        vectorClass path = reconstruct(parents, maze_end, maze_start);
        if(path.size() + get_levers_path_len(levers, used_levers_ind, num_of_used_levers) < shortest_path.size() + get_levers_path_len(levers, shortest_used_levers_ind, shortset_num_of_used_levers)
           || shortest_path.size() == 0){
            shortest_path = path;

            //copy levers
            shortset_num_of_used_levers = num_of_used_levers;
            for (int j = 0; j < num_of_used_levers; ++j) {
                shortest_used_levers_ind[j] = used_levers_ind[j];
            }
        }
        //restore to default
        apply_change_vec_maze(maze, change_vector, maze_size);
        for (int i = 0; i < maze_size; ++i) {
            delete [] parents[i];
        }
        delete [] parents;
    }

    for (int i = 0; i < maze_size; ++i) {
        change_vector[i] = 0;
    }

    change_vector = set_change_vector(maze_size, num_levers, levers, change_vector, shortest_used_levers_ind,
                                      shortset_num_of_used_levers);
    apply_change_vec_maze(maze, change_vector, maze_size);

    if(maze[maze_start.x][maze_start.y] == 1 || shortest_path.size() == 0){
        printf("-1");
        free_all(maze, maze_size, levers, change_vector );
        return -1;
    }

    printPath(shortest_path, levers, num_levers, shortest_used_levers_ind, shortset_num_of_used_levers);
    free_all(maze, maze_size, levers, change_vector );
    return 1;
}
