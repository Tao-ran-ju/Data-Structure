#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll N=1e5+10;
ll tree[N*4],n,m,mod;
ll w[N],Mul[N*4],Add[N*4];

void push_up(ll p){
	tree[p]=(tree[p*2]+tree[p*2+1])%mod;
}

void addtag(ll p,ll l,ll r,ll mul,ll add){
	tree[p]=(tree[p]*mul+add*(r-l+1))%mod;
	Mul[p]=Mul[p]*mul%mod;
	Add[p]=(Add[p]*mul+add)%mod;
}

void push_down(ll p,ll l,ll r){
	ll mid=(l+r)/2;
	addtag(p*2,l,mid,Mul[p],Add[p]);
	addtag(p*2+1,mid+1,r,Mul[p],Add[p]);
	Add[p]=0,Mul[p]=1;
}

void build(ll p,ll l,ll r){
	Mul[p]=1;
	if(l==r){
		tree[p]=w[l]%mod;
		return ;
	}
	ll mid=(l+r)/2;
	build(p*2,l,mid);
	build(p*2+1,mid+1,r);
	push_up(p);
}

void update(ll p,ll l,ll r,ll L,ll R,ll mul,ll add){
	if(L<=l&&R>=r){
		addtag(p,l,r,mul,add);
		return ;
	}
	push_down(p,l,r);
	ll mid=(l+r)/2;
	if(L<=mid)update(p*2,l,mid,L,R,mul,add);
	if(R>mid)update(p*2+1,mid+1,r,L,R,mul,add);
	push_up(p);
}

ll query(ll p,ll l,ll r,ll L,ll R){
	if(L<=l&&R>=r){
		return tree[p];
	}
	ll res=0;
	push_down(p,l,r);
	ll mid=(l+r)/2;
	if(L<=mid)res=(res+query(p*2,l,mid,L,R))%mod;
	if(R>mid)res=(res+query(p*2+1,mid+1,r,L,R))%mod;
	return res%mod;
}

int main(){
	
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	memset(Mul,1,sizeof Mul);
	cin>>n>>m>>mod;
	for(int i=1;i<=n;i++)cin>>w[i];
	build(1,1,n);
	while(m--){
		ll op,x,y,k;
		cin>>op;
		if(op==1){
			cin>>x>>y>>k;
			update(1,1,n,x,y,k,0);
		}
		else if(op==2){
			cin>>x>>y>>k;
			update(1,1,n,x,y,1,k);
		}
		else if(op==3){
			cin>>x>>y;
			cout<<query(1,1,n,x,y)%mod<<'\n';
		}
	}
	
	return 0;
}

