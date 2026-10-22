#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random> // ضيف هذا
using namespace std;

struct Player {
    string name;
    string role;
    bool isAlive = true;
    int suspicion = 0;
    int tasksCompleted = 0;
};

struct Task {
    string name;
    bool done = false;
};

class AmongUsPro {
private:
    vector<Player> players;
    vector<Task> tasks;
    int impostorCount = 2;

public:
    AmongUsPro() {
        srand(time(0));
        // 10 لاعبين
        vector<string> names = {"You","Red","Blue","Green","Pink","Orange","Yellow","Black","White","Purple"};
        for(int i=0; i<10; i++) {
            string role = (i < impostorCount)? "Impostor" : "Crewmate";
            players.push_back({names[i], role});
        }
        shuffle(players.begin(), players.end(), default_random_engine(time(0)));

        // 4 مهام مختلفة
        tasks = {{"Fix Wires",false}, {"Download Data",false}, {"Empty Garbage",false}, {"Scan Medbay",false}};
    }

    void doTask() {
        for(auto &t : tasks) {
            if(!t.done) {
                cout << "\n[TASK] Doing: " << t.name << endl;
                t.done = true;
                players[0].tasksCompleted++; // انت
                break;
            }
        }

        int doneCount = count_if(tasks.begin(), tasks.end(), [](Task t){return t.done;});
        cout << "Progress: " << doneCount << "/" << tasks.size() << endl;

        if(doneCount == tasks.size()) {
            cout << "\n[WIN] CREWMATES WIN! All tasks done\n"; exit(0);
        }
    }

    void impostorAI() {
        for(auto &imp : players) {
            if(imp.role == "Impostor" && imp.isAlive) {
                // AMK AI: يقتل اقل واحد مشكوك بيه
                int target = -1; int minSusp = 999;
                for(int i=0; i<players.size(); i++) {
                    if(players[i].isAlive && players[i].role == "Crewmate" && players[i].suspicion < minSusp) {
                        minSusp = players[i].suspicion; target = i;
                    }
                }
                if(target!= -1) {
                    players[target].isAlive = false;
                    cout << "\n[BODY FOUND] " << players[target].name << " was killed\n";
                    for(auto &p : players) if(p.isAlive) p.suspicion += rand()%3 + 1; // زيادة شك عشوائي
                }
            }
        }
    }

    void meeting() {
        cout << "\n====== EMERGENCY MEETING ======\n";
        for(int i=0; i<players.size(); i++) {
            if(players[i].isAlive)
                cout << i << ". " << players[i].name << " [Suspicion:" << players[i].suspicion << "]\n";
        }
        int vote; cout << "Vote: "; cin >> vote;

        if(players[vote].role == "Impostor") {
            cout << "\n[CORRECT] " << players[vote].name << " was Impostor!\n";
            players[vote].isAlive = false;
        } else {
            cout << "\n[WRONG] " << players[vote].name << " was innocent\n";
            players[vote].isAlive = false;
        }
        checkWin();
    }

    void checkWin() {
        int aliveCrew = 0, aliveImp = 0;
        for(auto &p : players) {
            if(p.isAlive && p.role == "Crewmate") aliveCrew++;
            if(p.isAlive && p.role == "Impostor") aliveImp++;
        }
        if(aliveImp == 0) {cout << "\n[WIN] CREWMATES WIN!\n"; exit(0);}
        if(aliveCrew <= aliveImp) {cout << "\n[WIN] IMPOSTORS WIN!\n"; exit(0);}
    }

    void play() {
        while(true) {
            cout << "\n====== Among Us PRO ======\n";
            cout << "1. Do Task 2. Call Meeting 3. Status 4. Exit\nChoice: ";
            int c; cin >> c;

            if(c==1) doTask();
            if(c==2) meeting();
            if(c==3) showStatus();
            if(c==4) break;

            impostorAI(); // دور الـ AI
            checkWin();
        }
    }

    void showStatus() {
        cout << "\n--- STATUS ---\n";
        cout << "Your tasks: " << players[0].tasksCompleted << endl;
        int alive = count_if(players.begin(), players.end(), [](Player p){return p.isAlive;});
        cout << "Players alive: " << alive << endl;
    }
};

int main() {
    cout << "Welcome to Among Us PRO - C++\n";
    cout << "10 Players, 2 Impostors, 4 Tasks\n";
    AmongUsPro game;
    game.play();
}
