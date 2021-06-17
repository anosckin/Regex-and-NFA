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
const int DEBUG = false;
struct Relation{
	int u,v;
	char R;
};

Relation R_init(int u,int v,int R){
	Relation temp;
	temp.u=u;
	temp.v=v;
	temp.R=R;
	return temp;
}

struct NFA{
	vector <int> start;
	vector <int> accept_states;
	vector <Relation> relations;
};

int number_of_states;
NFA automata[MAX_SIZE];
vector <pair<int,char> > adj[MAX_SIZE];
vector <pair<int,char> > short_adj[MAX_SIZE];
int accpetable_states[MAX_SIZE];
bool fix[MAX_SIZE];

void print_NFA(NFA answer, bool adj_ok = false);
NFA NFA_init (int start,int accept_state,char R){
	NFA temp;
	temp.start.pb(start);
	temp.accept_states.pb(accept_state);
	temp.relations.pb(R_init(start,accept_state,R));
	return temp;
}

NFA NFA_star(NFA a){
	NFA temp;
	++number_of_states;
	temp.start.pb(number_of_states);
    
	for (int i=0;i<a.relations.size();i++){
		temp.relations.pb(a.relations[i]);
	}
	
	for (int i=0;i<a.accept_states.size();i++){
		temp.accept_states.pb(a.accept_states[i]);
	}
	
	for (int i=0;i<temp.accept_states.size();i++){
		temp.relations.pb(R_init(temp.accept_states[i],number_of_states,'@')); // @ constitutes an epsilon
	}
	for (int i=0;i<a.start.size();i++){
		temp.relations.pb(R_init(number_of_states,a.start[i],'@')); // @ constitutes an epsilon
	}
	
    temp.accept_states.pb(number_of_states);
    
	return temp;
}

NFA NFA_concat(NFA a,NFA b){
	NFA temp;
	for (int i=0;i<a.start.size();i++){	
     	temp.start.pb(a.start[i]);
    }
    
	for (int i=0;i<b.accept_states.size();i++){
		temp.accept_states.pb(b.accept_states[i]);
	}
	
	for (int i=0;i<a.relations.size();i++){
		temp.relations.pb(a.relations[i]);
	}
	for (int i=0;i<b.relations.size();i++){
		temp.relations.pb(b.relations[i]);
	}
	
	for (int i=0;i<a.accept_states.size();i++){
		for (int j=0;j<b.start.size();j++){
			temp.relations.pb(R_init(a.accept_states[i],b.start[j],'@')); // @ constitues an epsilon
	    }
	}
	
	return temp;
}

NFA NFA_union(NFA a,NFA b){
	NFA temp;
	
	for (int i=0;i<a.start.size();i++){	
     	temp.start.pb(a.start[i]);
    }
    
	for (int i=0;i<b.start.size();i++){	
     	temp.start.pb(b.start[i]);
    }
    
	for (int i=0;i<a.accept_states.size();i++){
		temp.accept_states.pb(a.accept_states[i]);
	}
	
	for (int i=0;i<b.accept_states.size();i++){
		temp.accept_states.pb(b.accept_states[i]);
	}
	
	for (int i=0;i<a.relations.size();i++){
		temp.relations.pb(a.relations[i]);
	}
	for (int i=0;i<b.relations.size();i++){
		temp.relations.pb(b.relations[i]);
	}
	
	return temp;
}


//checks if char is a symbol
bool check_symbol (char c){
	return (('0'<=c and c<='9') or ('a'<=c and c<='z') or c=='@');
}
// adds '@' where there is epsilon implied
void add_epsilon(string &s){
	for (int i=1;i<s.size();i++){
		if (s[i-1]=='(' and s[i]==')'){
			s=s.substr(0,i)+'@'+s.substr(i);
		}
	}
}
// adds '.' wher concat is implied
void add_concats(string &s){
	for (int i=1;i<s.size();i++){
		if (check_symbol(s[i]) || s[i]=='('){
			if (s[i-1]!='|'&&s[i-1]!='.'&&s[i-1]!='('){
				s=s.substr(0,i)+'.'+s.substr(i);
			}
		}
	}
}
//converts string which is in infix to postfix
void string_to_postfix(string &s){
	vector <char> postfix;
	stack <pair <char,int> > st;	
	for (int i=0;i<s.length();i++){
		if (check_symbol(s[i])){
			postfix.pb(s[i]);
			continue;
		}
		if (s[i]=='('){
			st.push({'(',0});
			continue;
		}
		if (s[i]==')'){
			while (st.top().first!='('){
				postfix.pb(st.top().first);
				st.pop();
			}
			if (st.size()) st.pop();
			continue;
		}
		
		pair <char,int> operation;
		operation.first=s[i];
		if (s[i]=='|') operation.second=1;
		if (s[i]=='.') operation.second=2;
		if (s[i]=='*') operation.second=3;
		while (st.size()>0&&st.top().second>=operation.second){
			postfix.pb(st.top().first);
			st.pop();
		}
		st.push(operation);
	}
	
	while (st.size()>0){
		postfix.pb(st.top().first);
		st.pop();
	}
	
	string ans="";
	for (int i=0;i<postfix.size();i++){
		ans+=postfix[i];
	}
	
	s=ans;
}
//gets string ready for converting to NFA
void get_string_ready(string &s){
	add_epsilon(s);
	add_concats(s);
	string_to_postfix(s);
	for (int i=0;i<s.size();i++){
		if (check_symbol(s[i])){
			automata[i] = NFA_init(++number_of_states,++number_of_states,s[i]);
		}
	}
}
//converts string to working NFA and returns it
NFA convert_to_NFA (string &s){
	stack <NFA> st;
	for (int i=0;i<s.size();i++){
		if (check_symbol(s[i])){
			st.push(automata[i]); 
			//print_NFA(automata[i]);
			//pause;
		}else{
			//deb(s[i]);
			NFA cur,cur1,cur2;
			if (s[i]=='.'){
				cur1 = st.top();
				st.pop();
				cur2 = st.top();
				st.pop();
				cur = NFA_concat(cur2,cur1); // the order is important
				st.push(cur);
				//print_NFA(cur);
				//pause;
			}
			if (s[i]=='|'){
				cur1 = st.top();
				st.pop();
				cur2 = st.top();
				st.pop();
				cur = NFA_union(cur1,cur2);
				st.push(cur);
				//print_NFA(cur);
				//pause;
			}
			if (s[i]=='*'){
				cur = st.top();
				st.pop();
				cur = NFA_star(cur);
				st.push(cur);
				//print_NFA(cur);
				//pause;
			}
		}
	}
	NFA cur = st.top();
	for (int i=0;i<cur.start.size();i++){
		cur.relations.pb(R_init(0,cur.start[i],'@'));
	}
	
	cur.start.clear();
	cur.start.pb(0);
	
	return cur;
}
//pritns every NFA info
void print_NFA(NFA answer,bool adj_ok){
	if (!DEBUG) return;
	cout<<"START: "<<endl;
	for (int i=0;i<answer.start.size();i++){
		cout<<answer.start[i]<<" ";
	}
	cout<<endl;
	cout<<"ACCEPT: "<<endl;
	for (int i=0;i<answer.accept_states.size();i++){
		cout<<answer.accept_states[i]<<" ";
	}
	cout<<endl;
	cout<<"Relations: "<<endl;
	for (int i=0;i<answer.relations.size();i++){
		cout<<answer.relations[i].u<<" "<<answer.relations[i].v<<" "<<answer.relations[i].R<<endl;
	}
	
	if (adj_ok){
		cout<<"ADJ: "<<endl;
		for (int e=0;e<=number_of_states;e++){
			for (int i=0;i<short_adj[e].size();i++){
				cout<<e<<" "<<short_adj[e][i].f<<" "<<short_adj[e][i].s<<endl;
			}	
		}
	
	}
}

//creates adj vector and fills array acceptalbe_states
void create_adj_vector (NFA answer){
	for (int i=0;i<=number_of_states;i++){
		adj[i].clear();
	}
	for (int i=0;i<answer.relations.size();i++){
		Relation road = answer.relations[i];
		adj[road.u].pb({road.v,road.R});
	}

	for (int i=0;i<answer.accept_states.size();i++){
		accpetable_states[answer.accept_states[i]]=1;
	}
}

void go (int u){
	fix[u]=true;
	for (int i=0;i<short_adj[u].size();i++){
		if (!fix[short_adj[u][i].f]){
			go(short_adj[u][i].f);
		}
	}
}

void remove_epsilons (NFA &answer){
	vector <Relation> eps;
	
	//pause;
	for (int e=0;e<=number_of_states;e++){
		eps.clear();
		for (int e=0;e<=number_of_states;e++){
			for (int i=0;i<adj[e].size();i++){
				if (adj[e][i].s=='@') eps.pb(R_init(e,adj[e][i].f,'@'));
			}	
		}
		for (auto road:eps){
			vector <pair<int,char> > tempor;
			tempor.clear();
			if (accpetable_states[road.v]==1) {
				accpetable_states[road.u]=1;
			}
			
			for (int i=0;i<adj[road.v].size();i++){
				tempor.pb(adj[road.v][i]);
			}
			
			for (int i=0;i<tempor.size();i++){
				bool ok = true;
				for (int j=0;j<adj[road.u].size();j++){
					if (adj[road.u][j]==tempor[i]) ok = false;
				}
				if (ok) {
					adj[road.u].pb(tempor[i]);
				}
			}
			
		}	
	}
	//pause;
	for (int e=0;e<=number_of_states;e++){
		for (int i=0;i<adj[e].size();i++){
			if (adj[e][i].s!='@') short_adj[e].pb(adj[e][i]);
		}	
	}
	answer.accept_states.clear();
	for (int i=0;i<=number_of_states;i++){
		if (accpetable_states[i]==1){
			answer.accept_states.pb(i);
		}
	}
}

void shorten_NFA(NFA &answer){
	for (int i=0;i<answer.start.size();i++){
		go(answer.start[i]);
	}
	
	for (int i=0;i<answer.start.size();i++){
		if (!fix[answer.start[i]]) {
			answer.start.erase(answer.start.begin()+i);
			i--;
		}
	}
	
	for (int i=0;i<answer.accept_states.size();i++){
		if (!fix[answer.accept_states[i]]){
			answer.accept_states.erase(answer.accept_states.begin()+i);
			i--;
		}
	}
	for (int e=0;e<=number_of_states;e++){
		if (!fix[e]) short_adj[e].clear();
		for (int i=0;i<short_adj[e].size();i++){
			if (!fix[short_adj[e][i].f]){
				short_adj[e].erase(short_adj[e].begin()+i);
				i--;
			}
		}
	}
	
}
map <int,int> mp;
void print_answer(NFA &answer){
	
	int n=0,m=0;
	for (int i=0;i<=number_of_states;i++){
		if (fix[i]){
			mp[i]=n;
			n++;
		}
	}
	for (int e=0;e<=number_of_states;e++){
		for (int i=0;i<short_adj[e].size();i++){
			m++;
		}	
	}
	cout<<n<<" "<<answer.accept_states.size()<<" "<<m<<endl;
	for (int i=0;i<answer.accept_states.size();i++){
		cout<<mp[answer.accept_states[i]]<<" ";
	}
	for (int e=0;e<=number_of_states;e++){
		int cur=mp[e];
		if (!fix[e]){
			continue;
		}
		cout<<endl;
		cout<<short_adj[e].size()<<" ";
		for (int i=0;i<short_adj[e].size();i++){
			cout<<short_adj[e][i].s<<" "<<mp[short_adj[e][i].f]<<" ";
		}	
	}
	cout<<endl;
}

int main () {
	string s;
	cin>>s;
	get_string_ready(s);
	//deb(s);
	NFA answer=convert_to_NFA(s);
	create_adj_vector (answer);
	remove_epsilons(answer);
	//pause;
	shorten_NFA(answer);
	//print_NFA(answer,true);
	print_answer(answer);
}
// (l10)((an3n|yj|ca8|n)*)




