#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

using int64 = long long;

const int N = 5003;
const int64 P = 10000019LL;
const int64 A = 1000003LL;
const int64 B = 97LL;

vector<int> avals = {33,37,39,41};

struct HashTable {
    vector<vector<string>> t;
    long long collisions;
    HashTable():t(N),collisions(0){}
    void insertKey(int idx,const string& s){
        if(!t[idx].empty()) collisions++;
        t[idx].push_back(s);
    }
    bool findKey(int idx,const string& s){
        for(auto& x:t[idx]) if(x==s) return true;
        return false;
    }
    void displayTable(){
        for(int i=0;i<N;i++){
            cout<<i<<":";
            for(int j=0;j<t[i].size();j++){
                if(j==0) cout<<" ";
                cout<<t[i][j];
                if(j+1<t[i].size()) cout<<", ";
            }
            cout<<"\n";
        }
    }
    pair<int,int> maxBucket(){
        int bi=-1,bs=0;
        for(int i=0;i<N;i++){
            if((int)t[i].size()>bs){
                bs=t[i].size();
                bi=i;
            }
        }
        return {bi,bs};
    }
};

int64 hashcode(const string& s,int a){
    int64 h=0;
    for(unsigned char c:s) h=h*a+c;
    return h;
}

int compressDirect(int64 h){
    if(h<0) h=-h;
    return h%N;
}

int compressMAD(int64 h){
    if(h<0) h=-h;
    h%=P;
    return (int)(((A*h+B)%P)%N);
}

vector<string> readStrings(const string& f){
    ifstream in(f);
    vector<string> v;
    string s;
    while(getline(in,s)){
        int i=0,j=s.size();
        while(i<j && isspace((unsigned char)s[i])) i++;
        while(j>i && isspace((unsigned char)s[j-1])) j--;
        if(j>i) v.push_back(s.substr(i,j-i));
    }
    return v;
}

int main(int argc,char** argv){
    string filename;
    if(argc>=2) filename=argv[1];
    else filename="String_collection.txt";

    vector<string> keys = readStrings(filename);
    if(keys.empty()) return 1;

    cout<<"method,a,N,total_collisions,max_bucket_index,max_bucket_size\n";

    for(int a:avals){
        HashTable H;
        for(auto& k:keys){
            int64 h = hashcode(k,a);
            H.insertKey(compressDirect(h),k);
        }
        auto p = H.maxBucket();
        cout<<"direct,"<<a<<","<<N<<","<<H.collisions<<","<<p.first<<","<<p.second<<"\n";

        HashTable M;
        for(auto& k:keys){
            int64 h = hashcode(k,a);
            M.insertKey(compressMAD(h),k);
        }
        auto q = M.maxBucket();
        cout<<"MAD,"<<a<<","<<N<<","<<M.collisions<<","<<q.first<<","<<q.second<<"\n";
    }

    return 0;
}
