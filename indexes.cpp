#include <bits/stdc++.h>

using namespace std;

//Since any indexing scheme will need to have a collection of data blocks, and an unoptimized search, this is a superclass
//which accomplishes such functionality. All subclasses will have to implement a buildIndex function for construction of index 
//and search function for optimized search
class Index{
	protected:
		vector<vector<int>> data;//Data blocks. Each block is a vector<int>, with an int stored for each record
		int datas;
	public:
		Index(int blksize){
			datas=blksize/sizeof(int);
			data=vector<vector<int>>();
		}
		void insert(int x){
			if(data.size()==0){//If there are no data blocks
				//Add first data block
				data.push_back(vector<int>());
				auto p=data.begin();
				p->push_back(x);
			}else{
				auto p=data.end()-1;
				if(p->size()==datas){//If data block has been filled
					//Add a new data block
					data.push_back(vector<int>());
					auto p1=data.end()-1;
					p1->push_back(x);
				}else{//Data block has not been filled
					p->push_back(x);
				}
			}
		}
		pair<bool,int> unoptimizedsearch(int x){//Returns success of search followed by number of block access
			int blkaccesses=0;
			for(auto a:data){
				blkaccesses++;
				for(auto b:a){
					if(b==x){
						return make_pair(true,blkaccesses);
					}
				}
			}
			return make_pair(false,blkaccesses);
		}
		long long noofblocks(){
			return data.size();
		}
		virtual void buildIndex() =0;
		virtual pair<bool,int> search(int x) =0;
		virtual void printstats() =0;
};

class PrimaryIndex: public Index{ //Works for both ordering-key and ordering-non-key(Just so happens to)
	protected:
		//Index blocks. Each block is a vector<pair<int,int>>>, with each record containing the key and the data block number
		vector<vector<pair<int,int>>> index;
		
		int indexs;//Max number of elements in a data block and index block
	
	public:
		PrimaryIndex(int blksize):Index(blksize){
			indexs=blksize/(sizeof(int)+sizeof(int));
			index=vector<vector<pair<int,int>>>();
		}

		void buildIndex(){
			int blk=0;
			for(auto block:data){
				auto x=block[0];
				if(index.size()==0){
					//Add first index block
					index.push_back(vector<pair<int,int>>());
					auto q=index.begin();
					q->push_back(make_pair(x,0));
				}else{
					auto q=index.end()-1;
					if(q->size()==indexs){//If index block is filled
						//Add a new index block
						index.push_back(vector<pair<int,int>>());
						auto q1=index.end()-1;
						q1->push_back(make_pair(x,blk));	
					}else{//Index block is not filled
						q->push_back(make_pair(x,blk));
					}	
				}
				blk++;
			}
		}

		pair<bool,int> search(int x){//Returns success of search followed by number of block access
			int u=index.size()-1;
			int l=0,m;
			int blkaccesses=0;
			while(l<=u){//Performs binary search to determine which index block's range the key falls in
				m=(l+u)/2;
				auto p=index[m];
				blkaccesses++;
				if(x>=p.front().first&&(m+1>=index.size()||(m+1<index.size()&&x<index[m+1].front().first))){//Checks if key is in range of index block
					int prev=p.front().first;
					int id=p.front().second;
					for(int i=1;i<p.size();i++){//Determines which data block to refer to. id contains the data block to refer to
						if(x>=prev&&x<p[i].first){
							id=p[i-1].second;
							break;
						}
						if(i==p.size()-1){
							id=p[i].second;		
						}
					}
					
					for(auto a:data[id]){//Searches within the data block
						if(a==x){
							return make_pair(true,blkaccesses);//Search within data block is a success
						}
					}
					return make_pair(false,blkaccesses);
				}else if(x>p.back().first){
					l=m+1;
				}else{
					u=m-1;
				}
			}
			return make_pair(false,blkaccesses);
		}

		void printstats(){
			cout<<Index::noofblocks()<<" "<<index.size()<<endl;
		}

		long long noofblocks(){
			return Index::noofblocks()+index.size();
		}
};

class SecondaryIndex: public Index{
	protected:
		//Index blocks. Each block is a vector<pair<int,int>>>, with each record containing the key and the data block number
		vector<vector<pair<int,int>>> index;
		
		int indexs;//Max number of elements in a data block and index block

	public:
		SecondaryIndex(int blksize):Index(blksize){
			indexs=blksize/(sizeof(int)+sizeof(int));
			index=vector<vector<pair<int,int>>>();
		}

		void buildIndex(){
			vector<pair<int,int>> keys;
			int blk=0;
			for(auto a:data){
				for(auto x:a){
					keys.push_back(make_pair(x,blk));
				}
				blk++;
			}
			
			sort(keys.begin(),keys.end());
			
			for(auto a:keys){
				if(index.size()==0){
					//Add first index block
					index.push_back(vector<pair<int,int>>());
					auto q=index.begin();
					q->push_back(a);
				}else{
					auto q=index.end()-1;
					if(q->size()==indexs){//If index block is filled
						//Add a new index block
						index.push_back(vector<pair<int,int>>());
						auto q1=index.end()-1;
						q1->push_back(a);
					}else{//Index block is not filled
						q->push_back(a);
					}	
				}
			}
		}

		pair<bool,int> search(int x){
			int u=index.size()-1;
			int l=0,m;
			int blkaccesses=0;
			while(l<=u){//Performs binary search to determine which index block's range the key falls in
				m=(l+u)/2;
				auto p=index[m];
				blkaccesses++;
				if(x>=p.front().first&&(m+1>=index.size()||(m+1<index.size()&&x<index[m+1].front().first))){//Checks if key is in range of index block
					int id=p.front().second;
					for(auto a:p){//Determines which data block to refer to. id contains the data block to refer to
						if(x==a.first){
							id=a.second;
							break;
						}

					}
					
					for(auto a:data[id]){//Searches within the data block
						if(a==x){
							return make_pair(true,blkaccesses);//Search within data block is a success
						}
					}
					return make_pair(false,blkaccesses);
				}else if(x>p.back().first){
					l=m+1;
				}else{
					u=m-1;
				}
			}
			return make_pair(false,blkaccesses);
		}

		void printstats(){
			cout<<Index::noofblocks()<<" "<<index.size()<<endl;
		}

		long long noofblocks(){
			return Index::noofblocks()+index.size();
		}
};

class MultiLevelIndex: public PrimaryIndex{
	private:
		typedef vector<vector<pair<int,int>>> indexblocks;
		vector<indexblocks> ilevels;
		int levels;
	public:
		MultiLevelIndex(int blksize,int levels):PrimaryIndex(blksize){
			this->levels=levels-1;
			ilevels=vector<indexblocks>(this->levels);
		}
		void buildIndex(){
			PrimaryIndex::buildIndex();
			indexblocks *prev=&index;
			indexblocks *curr=&(ilevels[0]);
			for(int l=0;l<levels;l++){
				int blk=0;
				for(auto a:*prev){
					int x=a[0].first;
					if(curr->size()==0){
					//Add first index block
					curr->push_back(vector<pair<int,int>>());
					auto q=curr->begin();
					q->push_back(make_pair(x,blk));
					}else{
						auto q=curr->end()-1;
						if(q->size()==indexs){//If index block is filled
							//Add a new index block
							curr->push_back(vector<pair<int,int>>());
							auto q1=curr->end()-1;
							q1->push_back(make_pair(x,blk));
						}else{//Index block is not filled
							q->push_back(make_pair(x,blk));
						}	
					}
					blk++;
				}
				prev=curr;
				curr=&(ilevels[l+1]);
			}
		}
		pair<bool,int> search(int x){
			indexblocks *b=&(ilevels[levels-1]);

			int u=b->size()-1;
			int l=0,m;
			int blkaccesses=0;
			//cout<<"Performing binary search on first level of index blocks"<<endl;
			while(l<=u){//Performs binary search to determine which index block's range the key falls in
				m=(l+u)/2;
				auto p=(*b)[m];
				blkaccesses++;
				//cout<<"Accessing block "<<m<<" at first level of index"<<endl;
				if(x>=p.front().first&&(m+1>=b->size()||(m+1<b->size()&&x<(*b)[m+1].front().first))){//Checks if key is in range of index block
					int prev=p.front().first;
					int id=p.front().second;
					for(int i=1;i<p.size();i++){//Searches within 1st level of index block
						if(x>=prev&&x<p[i].first){
							id=p[i-1].second;
							break;
						}
						if(i==p.size()-1){
							id=p[i].second;		
						}
					}
					
					//cout<<"Binary search is first level of index is done"<<endl;
					
					for(int i=levels-2;i>=0;i--){
						auto q=ilevels[i][id];
						blkaccesses++;
						//cout<<"Accessing block "<<id<<" at next level of index"<<endl;
						for(int j=0;j<q.size();j++){
							if(x>=q[j].first&&(j+1>=q.size()||(j+1<q.size()&&x<q[j+1].first))){
								id=q[j].second;
								break;
							}
							if(j==q.size()-1){
								id=q[j].second;
							}
						}
					}
					
					blkaccesses++;
					//cout<<"Accessing block "<<id<<" at last level of index"<<endl;
					auto q=index[id];
					for(int i=0;i<q.size();i++){
						if(x>=q[i].first&&(i+1>=q.size()||(i+1<q.size()&&x<q[i+1].first))){
								id=q[i].second;
								break;
						}
					}

					blkaccesses++;
					//cout<<"Accessing data block "<<id<<endl;
					for(auto a:data[id]){//Searches within the data block
						if(a==x){
							//cout<<"Found in data block"<<endl;
							return make_pair(true,blkaccesses);//Search within data block is a success
						}
					}

					//cout<<"Not found at data block"<<endl;
					return make_pair(false,blkaccesses);

				}else if(x>p.back().first){
					l=m+1;
				}else{
					u=m-1;
				}
			}
			//cout<<"Not found in first level of index block"<<endl;
			return make_pair(false,blkaccesses);
		}
		void printstats(){
			cout<<Index::noofblocks()<<" "<<PrimaryIndex::noofblocks()-Index::noofblocks()<<" ";
			for(int i=0;i<levels;i++){
				cout<<ilevels[i].size()<<" ";
			}
			cout<<endl;
		}
		long long noofblocks(){
			long long ans=0;
			for(int i=0;i<levels;i++){
				ans+=ilevels[i].size();
			}

			ans+=PrimaryIndex::noofblocks();
			return ans;
		}
};

void insert(Index &t,int n){
	for(int i=0;i<n;i++){
		t.insert(i);
	}
}

void insertnonordered(Index &t,int n,int range=10000){
	for(int i=0;i<n;i++){
		int x=rand()%range;
		t.insert(x);
	}
}

void main1(){
	int blksize=128;
	int noofrecords=1;
	for(int k=0;k<9;k++){
		noofrecords*=10;
		PrimaryIndex t(blksize);
		insert(t,noofrecords);
		int n=rand()%1000;
		double avg=0,avgu=0;
		for(int i=0;i<n;i++){
			int x=rand()%noofrecords;
			avg+=t.search(x).second;
			avgu+=t.unoptimizedsearch(x).second;
		}
		avg=avg/n;
		avgu=avgu/n;
		cout<<noofrecords<<" ";
		t.printstats();
		cout<<avg<<" "<<avgu<<endl;
	}
}

void main2(){
	int blksize=128;
	int noofrecords=100000;
	PrimaryIndex t(blksize);
	insert(t,noofrecords);
	double avg=0,avgu=0;
	int n=1000;
	for(int i=0;i<n;i++){
		int x=rand	()%noofrecords;
		avg+=t.search(x).second;
		avgu+=t.unoptimizedsearch(x).second;
	}
	avg=avg/n;
	avgu=avgu/n;
	cout<<noofrecords<<" ";
	t.printstats();
	cout<<avg<<" "<<avgu<<endl;
}

double avgNoofAccesses(Index &p,int records,int samples,bool optimized,bool ordered){
	if(ordered){
		insert(p,records);
	}else{
		insertnonordered(p,records);
	}
	p.buildIndex();
	long sum=0;
	for(int i=0;i<samples;i++){
		int x=rand()%(10*records);
		if(optimized){
			sum+=p.search(x).second;
		}else{
			sum+=p.unoptimizedsearch(x).second;
		}
	}
	return ((double)sum)/samples;
}

void main3(){
	/*
	cout<<"PrimaryIndex without optimized search"<<endl;
	for(int records=1;records<=100000;records*=10){
		PrimaryIndex p(64);
		cout<<records<<" "<<avgNoofAccesses(p,records,100000,false,true)<<" "<<p.Index::noofblocks()<<endl;
	}

	cout<<"PrimaryIndex with optimized search"<<endl;
	for(int records=1;records<=100000;records*=10){
		PrimaryIndex p(64);
		cout<<records<<" "<<avgNoofAccesses(p,records,100000,true,true)<<" "<<p.noofblocks()<<endl;
	}

	cout<<"MultiLevelIndex with optimized search"<<endl;
	for(int levels=2;levels<=5;levels++){
		for(int records=1;records<=100000;records*=10){
			MultiLevelIndex p(64,levels);
			cout<<levels<<" "<<records<<" "<<avgNoofAccesses(p,records,100000,true,true)<<" "<<p.noofblocks()<<endl;
		}
	}
	*/
	///*
	cout<<"SecondaryIndex without optimized search"<<endl;
	for(int records=1;records<=100000;records*=10){
		SecondaryIndex p(64);
		cout<<records<<" "<<avgNoofAccesses(p,records,100000,false,false)<<" "<<p.Index::noofblocks()<<endl;
	}
	cout<<"SecondaryIndex with optimized search"<<endl;
	for(int records=1;records<=100000;records*=10){
		SecondaryIndex p(64);
		cout<<records<<" "<<avgNoofAccesses(p,records,100000,true,false)<<" "<<p.noofblocks()<<endl;
	}
	//*/
}

void main4(){
	MultiLevelIndex t(32, 4);
	insert(t,100000);
	t.buildIndex();
	while(true){
		int n;
		cin>>n;
		t.search(n);
	}
}

int main(){
	srand(time(NULL));
	//main1();
	//main2();
	main4();
	return 0;
}