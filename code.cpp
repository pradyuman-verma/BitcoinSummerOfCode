//pradyuman verma
#include<bits/stdc++.h>
#include<fstream>
#include <chrono>
using namespace std;

#define int                  long long
#define endl                 "\n"
#define pb                   push_back
#define present(c, e)        (c.find(e) != c.end())
#define cpresent(c, e)       (find(all(c), e) != c.end())

/* 
1. logic behind how I solved the problem, as it is beign asked that we have to maximise the total fee for the block with
with constraint that total weight of transactions in a block must not exceed 4000000 weight, so I made a user defined data
type mempool which will store each row of mempool.csv file.
2. and added a new int variable ratio which will store the ratio of fee and weight, ans then sorted the mempool vector
using this ratio in comp function.
3. then I iterated on the new sorted vector, and if vector[i].weight < max_weight and all the parents id of that vector[i] 
have been marked in map then i will add this vector[i] to my block vector.
4. and I also used a queue which will store all the transaction that are in queue because their parents transaction are not
completed yet, so after each iteration I have checked for each element in the queue that if its parents has been marked
yet or not, if marked i will remove that element from queue.
5. then I putted all the my block vector txids in blokc.txt file
#note - the time-taken by programm in gcc compiler is 3.8096 sec*/

struct mempool{
    string txid;
    vector<string> parent_txids;
    int fee, weight;
    double ratio;
};

class Solution{
    public :
        void Solve(){
            ifstream ip("mempool.csv");
            ofstream op("block.txt", ofstream::out);
            if(!ip.is_open()) cout << "ERROR: file open" << endl;
            string txid, fee, weight, parent_txids;
            vector<mempool> transactions;
            int i = 0;
            while(ip.good()){
                mempool next;
                getline(ip, txid, ',');
                getline(ip, fee, ',');
                getline(ip, weight, ',');
                getline(ip, parent_txids, '\n');
                next.txid = txid;
                next.parent_txids = split(parent_txids, ';');
                next.fee = stoi(fee);
                next.weight = stoi(weight);
                next.ratio = (double)next.fee / (double)next.weight;
                if(i != 0) transactions.pb(next);
                i += 1;
            }
            sort(transactions.begin(), transactions.end(), comp);
            int max_weight = 4000000, total_fee = 0;
            vector<string> block;
            unordered_map<string, int> isDone;
            isDone[""] = 1;
            queue<mempool> inQueue;
            for(int i = 0; i < transactions.size(); i ++){
                if(transactions[i].weight <= max_weight){
                    vector<string> parents = transactions[i].parent_txids;
                    bool isNext = true;
                    for(auto prnt : parents){
                        if(isDone.find(prnt) == isDone.end()){
                            isNext = false;
                            inQueue.push(transactions[i]);
                            break;
                        }
                    }
                    if(isNext){
                        max_weight -= transactions[i].weight;
                        isDone[transactions[i].txid] = 1;
                        total_fee += transactions[i].fee;
                        block.push_back(transactions[i].txid);
                    }
                    int n = inQueue.size(), i = 0;
                    while(i < n){
                        mempool curr = inQueue.front();
                        inQueue.pop();
                        vector<string> parents = curr.parent_txids;
                        bool isNext = true;
                        for(auto prnt : parents){
                            if(isDone.find(prnt) == isDone.end()){
                                isNext = false;
                                inQueue.push(curr);
                                break;
                            }
                        }
                        if(isNext){
                            max_weight -= curr.weight;
                            isDone[curr.txid] = 1;
                            total_fee += curr.fee;
                            block.push_back(curr.txid);
                            i = 0;
                            n = inQueue.size();
                        }else i += 1;
                    }
                }else break;
            }
            //cout << total_fee << " " << max_weight;
            if(op.is_open()){
                for (int i = 0; i < block.size(); i++) {
                    op << block[i] << "\n";
                }
                op.close();
            }else cout << "Problem with file opening";
        }
        static bool comp(mempool A, mempool B){
            return(A.ratio > B.ratio);
        }
        static int stoi(string str){
            int ans = 0;
            int n = str.length();
            int i = 0;
            if(str[0] == '-') i += 1;
            while(i < n){
                if(isdigit(str[i])){
                    ans = ans * 10 + (str[i] - '0');
                }else return(-1);
                i += 1;
            }
            ans *= (str[0] == '-') ? -1 : 1;
            return(ans);
        }
        static vector<string> split(const string& str, char delim){
            auto i = 0;
            vector<string> list;
            auto pos = str.find(delim);
            while(pos != string::npos){
                list.push_back(str.substr(i, pos - i));
                i = ++pos;
                pos = str.find(delim, pos);
            }
            list.push_back(str.substr(i, str.length()));
            return list;
        }
};

signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    //auto start = chrono::high_resolution_clock::now();
    Solution ob;
    ob.Solve();
    // auto end = chrono::high_resolution_clock::now();
    // double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    // time_taken *= 1e-9;
    // cout << "Time taken by program is : " << fixed << time_taken << setprecision(5);
    // cout << " sec";
    return 0;
}
//g++ -o out code.cpp