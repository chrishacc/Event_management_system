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
        return "";  // �ַ�����ֻ�����հ��ַ�
    }
    return str.substr(start, end - start + 1);
}


void loadTeamsFromFile() {
    ifstream file("team.txt");
    if (!file) {
        cout << "�޷��������ļ� team.txt��" << endl;
        return;
    }

    teams.clear();
    root = nullptr; // ��ն���������

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
            insertTreeNode(root, team); // ����ǰ Team ������������������
        }
    }

    file.close();
}


void saveTeamsToFile() {
    ofstream file("team.txt");
    if (!file) {
        cout << "�޷����������ļ� team.txt��" << endl;
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
    cout << "����������ӱ�ţ�";
    cin >> team.teamId;
    cout << "�����������Ʒ���ƣ�";
    cin >> team.projectName;
    cout << "���������ѧУ��";
    cin >> team.school;
    cout << "�������������";
    cin >> team.category;
    cout << "����������ߣ�";
    cin >> team.participants;
    cout << "������ָ����ʦ��";
    cin >> team.coach;

    teams.push_back(team);
    cout << "����������ӳɹ���" << endl;
}

void deleteTeam() {
    string teamId;
    cout << "������Ҫɾ���Ĳ����ӱ�ţ�";
    cin >> teamId;

    auto it = find_if(teams.begin(), teams.end(), [teamId](const Team& team) {
        return team.teamId == teamId;
        });

    if (it != teams.end()) {
        teams.erase(it);
        cout << "��������ɾ���ɹ���" << endl;
    }
    else {
        cout << "δ�ҵ�Ҫɾ���Ĳ������飡" << endl;
    }
}

void modifyTeam() {
    string teamId;
    cout << "������Ҫ�޸ĵĲ����ӱ�ţ�";
    cin >> teamId;

    auto it = find_if(teams.begin(), teams.end(), [teamId](const Team& team) {
        return team.teamId == teamId;
        });

    if (it != teams.end()) {
        cout << "�������µĲ�����Ʒ���ƣ�";
        cin >> it->projectName;
        cout << "�������µĲ���ѧУ��";
        cin >> it->school;
        cout << "�������µ��������";
        cin >> it->category;
        cout << "�������µĲ����ߣ�";
        cin >> it->participants;
        cout << "�������µ�ָ����ʦ��";
        cin >> it->coach;

        cout << "���������޸ĳɹ���" << endl;
    }
    else {
        cout << "δ�ҵ�Ҫ�޸ĵĲ������飡" << endl;
    }
}

void searchTeamById() {
    string teamId;
    cout << "������Ҫ��ѯ�Ĳ����ӱ�ţ�";
    cin >> teamId;

    TreeNode* result = searchTreeNodeById(root, teamId);

    if (result != nullptr) {
        Team& team = result->team;
        cout << "�����ӱ�ţ�" << team.teamId << endl;
        cout << "������Ʒ���ƣ�" << team.projectName << endl;
        cout << "����ѧУ��" << team.school << endl;
        cout << "�������" << team.category << endl;
        cout << "�����ߣ�" << team.participants << endl;
        cout << "ָ����ʦ��" << team.coach << endl;
    }
    else {
        cout << "δ�ҵ��ò������飡" << endl;
    }
}


void searchTeamsBySchool() {
    string school;
    cout << "���������ѧУ���ƣ�";
    cin >> school;

    vector<Team> matchedTeams;
    for (const auto& team : teams) {
        if (team.school == school) {
            matchedTeams.push_back(team);
        }
    }

    if (!matchedTeams.empty()) {
        cout << "������ѧУ��ѯ�����" << endl;
        // ����
        // ѡ������ʾ���������Ը�������ѡ�����������㷨
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

        // ������
        for (const auto& team : matchedTeams) {
            cout << "�����ӱ�ţ�" << team.teamId << endl;
            cout << "������Ʒ���ƣ�" << team.projectName << endl;
            cout << "�������" << team.category << endl;
            cout << "�����ߣ�" << team.participants << endl;
            cout << "ָ����ʦ��" << team.coach << endl;
            cout << "----------------------------------------" << endl;
        }
    }
    else {
        cout << "δ�ҵ����������Ĳ������飡" << endl;
    }
}

// �����к�ϵͳ
void callFinalRooms() {
    map<string, vector<Team>> finalRooms;  // ʹ�� map �洢�����ң���Ϊ�������
    int currentRoom = 0;  // ��ǰ�кŵľ���������

    // �������Ӱ����������䵽������
    for (const auto& team : teams) {
        finalRooms[team.category].push_back(team);
    }

    // ��˳��кŲ�����
    for (const auto& room : finalRooms) {
        cout << "================== ������ " << room.first << " ==================" << endl;
        cout << "�����������˳��" << endl;

        for (const auto& team : room.second) {
            cout << "�����ӱ�ţ�" << team.teamId << endl;
            cout << "������Ʒ���ƣ�" << team.projectName << endl;
            cout << "����ѧУ��" << team.school << endl;
            cout << "�������" << team.category << endl;
            cout << "�����ߣ�" << team.participants << endl;
            cout << "ָ����ʦ��" << team.coach << endl;
            cout << "----------------------------------------" << endl;

            // �ȴ����������������������
            cout << "�����������밴�����������һ֧�����������...";
            cin.ignore();  // ����֮ǰ�Ļ��з�
            cin.get();  // �ȴ���������
        }

        cout << "���в��������ѽ���������������" << endl;
        cout << endl;
    }
}



// У԰���γ���
void navigateCampus() {
    // ʵ��У԰���γ�����߼�


}

int main() {
    loadTeamsFromFile();

    while (true) {
        cout << "================== ���¹���ϵͳ ==================" << endl;
        cout << "1. ��Ӳ�������" << endl;
        cout << "2. ɾ����������" << endl;
        cout << "3. �޸Ĳ�������" << endl;
        cout << "4. �������ӱ�Ų�ѯ��������" << endl;
        cout << "5. ������ѧУ��ѯ��������" << endl;
        cout << "6. �����к�ϵͳ" << endl;
        cout << "0. �˳�ϵͳ" << endl;
        cout << "=================================================" << endl;
        cout << "���������ѡ�";

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
            cout << "��лʹ�����¹���ϵͳ���ټ���" << endl;
            return 0;
        default:
            cout << "��Ч�Ĳ���ѡ����������롣" << endl;
            break;
        }

        cout << endl;
    }
}
