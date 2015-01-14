#include <cstdio>
#include <vector>
using namespace std;
#define maxn 500
vector <int> link[maxn];
int n;
int ans;
int match[maxn];
int Queue[maxn],head,tail;
int pred[maxn],base[maxn];
bool InQueue[maxn],InBlossom[maxn];
int start,finish;
int newbase;

void push(int u)
{
	Queue[tail++] = u; InQueue[u] = true;
}
int pop()
{
	return Queue[head++];
}

int FindCommonAncestor(int u, int v) {
	bool InPath[maxn];
	for(int i = 0; i <= n; i++)
		InPath[i] = 0;
	while(true) {
		u = base[u];
		InPath[u] = true;
		if(u == start) break;
		u = pred[match[u]];
	}
	while(true) {
		v = base[v];
		if(InPath[v]) break;
		v = pred[match[v]];
	}
	return v;
}

void ResetTrace(int u) {
	int v;
	while(base[u] != newbase) {
		v = match[u];
		InBlossom[base[u]] = InBlossom[base[v]] = true;
		u = pred[v];
		if(base[u] != newbase) pred[u] = v;
	}
}

void BlossomContract(int u, int v){
	newbase = FindCommonAncestor(u, v);
	for(int i = 0; i <= n; i++)
		InBlossom[i] = 0;
	ResetTrace(u), ResetTrace(v);
	if(base[u] != newbase) pred[u] = v;
	if(base[v] != newbase) pred[v] = u;
	for (int i = 0; i <= n; i++)
		if(InBlossom[base[i]]) {
			base[i] = newbase;
			if(!InQueue[i]) push(i);
		}
}

bool FindAugmentingPath(int u) {
	bool found = false;
	for(int i = 0; i <= n; i++) pred[i] = -1, base[i] = i;
	for(int i = 0; i <= n; i++) InQueue[i] = 0;
	start = u; finish = -1;
	head = tail = 0;
	push(start);
	while(head < tail) {
		int u = pop();
		for(int i = link[u].size()-1 ; i >= 0; i--) {
			int v = link[u][i];
			if(base[u] != base[v] && match[u] != v)
			if(v == start || (match[v] >= 0 && pred[match[v]] >= 0))
				BlossomContract(u,v);
			else if(pred[v] == -1) {
				pred[v] = u;
				if(match[v] >= 0)
					push(match[v]);
				else {
					finish = v;
					return true;
				}
			}
		}
	}
	return found;	
}

void AugmentPath() {
	int u, v, w;
	u = finish;
	while(u >= 0) {
		v = pred[u];
		w = match[v];
		match[v] = u;
		match[u] = v;
		u = w;
	}
}

void FindMaxmatching() {
	for(int i = 0; i <= n; i++) match[i] = -1;
	for(int i = 0; i <= n; i++)
		if(match[i] == -1 && FindAugmentingPath(i))
		{
			ans+=2;
			AugmentPath();
		}
}

int main()
{
	//freopen("data.in","r",stdin);
	scanf("%d",&n);
	int x,y;
	ans = 0;
	while(scanf("%d %d",&x,&y)==2)
	{
		link[x].push_back(y);
		link[y].push_back(x);
	}
	FindMaxmatching();
	printf("%d\n",ans);
	for(int i = 1; i <= n; i++)
		if(i < match[i])
			printf("%d %d\n",i,match[i]);
	return 0;
}