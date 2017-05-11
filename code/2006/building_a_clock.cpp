#include <iostream>
#include <map>
#include <string>

using namespace std;

#define PB push_back

// the gear with name s has T[s] teeth
map<char, int> T;

// the best solution is stored in the following global variables
string AnswerM , AnswerH; int AnswerShaft , AnswerGear;

// other necessary global variables for this program
int N, R; bool used[7]; char ch[7];


// given a plan of a clock hand denoted by "s", compute its rotation velocity
int Time(string s) {
    int Rs = R, Rt = 1, lasT;
    // record the number of teeth of the last gear in the rotation
    for (int i = 1; i < s.size(); i++) {
        if (s[i] == '-');
        else if (s[i-1] == '-') {
            Rs *= -lasT; Rt *= T[s[i]]; lasT = T[s[i]];
        } else lasT = T[s[i]];
    }
    // if two gears match with each other, than return their velocity ratio
    if (Rs % Rt == 0) return Rs/Rt;
    else return 0;
}


// update the best solution to build the clock
void Refresh(string sm , string sh , int shaft , int gear) {
    // if the minute hand currently does not have a best solution, use current
    // solution as the best one
    if (AnswerM == "X") {
        AnswerM = sm; AnswerH = sh; AnswerShaft = shaft; AnswerGear = gear;
        return;
    }
    // initialize the best common string of the best plan and the current plan
    string sold , snew; sold.clear(); snew.clear();

    // store the gear that drive the minute hand and the hour hand (best plan)
    // into the public string, denoted by "sold"
    for (int i = 0; i < AnswerM.size(); i++)
        if (AnswerM[i] != '*' && AnswerM[i] != '-') sold.PB(AnswerM[i]);
    for (int i = 0; i < AnswerH.size(); i++)
        if (AnswerH[i] != '*' && AnswerH[i] != '-') sold.PB(AnswerH[i]);

    // store the gears that drive the minute hand and the hour hand (current
    // plan) into the public string, denoted by "snew"
    for (int i = 0; i < sm.size(); i++)
        if (sm[i] != '*' && sm[i] != '-') snew.PB(sm[i]);
    for (int i = 0; i < sh.size(); i++)
        if (sh[i] != '*' && sh[i] != '-') snew.PB(sh[i]);

    // change the best plan according to number of shafts, of gears, and the
    // lexographical order of the plan
    if (shaft < AnswerShaft) {
        AnswerM = sm; AnswerH = sh; AnswerShaft = shaft; AnswerGear = gear;
    } else if (shaft == AnswerShaft && gear < AnswerGear) {
        AnswerM = sm; AnswerH = sh; AnswerShaft = shaft; AnswerGear = gear;
    } else if (shaft == AnswerShaft && gear == AnswerGear && snew < sold) {
        AnswerM = sm; AnswerH = sh; AnswerShaft = shaft; AnswerGear = gear;
    }
}


// depth first search over the minute and the hour hand, using the number of
// shafts as the first keyword, the number of gears as the second keyword
void Dfs(string sm, string sh, int shaft, int gear) {
    // there should not be a gear in the third position
    if (sm.size() == 3 || sh.size() == 3) return;
    // there should not be three consecutive gears in the minute hand
    if (sm.size() >= 4 && sm[sm.size()-1] != '-' &&
        sm[sm.size()-2] != '-' && sm[sm.size()-3] != '-') return;
    // there should not be three consecutive gears in the hour hand
    if (sh.size() >= 4 && sh[sh.size()-1] != '-' &&
        sh[sh.size()-2] != '-' && sh[sh.size()-3] != '-') return;
    // if the current plan is worse than the best plan, quit directly
    if (shaft > AnswerShaft || (shaft == AnswerShaft && gear > AnswerGear))
        return;

    if (Time(sm) == 24 && Time(sh) == 2) {
        // if the minute hand rotates once every hour, the hour hand rotates
        // twice every day, than update the best plan
        Refresh(sm, sh, shaft, gear);
    } else if (Time(sm) == 24) {
        // if the minute hand is correct, then handle the hour hand
        if (sm.size() >= 3 && sm[sm.size()-2] != '-') return;
        // iterate over each unused gear
        for (int i = 1; i <= N; i++) if (!used[i]) {
            used[i] = true;
            // add this gear on the same shaft
            Dfs(sm, sh+ch[i], shaft, gear+1);
            // add a new shaft, and connect it to the last gear
            if (sh.size() > 1) Dfs(sm, sh+'-'+ch[i], shaft+1, gear+1);
            // backtracking step, revert back to the last state
            used[i] = false;
        }
    } else {
        // the minute hand is incorrect, then handle it, similar to last case
        for (int i = 1; i <= N; i++) if (!used[i]) {
            used[i] = true;
            Dfs(sm+ch[i], sh, shaft, gear+1);
            if (sm.size() > 1) Dfs(sm+'-'+ch[i], sh, shaft+1, gear+1);
            used[i] = false;
        }
    }
}


// depth first search over common part, using the number of shafts as the 1st
// keyword, the number of gears as the second keyword
void Dfs(string s, int shaft, int gear) {
    // there should not be two common shafts on the original shaft
    if (s.size() == 3) return;
    // there should not be three consecutive shafts on the common path
    if (s.size() >= 4 && s[s.size()-1] != '-' &&
        s[s.size()-2] != '-' && s[s.size()-3] != '-') return;
    // if there are already too much shafts and gears
    if (shaft > AnswerShaft || (shaft == AnswerShaft && gear > AnswerGear))
        return;

    // depth first search, similar to the case in the last method
    Dfs(s, s, shaft, gear);
    for (int i = 1; i <= N; i++) if (!used[i]) {
        used[i] = true;
        Dfs(s+ch[i], shaft, gear+1);
        if (s.size() > 1) Dfs(s+'-'+ch[i], shaft+1, gear+1);
        used[i] = false;
    }
}


int main() {
    int Case = 0;

    while (cin >> N && N) {
        if (Case > 0) cout << "\n";
        cin >> R;
        int x;
        // initialize all gears to be unused, then read input
        for (int i = 1; i <= N; i++) used[i] = false;
        for (int i = 1; i <= N; i++) {
            cin >> ch[i] >> x; T[ch[i]] = x;
        }
        // initialize the best plan, minute hand and hour hand shoube be empty
        // number of used shafts and gears should be infinity
        AnswerM = "X"; AnswerH = "X"; AnswerShaft = AnswerGear = 100000000;

        // calculate the best plan from the first shaft
        Dfs("*", 1, 0);

        if (AnswerM == "X") cout << "Trial " << ++Case << " IS IMPOSSIBLE\n";
        else cout << "Trial " << ++Case << "\nMinutes: " << AnswerM <<
            "\nHours: " << AnswerH <<"\n";
    }

    return 0;
}
