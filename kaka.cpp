#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

struct state {
    bool player;
    string history;
    int street;
    double stack;
    double pot;
    double bet;
    bool isterminal;

    state(bool player, string history, int street, double stack, double pot, double bet, bool isterminal) {
        this->player = player;
        this->history = history;
        this->street = street;
        this->stack = stack;
        this->pot = pot;
        this->bet = bet;
        this->isterminal = isterminal;
    };
};

long long nexthistoryhash(long long history, int action) {
    return history * 10 + action;
}

double betsizes[] = {.25, .5, .75, 1, 1.5, 2};
const int lenbetsizes = 6;

vector<state>* getnextstates(state& s) {

    vector<state>* ret = new vector<state>();

    if(s.isterminal) return ret;
    /// 0 call -> it is always an options, since s is the effective stack
    ret->push_back(
        state(!s.player,
              s.history + "c",
              s.player? s.street : s.street + 1,
              s.stack - s.bet,
              s.pot + s.bet,
              0,
              (s.player == false && s.street == 2)? true : false));
    ///1 fold
    if(s.bet > 0)
    ret->push_back(
        state(!s.player,
              s.history + "f",
              s.street,
              s.stack - s.bet,
              s.pot + s.bet,
              0,
              true));
    ///2 all in
    if(s.bet < s.stack)
        ret->push_back(
            state(!s.player,
                  s.history + "a",
                  s.street,
                  s.stack - s.bet,
                  s.stack + s.pot + s.bet,
                  s.stack - s.bet,
                  false));
    ///3... bet sizes. cant bet x with x - y behind
    char str[5];
    for(int i = 0; i < lenbetsizes; i++) {
        double betsize = betsizes[i] * (s.pot + s.bet);
        if(betsize <= s.stack - s.bet - betsize) {
            ret->push_back(state(!s.player,
                                s.history + "b" + itoa(i, str, 10),
                                s.street,
                                s.stack - s.bet,
                                s.pot + s.bet + betsize,
                                betsize,
                                false));
        }
    }
    return ret;

}

int main()
{
    vector<state> isets;

    state initialstate(true, "", 0, 96, 4, 0, false);

    int totallength = 0;

    isets.push_back(initialstate);

    for(int i = 0; i < isets.size(); i++) {
        int len = isets.size();
        for(; i < len; i++) {
            vector<state>* nextstates = getnextstates(isets[i]);
            for(int i = 0; i < nextstates->size(); i++) {
                isets.push_back((*nextstates)[i]);
                //cout << (*nextstates)[i].history << "  " << (*nextstates)[i].stack<< endl;
            }
            delete nextstates;
        }
        cout << isets.size() << "oh boy\n";
    }
    cout << isets.size() << endl;
}
