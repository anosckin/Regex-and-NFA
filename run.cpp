#pragma GCC diagnostic warning "-std=c++11"
#include<bits/stdc++.h>
#define ll long long
#define pb push_back
#define f first
#define s second
#define INF INT_MAX
#define MOD 1000000007
#define pause system("pause")
#define all(x) (x).begin(),(x).end()
#define deb(x) cout << #x << " " << x << endl
using namespace std;
const int MAX_SIZE = 10000000;
string s;
vector <pair<int,char> > adj[MAX_SIZE];
bool accept[MAX_SIZE];
vector <int> cur_states;
vector <int> temp;
char ans[MAX_SIZE];

void solve (string &s){
	cur_states.pb(0);
	for (int w=0;w<s.size();w++){
//		cout<<"i: "<<i<<endl;
//		for (int i=0;i<cur_states.size();i++){
//			cout<<cur_states[i]<<" ";
//		}
//		cout<<endl;
		for (int i=0;i<cur_states.size();i++){
			int u=cur_states[i];
			for (int j=0;j<adj[u].size();j++){
				if (adj[u][j].s==s[w]){
					temp.pb(adj[u][j].f);
				}
			}
		}
		
		cur_states.clear();
		for (int i=0;i<temp.size();i++){
			cur_states.pb(temp[i]);
		}
	
		temp.clear();
//		
//		cout<<"i: "<<i<<endl;
//		for (int i=0;i<cur_states.size();i++){
//			cout<<cur_states[i]<<" ";
//		}
//		cout<<endl;
		ans[w]='N';
		for (int i=0;i<cur_states.size();i++){
			if (accept[cur_states[i]]) ans[w]='Y';
		}
	}
}
main () {
	cin>>s;
	int n,m,t;
	cin>>n>>m>>t;
	for (int i=0;i<m;i++){
		int a;
		cin>>a;
		accept[a]=true;
	}
	
	for (int i=0;i<n;i++){
		int k;
		cin>>k;
		for (int j=1;j<=k;j++){
			char c;
			int v;
			cin>>c>>v;
			adj[i].pb({v,c});
		}
	}
	solve(s);
	for (int i=0;i<s.length();i++){
		cout<<ans[i];
	}
	cout<<endl;
}

