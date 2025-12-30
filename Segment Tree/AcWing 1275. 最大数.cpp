#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+10;
struct Tree{
	int l,r,v;
}tree[N*4];

int m,p,num;
char op[2];

void push_up(int u){
	tree[u].v=max(tree[u*2].v,tree[u*2+1].v);
}

void build(int u,int l,int r){
	tree[u].l=l;
	tree[u].r=r;
	
	if(l==r)return;
	
	int mid=(l+r)/2;
	build(u*2,l,mid);
	build(u*2+1,mid+1,r);
	
}

void modify(int u,int x,int v){
	if(tree[u].l==tree[u].r)tree[u].v=v;
	else{
		int mid=(tree[u].l+tree[u].r)/2;
		if(mid>=x)modify(u*2,x,v);
		else modify(u*2+1,x,v);
		push_up(u);
	}
}

int query(int u,int l,int r){
	if(tree[u].l>=l&&tree[u].r<=r)return tree[u].v;
	int mid=(tree[u].l+tree[u].r)/2;
	int res=0;
	if(l<=mid)res=query(u*2,l,r);
	if(r>mid)res=max(res,query(u*2+1,l,r));
	
	return res;
}
int main(){
	cin>>m>>p;
	int last=0,n=0;
	
	build(1,1,m);
	
	while(m--){
		scanf("%s%d",op,&num);
		
		if(*op=='A'){
			modify(1,n+1,(num+(long long)last)%p);
			n++;
		}
		else if(*op=='Q'){
			last=query(1,n-num+1,n);
			cout<<last<<endl;
			
		}
	}
	
	return 0;
}

