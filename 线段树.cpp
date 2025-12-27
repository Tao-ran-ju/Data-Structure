#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll N=1e5+10;
ll n,m,a[N],tree[N*4],tag[N*4]; 
//a[N]用于记录数列的元素，从a[1]开始
//tree[i]表示第i个结点的值，表示一个线段区间的值，如最值、区间和等（本题目中是区间和）
//tag[i]为第i个结点的Lazy-Tag，统一记录这个区间的修改
void push_up(ll p){
	tree[p]=tree[p*2]+tree[p*2+1];
}

void build(ll p,ll l,ll r){ //建树。p为结点编号，它指向区间[pl,pr]
	tag[p]=0;  //Lazy-Tag标记初始化为0
	if(l==r){  //最底层的叶子，赋值
		tree[p]=a[l];
		return ;
	}
	ll mid=(l+r)/2;  
	build(p*2,l,mid);  //建立左子树
	build(p*2+1,mid+1,r);  //建立右子树
	push_up(p);  //递归完成后回溯，更新结点p的值
}

void addtag(ll p,ll l,ll r,ll d){  //给结点p打上tag标记，并更新tree
	tag[p]+=d;  //打上tag标记
	tree[p]+=(r-l+1)*d;  //计算新的tree
}

void push_down(ll p,ll l,ll r){  //不能覆盖时，把tag传递给子树
	if(tag[p]){  //假如有不为零的tag标记（通过之前某次做区间修改时留下的）
		ll mid=(l+r)/2;
		addtag(p*2,l,mid,tag[p]);  //给左子树打上tag标记
		addtag(p*2+1,mid+1,r,tag[p]);  //给右子树打上tag标记
		tag[p]=0;  //结点p自己的tag标记已经传递给左右子树了，重置为0
	}
}

void update(ll L,ll R,ll p,ll l,ll r,ll d){  //区间修改：[L,R]内的每个元素都加上d
	if(L<=l&&R>=r){  //如果能完全覆盖，给结点p打上标记后return即可
		addtag(p,l,r,d);  
		return ;
	}
	push_down(p,l,r);  //如果不能覆盖，则递归左右子树并打上tag的过程可能会破坏结点p对应线段区间原有的一致性
	ll mid=(l+r)/2;
	if(L<=mid)update(L,R,p*2,l,mid,d);  //递归左子树
	if(R>mid)update(L,R,p*2+1,mid+1,r,d);  //递归右子树
	push_up(p);  //递归完成后回溯，更新结点p的值
}

ll query(ll L,ll R,ll p,ll l,ll r){  //查询区间为[L,R]，目前遍历到的区间为[l,r]
	if(L<=l&&R>=r)return tree[p];  //如果能完全覆盖，直接返回结点p的值
	push_down(p,l,r);  //如果不能直接覆盖，则需要将结点p的tag传递给左右子树，之后再遍历左右子树
	ll mid=(l+r)/2;
	ll ans=0;
	if(L<=mid)ans+=query(L,R,p*2,l,mid);  //左子节点有重叠
	if(R>mid)ans+=query(L,R,p*2+1,mid+1,r);  //右子节点有重叠
	return ans;
}

int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++)cin>>a[i];
	build(1,1,n);  //建树
	
	while(m--){
		ll op,L,R,d; 
		cin>>op;
		if(op==1){  //operation 1：区间修改，[L,R]区间内每个元素都加上d
			cin>>L>>R>>d;
			update(L,R,1,1,n,d);
		}
		else if(op==2){  //operation 2：区间查询，[L,R]区间内所有元素之和
			cin>>L>>R;
			cout<<query(L,R,1,1,n)<<endl;
		}
	}
	
	return 0;
}
