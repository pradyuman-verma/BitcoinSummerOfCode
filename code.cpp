//pradyuman verma
#include<bits/stdc++.h>
#include<fstream>
#include <chrono>
using namespace std;

#define int long long

/* Here is the output of code -- 
    Total number of transactions read: 5214
    Number of tx in final block: 3174
    Total fee in final block : 5696031
    Total weight of fnal block: 3999936
    Percentage of weight: 99.99840 %
    Time taken by program is : 1.53695 sec
*/
//created a class for Transactions

struct Transaction{ 
    string tx_id;
    vector<string> parent_txids;
    int fee, weight;
    float ratio;
};

class Solution{
    public :
        void Solve(){
            unordered_map<string, Transaction *> txn_hash; //unordered map to map txn_ids with transactions
            read_csv("mempool.csv", txn_hash);
            set<pair<float, Transaction *>, greater<pair<float, Transaction *>>> block; //it will contain all the transaction sorted in non increasing order of ratio.
            set<string> done_txnids;
            vector<string> mempool;
            double max_weight = 4000000;
            double totalBlockWeight = 0;
            int total_fee = 0;

            for(auto txn : txn_hash)
                block.insert({txn.second->ratio, txn.second});
            
            while (!block.empty() && totalBlockWeight < max_weight){
                bool found = false;
                for (auto &txn : block){
                    Transaction *curr_tx = txn.second;
                    int currFee = curr_tx->fee;
                    int currWeight = curr_tx->weight;
                    if (isValid(curr_tx, done_txnids) && ((totalBlockWeight + currWeight) <= max_weight)){
                        totalBlockWeight += currWeight;
                        done_txnids.insert(curr_tx->tx_id);
                        mempool.push_back(curr_tx->tx_id);
                        total_fee += currFee;
                        block.erase(txn);
                        found = true;
                        break;
                    }
                }
                if(!found)
                    break;
            }

            cout << fixed << setprecision(5) << endl;
            cout << "Number of tx in final block " << done_txnids.size() << endl;
            cout << "Total fee in final block : " << total_fee <<"\n";
            cout << "Total weight of final block: " << totalBlockWeight << endl;
            cout<<"Percentage of weight: "<< (double)(totalBlockWeight / max_weight) * 100.0 <<" %"<< endl;
            writeOutput(mempool, "block.txt");
        }

        pair<string, Transaction*> createTransaction(vector<string> &row){
            auto next_txn = new Transaction();
            next_txn->tx_id = row[0];
            next_txn->fee = stoi(row[1]);
            next_txn->weight = stoi(row[2]);
            next_txn->ratio = (float)next_txn->fee / (float)next_txn->weight;
            vector<string> p;
            for (int i = 3; i < row.size(); i++)
                p.push_back(row[i]);
            next_txn->parent_txids = p;
            return {row[0], next_txn};
        }

        void read_csv(string fileName, unordered_map<string, Transaction*> &txn_hash){
            ifstream fin(fileName);
            vector<string> row;
            string temp, line, word;
            getline(fin, line);
            while (getline(fin, line)){
                row.clear();
                stringstream s(line);
                while (getline(s, word, ','))
                    row.push_back(word); 
                pair<string, Transaction*> p = createTransaction(row);
                txn_hash[p.first] = p.second;
            }
            fin.close();
            cout << "Total number of transactions read: " << txn_hash.size() << endl;
        }

        bool isValid(Transaction *tx, set<string> &done_txnids){
            for (auto parent : tx->parent_txids)
                if (done_txnids.find(parent) == done_txnids.end())
                    return false;
            return true;
        }

        void writeOutput(vector<string> &mempool, string fn){
            ofstream myfile(fn);
            for (auto s : mempool)
                myfile << s << endl;
            myfile.close();
        }
};

signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    auto start = chrono::high_resolution_clock::now();
    Solution ob;
    ob.Solve();
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << "Time taken by program is : " << fixed << time_taken << setprecision(5);
    cout << " sec";
    return 0;
}
/* References - 
    -> https://en.wikipedia.org/wiki/Continuous_knapsack_problem
    -> https://github.com/mempool/mempool
    -> https://www.blockchain.com/charts/mempool-size
*/