#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>
#include<queue>
#include<map>
#include <tuple>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//参赛队伍信息结构体
struct Team {
    string teamId;
    string projectName;
    string school;
    string category;
    string participants;
    string coach;
};

//二叉排序树结点
struct TreeNode {
    Team team;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* root = nullptr;

vector<Team> teams;

//队列

//队列结点
struct QueueNode {
    Team team;
    QueueNode* next;
};

//队列的实现
class Queue {
private:
    QueueNode* front;
    QueueNode* rear;
public:
    Queue() : front(nullptr), rear(nullptr) {}
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(const Team& team) {
        QueueNode* newNode = new QueueNode;
        newNode->team = team;
        newNode->next = nullptr;

        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }
    void dequeue() {
        if (isEmpty()) {
            return;
        }

        QueueNode* temp = front;
        front = front->next;
        delete temp;

        if (front == nullptr) {
            rear = nullptr;
        }
    }

    Team frontElement() const {
        if (isEmpty()) {
            throw logic_error("队列为空");
        }

        return front->team;
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};

//二叉排序树的插入
void insertTreeNode(TreeNode*& node, const Team& team) {
    if (node == nullptr) {
        node = new TreeNode;
        node->team = team;
        node->left = nullptr;
        node->right = nullptr;
        return;
    }

    if (team.teamId < node->team.teamId) {
        insertTreeNode(node->left, team);
    }
    else {
        insertTreeNode(node->right, team);
    }
}

struct SearchResult {
    TreeNode* node;
    int length;
};

SearchResult searchTreeNodeById(TreeNode* node, const string& teamId, int length = 1) {
    SearchResult result;

    if (node == nullptr || node->team.teamId == teamId) {
        result.node = node;
        result.length = length;
        return result;
    }

    if (teamId < node->team.teamId) {
        return searchTreeNodeById(node->left, teamId, length + 1);
    }
    else {
        return searchTreeNodeById(node->right, teamId, length + 1);
    }
}

//删除字符串中的空格
string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos || end == std::string::npos) {
        return "";  // 字符串中只包含空白字符
    }
    return str.substr(start, end - start + 1);
}


void loadTeamsFromFile() {
    ifstream file("team.txt");
    if (!file) {
        cout << "无法打开数据文件 team.txt！" << endl;
        return;
    }

    teams.clear();
    root = nullptr; // 清空二叉排序树

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Team team;
        string item;
        vector<string> items;

        while (getline(ss, item, '#')) {
            item = trim(item);
            items.push_back(item);
        }
        if (items.size() == 6) {
            team.teamId = items[0];
            team.projectName = items[1];
            team.school = items[2];
            team.category = items[3];
            team.participants = items[4];
            team.coach = items[5];

            teams.push_back(team);
            insertTreeNode(root, team); // 将当前 Team 对象插入二叉排序树中
        }
    }

    file.close();
}

//保存参赛队伍信息到文件
void saveTeamsToFile() {
    ofstream file("team.txt");
    if (!file) {
        cout << "无法创建数据文件 team.txt！" << endl;
        return;
    }

    for (const auto& team : teams) {
        file << team.teamId << "#" << team.projectName << "#" << team.school << "#"
            << team.category << "#" << team.participants << "#" << team.coach << endl;
    }

    file.close();
}

//参赛队伍的添加
void addTeam() {
    Team team;
    cout << "请输入参赛队编号：";
    cin >> team.teamId;
    cout << "请输入参赛作品名称：";
    cin >> team.projectName;
    cout << "请输入参赛学校：";
    cin >> team.school;
    cout << "请输入赛事类别：";
    cin >> team.category;
    cout << "请输入参赛者：";
    cin >> team.participants;
    cout << "请输入指导老师：";
    cin >> team.coach;

    teams.push_back(team);
    cout << "参赛队伍添加成功！" << endl;
}

//参赛队伍的删除
void deleteTeam() {
    string teamId;
    cout << "请输入要删除的参赛队编号：";
    cin >> teamId;

    auto it = find_if(teams.begin(), teams.end(), [teamId](const Team& team) {
        return team.teamId == teamId;
        });

    if (it != teams.end()) {
        teams.erase(it);
        cout << "参赛队伍删除成功！" << endl;
    }
    else {
        cout << "未找到要删除的参赛队伍！" << endl;
    }
}

//参赛队伍的修改
void modifyTeam() {
    string teamId;
    cout << "请输入要修改的参赛队编号：";
    cin >> teamId;

    auto it = find_if(teams.begin(), teams.end(), [teamId](const Team& team) {
        return team.teamId == teamId;
        });

    if (it != teams.end()) {
        cout << "请输入新的参赛作品名称：";
        cin >> it->projectName;
        cout << "请输入新的参赛学校：";
        cin >> it->school;
        cout << "请输入新的赛事类别：";
        cin >> it->category;
        cout << "请输入新的参赛者：";
        cin >> it->participants;
        cout << "请输入新的指导老师：";
        cin >> it->coach;

        cout << "参赛队伍修改成功！" << endl;
    }
    else {
        cout << "未找到要修改的参赛队伍！" << endl;
    }
}

//根据参赛队伍编号查询
void searchTeamById() {
    string teamId;
    cout << "请输入要查询的参赛队编号：";
    cin >> teamId;

    SearchResult searchResult = searchTreeNodeById(root, teamId);
    TreeNode* result = searchResult.node;
    int asl = searchResult.length;

    if (result != nullptr) {
        Team& team = result->team;
        cout << "参赛队编号：" << team.teamId << endl;
        cout << "参赛作品名称：" << team.projectName << endl;
        cout << "参赛学校：" << team.school << endl;
        cout << "赛事类别：" << team.category << endl;
        cout << "参赛者：" << team.participants << endl;
        cout << "指导老师：" << team.coach << endl;

        cout << "平均查找长度 ASL：" << asl << endl;
    }
    else {
        cout << "未找到该参赛队伍！" << endl;
    }
}

//根据参赛队伍所属学校查询参赛队伍

//比较学校名称
bool compareSchool(const string& school1, const string& school2) {
    int len1 = school1.length();
    int len2 = school2.length();
    int minLen = min(len1, len2);

    for (int i = 0; i < minLen; i++) {
        if (school1[i] < school2[i]) {
            return true;
        }
        else if (school1[i] > school2[i]) {
            return false;
        }
    }

    return len1 <= len2;
}

//快速排序的分区函数
int partition(vector<Team>& teams, int left, int right) {
    Team pivot = teams[right];  // 选择最右边的元素作为基准元素
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (compareSchool(teams[j].school, pivot.school)) {
            i++;
            swap(teams[i], teams[j]);
        }
    }

    swap(teams[i + 1], teams[right]);
    return i + 1;
}

//快速排序
void quickSort(vector<Team>& teams, int left, int right) {
    if (left >= right) {
        return;
    }

    int pivotIndex = partition(teams, left, right);
    quickSort(teams, left, pivotIndex - 1);
    quickSort(teams, pivotIndex + 1, right);
}

//根据学校查找参赛队伍主函数
void searchTeamsBySchool() {
    string school;
    cout << "请输入参赛学校名称：";
    cin >> school;

    vector<Team> matchedTeams;
    for (const auto& team : teams) {
        if (team.school == school) {
            matchedTeams.push_back(team);
        }
    }

    if (!matchedTeams.empty()) {
        cout << "按参赛学校查询结果：" << endl;

        // 快速排序
        quickSort(matchedTeams, 0, matchedTeams.size() - 1);

        // 输出结果
        for (const auto& team : matchedTeams) {
            cout << "参赛队编号：" << team.teamId << endl;
            cout << "参赛作品名称：" << team.projectName << endl;
            cout << "赛事类别：" << team.category << endl;
            cout << "参赛者：" << team.participants << endl;
            cout << "指导老师：" << team.coach << endl;
            cout << "----------------------------------------" << endl;
        }
    }
    else {
        cout << "未找到符合条件的参赛队伍！" << endl;
    }
}

// 决赛叫号系统
void callFinalRooms() {
    map<string, Queue> finalRooms;  // 决赛室队列，按赛事类别存储参赛队伍
    vector<string> categories;  // 赛事类别列表

    // 将参赛队伍按赛事类别存储到决赛室队列中
    for (const auto& team : teams) {
        finalRooms[team.category].enqueue(team);
        if (find(categories.begin(), categories.end(), team.category) == categories.end()) {
            categories.push_back(team.category);
        }
    }

    // 按顺序叫号，参赛队伍进场
    for (const auto& category : categories) {
        cout << "赛事类别：" << category << endl;
        cout << endl;
        Queue& roomQueue = finalRooms[category];

        while (!roomQueue.isEmpty()) {
            Team team = roomQueue.frontElement();
            roomQueue.dequeue();


            cout << "队伍编号：" << team.teamId << "，项目名称：" << team.projectName << endl;
            // 进场操作
            cout << "参赛队伍进入赛场..." << endl;
            // 进行比赛...
            cout <<"参赛队伍比赛完毕" << endl;
            // 比赛结束后，等待下一参赛队伍进场
            if (!roomQueue.isEmpty()) {
                cout << "等待下一参赛队伍进场..." << endl;
                cout << "------------------------" << endl;
            }
        }

        cout << endl;
    }
}


// 校园导游程序

// 定义校园地点及其坐标的结构体
struct CampusLocation {
    Point coordinates;
    string description;
};

// 自定义比较函数，用于优先队列的最小堆
struct CompareDistance {
    bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    }
};

void navigateCampus() {
    // 校园地图和地点之间的连接关系及权值
    map<string, CampusLocation> campus_map = {
        {"后勤服务楼", {Point(79, 206), "后勤服务楼"}},
        {"西门", {Point(403, 179), "西门"}},
        {"大学生活动中心", {Point(211, 354), "大学生活动中心"}},
        {"明德楼", {Point(403, 263), "明德楼"}},
        {"校训碑", {Point(302, 480), "校训碑"}},
        {"文理大楼", {Point(563, 264), "文理大楼"}},
        {"计算机学院楼", {Point(652, 214), "计算机学院楼"}},
        {"图书馆", {Point(622, 387), "图书馆"}},
        {"东门", {Point(736, 532), "东门"}},
        {"行政大楼", {Point(991, 168), "行政大楼"}}
    };

    map<pair<string, string>, int> connectivity = {
        {{"后勤服务楼", "西门"}, 500},
        {{"后勤服务楼", "大学生活动中心"}, 300},
        {{"大学生活动中心", "明德楼"}, 200},
        {{"西门", "明德楼"}, 100},
        {{"大学生活动中心", "校训碑"}, 250},
        {{"明德楼", "文理大楼"}, 100},
        {{"校训碑", "图书馆"}, 500},
        {{"文理大楼", "图书馆"}, 200},
        {{"文理大楼", "计算机学院楼"}, 100},
        {{"图书馆", "东门"}, 250},
        {{"计算机学院楼", "行政大楼"}, 500}
    };

    // 导航功能
    auto navigate = [&](const string& start, const string& end) {
        // 创建距离字典，初始距离设为无穷大
        map<string, int> distance;
        for (const auto& place : campus_map) {
            distance[place.first] = INT_MAX;
        }
        distance[start] = 0;

        // 创建优先队列，用于选择最小距离的下一个节点
        priority_queue<pair<string, int>, vector<pair<string, int>>, CompareDistance> pq;
        pq.push({ start, 0 });

        // 创建父节点字典，用于记录最短路径
        map<string, string> parent;
        parent[start] = "";

        while (!pq.empty()) {
            string current = pq.top().first;
            int curr_distance = pq.top().second;
            pq.pop();

            if (current == end) {
                break;  // 到达目标地点，停止搜索
            }

            for (const auto& connection : connectivity) {
                string src = connection.first.first;
                string dest = connection.first.second;
                int weight = connection.second;

                if (src == current && curr_distance + weight < distance[dest]) {
                    distance[dest] = curr_distance + weight;
                    parent[dest] = src;
                    pq.push({ dest, distance[dest] });
                }

                if (dest == current && curr_distance + weight < distance[src]) {
                    distance[src] = curr_distance + weight;
                    parent[src] = dest;
                    pq.push({ src, distance[src] });
                }
            }
        }

        // 从终点回溯获取最短路径
        int shortest_distance = distance[end];
        Point start_coord = campus_map[start].coordinates;
        Point end_coord = campus_map[end].coordinates;
        string current = end;
        vector<string> path;
        while (current != start) {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        return make_tuple(shortest_distance, start_coord, end_coord, path);
    };

    // 目标地导航
    cout << "欢迎来到校园导游程序！" << endl;
    cout << "以下是校园地图中的一些目标地：" << endl;
    for (const auto& place : campus_map) {
        cout << place.second.description << endl;
    }

    // 任意两个目标地导航查询
    while (true) {
        string start_place, end_place;
        cout << "请输入起点建筑物名称：";
        cin >> start_place;
        cout << "请输入终点建筑物名称：";
        cin >> end_place;

        if (campus_map.count(start_place) && campus_map.count(end_place)) {
            int distance;
            Point start_coord, end_coord;
            vector<string> path;
            tie(distance, start_coord, end_coord, path) = navigate(start_place, end_place);
            cout << "从 " << start_place << " 到 " << end_place << " 的最短路径距离为： " << distance << endl;

            // 输出最短路径
            cout << "最短路径为：";
            for (const auto& place : path) {
                cout << place << "  ->  ";
            }
            cout << endl;

            // 读取现有校园地图图片
            Mat image = imread("campus_map.jpg");

            // 在图片上绘制导航路线
            for (size_t i = 1; i < path.size(); i++) {
                Point start = campus_map[path[i - 1]].coordinates;
                Point end = campus_map[path[i]].coordinates;
                line(image, start, end, Scalar(0, 255, 0), 3);
            }

            // 显示图片
            namedWindow("Campus Map with Route", WINDOW_NORMAL);
            imshow("Campus Map with Route", image);
            waitKey(0);

            // 保存修改后的图片
            imwrite("campus_map_with_route.jpg", image);

            cout << "导航路线已在图片上显示，并保存为 campus_map_with_route.jpg" << endl;
            break;
        }
        else {
            cout << "输入的建筑物名称有误，请重新输入！" << endl;
        }
    }
}


int main() {
    loadTeamsFromFile();

    while (true) {
        cout << "================== 赛事管理系统 ==================" << endl;
        cout << "1. 添加参赛队伍" << endl;
        cout << "2. 删除参赛队伍" << endl;
        cout << "3. 修改参赛队伍" << endl;
        cout << "4. 按参赛队编号查询参赛队伍" << endl;
        cout << "5. 按参赛学校查询参赛队伍" << endl;
        cout << "6. 决赛叫号系统" << endl;
        cout << "7. 校园导游程序" << endl;
        cout << "0. 退出系统" << endl;
        cout << "=================================================" << endl;
        cout << "请输入操作选项：";

        int option;
        cin >> option;

        switch (option) {
        case 1:
            addTeam();
            saveTeamsToFile();
            break;
        case 2:
            deleteTeam();
            saveTeamsToFile();
            break;
        case 3:
            modifyTeam();
            saveTeamsToFile();
            break;
        case 4:
            searchTeamById();
            break;
        case 5:
            searchTeamsBySchool();
            break;
        case 6:
            callFinalRooms();
            break;
        case 7:
            navigateCampus();
			break;
        case 0:
            cout << "感谢使用赛事管理系统！再见！" << endl;
            return 0;
        default:
            cout << "无效的操作选项！请重新输入。" << endl;
            break;
        }

        cout << endl;
    }
}
