#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>
#include<queue>
#include<map>


using namespace std;

struct Team {
    string teamId;
    string projectName;
    string school;
    string category;
    string participants;
    string coach;
};

struct TreeNode {
    Team team;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* root = nullptr;

vector<Team> teams;

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

TreeNode* searchTreeNodeById(TreeNode* node, const string& teamId) {
    if (node == nullptr || node->team.teamId == teamId) {
        return node;
    }

    if (teamId < node->team.teamId) {
        return searchTreeNodeById(node->left, teamId);
    }
    else {
        return searchTreeNodeById(node->right, teamId);
    }
}

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

void searchTeamById() {
    string teamId;
    cout << "请输入要查询的参赛队编号：";
    cin >> teamId;

    TreeNode* result = searchTreeNodeById(root, teamId);

    if (result != nullptr) {
        Team& team = result->team;
        cout << "参赛队编号：" << team.teamId << endl;
        cout << "参赛作品名称：" << team.projectName << endl;
        cout << "参赛学校：" << team.school << endl;
        cout << "赛事类别：" << team.category << endl;
        cout << "参赛者：" << team.participants << endl;
        cout << "指导老师：" << team.coach << endl;
    }
    else {
        cout << "未找到该参赛队伍！" << endl;
    }
}


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
        // 排序
        // 选择排序示例，您可以根据需求选择其他排序算法
        for (int i = 0; i < matchedTeams.size() - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < matchedTeams.size(); j++) {
                if (matchedTeams[j].category < matchedTeams[minIndex].category) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                swap(matchedTeams[i], matchedTeams[minIndex]);
            }
        }

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
    map<string, vector<Team>> finalRooms;  // 使用 map 存储决赛室，键为赛事类别
    int currentRoom = 0;  // 当前叫号的决赛室索引

    // 将参赛队按赛事类别分配到决赛室
    for (const auto& team : teams) {
        finalRooms[team.category].push_back(team);
    }

    // 按顺序叫号并进场
    for (const auto& room : finalRooms) {
        cout << "================== 决赛室 " << room.first << " ==================" << endl;
        cout << "参赛队伍进场顺序：" << endl;

        for (const auto& team : room.second) {
            cout << "参赛队编号：" << team.teamId << endl;
            cout << "参赛作品名称：" << team.projectName << endl;
            cout << "参赛学校：" << team.school << endl;
            cout << "赛事类别：" << team.category << endl;
            cout << "参赛者：" << team.participants << endl;
            cout << "指导老师：" << team.coach << endl;
            cout << "----------------------------------------" << endl;

            // 等待比赛结束，按任意键继续
            cout << "比赛结束，请按任意键继续下一支参赛队伍进场...";
            cin.ignore();  // 忽略之前的换行符
            cin.get();  // 等待按键输入
        }

        cout << "所有参赛队伍已进场并比赛结束！" << endl;
        cout << endl;
    }
}



// 校园导游程序
void navigateCampus() {
    // 实现校园导游程序的逻辑


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
