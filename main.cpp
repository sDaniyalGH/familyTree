#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

// if relatioan eshtab bood --> error
// har chi eshtab bood --> error
// back dashte bashe
// file ein file khodesh
// husband
// 0 to 9 jad      ,,,,,   0 to 9 nave
struct Person {
    string name;
    ll id{};
    ll idDad = -1;
    ll idMom = -1;
    bool gender{}; // 0 male , 1 female
    bool married{};
    ll idWife = -1;
    vector<Person> children;
};
const int oo = 1e5;
vector<Person> people;
set<ll> ids;
ll parentBFS[oo];
ll dis[oo];
vector<ll> hamsayeHa[oo];
bool isChangeable;
string nowStr;
enum Ashkhas {
    brother,
    sister,
    niece,
    nephew,
    aunt,
    uncle,
    grand_Child,
    cousin,
    ancestor,
    parent,
    grand_parent,
    wife,
    husband,
    son,
    daughter
};

void updateHamsayeHa() {

    for (auto &e: people) {

        // add mom
        if (e.idMom != -1)
            hamsayeHa[e.id].push_back(e.idMom);

        // add dad
        if (e.idDad != -1)
            hamsayeHa[e.id].push_back(e.idDad);

        for (auto &child: e.children)
            hamsayeHa[e.id].push_back(child.id);

        // add husband & wife

        if (e.married) {
            hamsayeHa[e.id].push_back(e.idWife);

        }


    }
}

void loadfile(string str) {
    str += ".txt";
    fstream file(str);

    string line;

    while (getline(file, line)) {
        string temp;
        vector<string> infos;
        for (char e: line) {
            if (e != ',')
                temp += e;
            else {
                infos.push_back(temp);
                temp = "";

            }
        }
        infos.push_back(temp);

        Person p;
        p.name = infos[0];
        p.id = stoi(infos[1]);
        p.idDad = stoi(infos[2]);
        p.idMom = stoi(infos[3]);
        p.gender = stoi(infos[4]);
        p.idWife = stoi(infos[5]);
        if (p.idWife != -1)
            p.married = 1;
        else
            p.married = 0;
        ids.insert(p.id);
        people.push_back(p);
    }

    // add children
    for (auto &e: people) {

        for (auto &dadormom: people) {
            if (dadormom.id == e.idMom || dadormom.id == e.idDad)
                dadormom.children.push_back(e);
        }
    }

}

void savefile() {

    fstream file;
    file.open("familyTree.txt", ios::out);

    for (auto &e: people)
        file << e.name << "," << e.id << "," << e.idDad << "," << e.idMom << "," << e.gender << "," << e.idWife << "\n";

    file.close();

}

void addPerson() {

    ll id = ids.size() + 1;

    ids.insert(id);

    string name;
    cout << "name ?\n";
    cin >> name;

    bool gender;
    cout << "\ngender\n0-male\n1-female\n\n";
    cin >> gender;

    ll idParent;
    cout << "\nparent id ??? \n\n\n";
    cin >> idParent;

    while (idParent == id) {
        cout << "\nPlease Enter Another id\n";
        cin >> idParent;
    }


    bool isMarried;
    ll idWife = -1;
    cout << "\nis married??? \n\n\n";
    cin >> isMarried;

    if (isMarried) {
        cout << "\nwife id";
        cin >> idWife;
    }

    Person p;
    p.name = name;
    p.id = id;
    p.gender = gender;

    // id parentBFS
    // must not search its id
    for (auto &e: people) {
        if (e.id == idParent) {
            if (e.gender)
                p.idMom = idParent;
            else
                p.idDad = idParent;

            e.children.push_back(p);
            break;
        }
    }
    p.married = isMarried;
    p.idWife = idWife;


    // add children of wife and set mon or dad of children
    if (isMarried) {
        for (auto &e: people) {
            if (e.id == idWife) {
                auto wife = e;

                p.children = wife.children;

                for (auto &child: p.children) {
                    if (p.gender)
                        child.idMom = id;
                    else
                        child.idDad = id;
                }
                break;

            }
        }
    }

    people.push_back(p);
    updateHamsayeHa();

}

void bfs(ll first) {

    for (int i = 0; i < oo; ++i) {
        dis[i] = oo;
        parentBFS[i] = -1;
    }


    queue<ll> saf;
    saf.push(first);
    dis[first] = 0;

    while (!saf.empty()) {
        ll now = saf.front();
        saf.pop();

        for (auto &each: hamsayeHa[now]) {
            if (dis[each] > dis[now] + 1) {
                dis[each] = dis[now] + 1;
                parentBFS[each] = now;
                saf.push(each);
            }
        }

    }


}

vector<ll> findAfrad(ll first, ll sec) {
    vector<ll> afrad;

    afrad.push_back(sec);
    while (parentBFS[sec] != first) {
        afrad.push_back(parentBFS[sec]);
        sec = parentBFS[sec];
    }

    return afrad;
}

vector<Person> findBroSis(Person person) {

    vector<Person> ans;
    for (auto &e: people) {
        if (((e.idDad == person.idDad && e.idDad != -1) || (e.idMom == person.idMom && e.idMom != -1)) &&
            e.id != person.id) {
            // e is brother or sister

            ans.push_back(e);
        }
    }
    return ans;
}

string findOne(Person first, Person sec) {


    if (first.idWife == sec.id) {
        if (sec.gender)
            return "wife";
        else
            return "husband";


    }
    for (auto &e: first.children) {
        if (e.id == sec.id) {
            if (sec.gender)
                return "daughter";
            else
                return "son";
        }
    }

    //if ((first.idDad != -1 && first.idDad == sec.id) || (first.idMom != -1 && first.idMom == sec.id))
    return "parent";

    // parent or (dad / mom) ?????


}


string tarkib(string first, string second) {
    isChangeable = 1;
    if (first == "brother" || first == "sister") {
        if (second == "son") {
            return "nephew";
        } else if (second == "daughter") {
            return "niece";
        }
        //else return second;
    }

    if (first == "uncle" || first == "aunt") {
        if (second == "parent")
            return "grand_parent";
        else if (second == "daughter" || second == "son")
            return "cousin";
    }

    if (first == "grand_Child") {
        if (second == "son" || second == "daughter")
            return "2Generation";
    }
    if (first.find("Generation") != string::npos) {
        if (second != "parent") {
            return to_string((first[0] - '0') + 1) + "Generation";
        }

    }

    if (first.find("ancestor") != string::npos) {
        if (second == "parent") {
            return to_string((first[0] - '0') + 1) + "ancestor";
        }
    }

    if (first == "grand_parent") {
        if (second == "son")
            return "uncle";
        else if (second == "daughter")
            return "aunt";
        else if (second == "husband" || second == "wife")
            return "grand_parent";
        else if (second == "parent")
            return "1ancestor";
    }

    if (first == "parent") {
        if (second == "son")
            return "brother";
        else if (second == "daughter")
            return "sister";
        else if (second == "husband" || second == "wife")
            return "parent";
        else if (second == "parent")
            return "grand_parent";
    }

    if (first == "daughter" || first == "son") {

//        if (first == "daughter" && second == "parent")
//            return "sister";
//        else if (first == "son" && second == "parent")
//            return "brother";

        if (second == "son" || second == "daughter")
            return "grand_Child";
    }

    if (first == "wife" || first == "husband") {
        if (second == "son" || second == "daughter")
            return second;

    }

    isChangeable = 0;
    return second;
}

queue<string> sadesazi(queue<string> ans) {

    if (ans.size() == 1)
        return ans;

    queue<string> newAns;
    nowStr = ans.front();
    string nextStr;
    ans.pop();


    while (!ans.empty()) {
        nextStr = ans.front();
        ans.pop();
        string tarkibStrs = tarkib(nowStr, nextStr);

        if (!isChangeable) {
            if (newAns.empty())
                newAns.push(nowStr);

            newAns.push(nextStr);
        } else {


            if (!newAns.empty() && tarkibStrs != nextStr)
                newAns.back() = tarkibStrs;
            else
                newAns.push(tarkibStrs);
        }

        nowStr = tarkibStrs;
    }

    return newAns;

}

string findRelationship(ll first, ll sec) {

    queue<string> ans;
    bfs(first);
    vector<ll> afrad = findAfrad(first, sec);
    ll now = first, next;


    // ravabet kenare ham
    while (!afrad.empty()) {
        next = afrad.back();
        afrad.pop_back();
        Person pNow, pNext;

        for (auto &e: people) {
            if (e.id == now)
                pNow = e;
            else if (e.id == next) {
                pNext = e;
            }
        }
        ans.push(findOne(pNow, pNext));
        now = next;
    }





    // sade sazi
    queue<string> newAns;
    queue<string> newnewAns;

    while (1) {
        newAns = sadesazi(ans);
        newnewAns = sadesazi(newAns);

        if (newnewAns == newAns)
            break;
        else
            ans = newnewAns;

    }

    // javab asli
    string returned;
    vector<string> newAnsV;

    while (!newAns.empty()) {
        newAnsV.push_back(newAns.front());
        newAns.pop();
    }

    if (!newAnsV.empty())
        returned = newAnsV[newAnsV.size() - 1];
    else
        returned = nowStr;


    for (int i = newAnsV.size() - 2; i >= 0; --i) {
        returned += " of " + newAnsV[i];

    }

    string a;

    for (auto &ee: returned)
        if (ee != '1')
            a += ee;


    return a;

}

void updateWifeHusband() {
    for (auto &e: people) {
        if (e.married) {

            for (auto &marry: people) {
                if (marry.id == e.idWife) {
                    marry.idWife = e.id;
                    marry.married = 1;

                    // add children

                    for (auto &child: marry.children) {

                        e.children.push_back(child);

                        if (e.gender)
                            child.idMom = e.id;
                        else
                            child.idDad = e.id;


                    }

                    break;
                }

            }


        }
    }
}

void menu() {

    while (true) {
        cout << "\n1-open file\n"
                "2-add new person\n"
                "3-find relationship\n"
                "4-exit\n\n";
        ll num;
        cin >> num;

        if (num == 2) {
            addPerson();
            savefile();
            updateHamsayeHa();
            updateWifeHusband();
        } else if (num == 3) {
            cout << "input first and second\n";
            ll first, sec;
            cin >> first >> sec;
            string p = findRelationship(first, sec);
            cout << p;
        } else if (num == 4)
            exit(0);
    }


}


int main() {

    //cout << "file name ?\n";
    string str = "familyTree";
    //cin >> str;
    loadfile(str);
    updateWifeHusband();
    updateHamsayeHa();
    for (auto &e: people) {
        cout << "\n";
        cout << e.name << " " << e.idDad << " " << e.idMom << " " << e.id << " " << e.gender << " " << e.idWife << "\n";
    }

    menu();
    return 0;
}
