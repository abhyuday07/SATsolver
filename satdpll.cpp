//Copyright 2018 Pandey A. Srinjay K.
//Strict dependency on g++ 2017 standard
//Heuristic driven solution to SAT solving (NP)
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include <set>
#include<fstream>
#include<vector>
#include<cstring>
#include<ctime>
#include<algorithm>
#include<thread>
#include<chrono>
using namespace std;
//this variable will count the no of calls made
int cnt=0;
int clauses=0;
int var=0;
//set currassgn will comtain already satisfied literals
set<int> currassgn;
//this fuction will return the return the literal which will have maximum occurence
int max_occ(vector<set<int>> query){
	int pos[var+1],occur[var+1];
	for(int i=0;i<var+1;i++){ occur[i]=0; pos[i]=0; }
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(auto j=it->begin();j!=it->end();j++){  occur[abs(*j)]++; if(*j>0) pos[*j]++; }
	}
	int idx=distance(occur,max_element(occur+1,occur+var+1));
	if(2*pos[idx]>=occur[idx]) return idx;
	else return -idx;
}

//this will check for pure literals
int is_pure(vector<set<int>> query){
	bool arr[2*var+1];
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(set<int>::iterator j=it->begin();j!=it->end();j++){
			if(*j>0) arr[*j]=1;
			else arr[var+abs(*j)]=1;
		}
	}
	for(int i=1;i<=var;i++){
		if((arr[i]&&!arr[var+i])||(!arr[i]&&arr[var+i])) return 1;
	}
	return 0;
}

//this function will eliminate all the pure literals
pair<vector<set<int>>,set<int>> purify(vector<set<int>> query){
	bool arr[2*var+1];
	//query1 will be a copy for query
	vector<set<int>> query1;
	//this set will contain all the pure literals
	set<int> pure;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		query1.push_back(*it);
	}
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(set<int>::iterator j=it->begin();j!=it->end();j++){
			if(*j>0) arr[*j]=1;
			else arr[var+abs(*j)]=1;
		}
	}
	//entering values in the set named pure
	for(int i=1;i<=var;i++){
		if(arr[i]&&!arr[var+i]) pure.insert(i);
		else if(!arr[i]&&arr[var+i]) pure.insert(-i);
	}
	//erasing pure literals from query1
	for(std::set<int>::iterator it=pure.begin(); it!=pure.end();it++){
		int rem=*it;
		query1.erase(remove_if(query1.begin(),query1.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query1.end());
	}
	pair<vector<set<int>>,set<int>> q;
	q.first=query1;
	q.second=pure;
	return q;
}

//this fucnction will check for empty clauses
pair<int,int> isempty(vector<set<int>> query){
	pair<int,int> check;
	check.first=0;
	check.second=0;
	int all=1;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		if(it->size()==0){ check.first=1;}
		all=0;
		if(it->size()==1){ check.second=*((*it).begin());}
	}
	if(all==1) check.first=-1;
	return check;
}

//this function eliminates all the singular clauses i.e. the clauses with only one literal
pair<vector<set<int>>,set<int>> eliminateall(vector<set<int>> query){
	vector<set<int>> query1;
	set<int> singular;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		query1.push_back(*it);
		if(it->size()==1)  singular.insert(*(it->begin()));
	}
	int r=0;
	for(std::set<int>::iterator iter=singular.begin();iter!=singular.end();iter++){
		int rem = *iter;
		//cout<<rem<<',';
		query1.erase(remove_if(query1.begin(),query1.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query1.end());
		for(std::vector<set<int>>::iterator it=query1.begin();it!=query1.end();it++){
			set<int>::iterator it2 = it->find(-rem);
			if(it2!=(it->end())) it->erase(it2),r++;
		}
	}
	pair<vector<set<int>>,set<int>> val;
	val.first=query1;
	val.second=singular;	return val;
}

//this function will solve the clauses and return either SAT(1) or UNSAT(0)
int solve(vector<set<int>> query){
	pair<int,int> check=isempty(query);

	if(check.first==1) return 0;
	if(check.first==-1) return 1;
	cnt++;
	if(check.second!=0){
		pair<vector<set<int>>,set<int>> q=eliminateall(query);
		if(solve(q.first)==1){
			for(std::set<int>::iterator iter=(q.second).begin();iter!=(q.second).end();iter++)
			currassgn.insert(*iter);
			return 1;
		}
		else return 0;
	}
	if(is_pure(query)){
		pair<vector<set<int>>,set<int>> q=purify(query);
		if(solve(q.first)==1){
			for(std::set<int>::iterator iter=(q.second).begin();iter!=(q.second).end();iter++)
			currassgn.insert(*iter);
			return 1;
		}
		else return 0;
	}

	//int l=max_occ(query);
	// for(auto it=(query.begin())->begin();it!=(query.begin())->end();it++)
	// 	cout<<"---"<<*it<<"---";

	// Here we knock out the literal which has maximum occurence  and the recursively solve the problem
	int l=max_occ(query);
	set<int> base1,base2;
	base1.insert(l);
	base2.insert(-l);
	query.push_back(base1);
	if(solve(query)==1){ currassgn.insert(l); return 1;}
	else{
		query.pop_back();
		query.push_back(base2);
		if(solve(query)==1){ currassgn.insert(-l); return 1;}
		else return 0;
	}

}


int main(){
	clock_t start=clock();
	FILE* fp=fopen("sat.txt","r");
	fscanf(fp,"p cnf %d %d\n",&var,&clauses);
	//query is a vector of sets
	vector<set<int>> query;
	int temp=0;
	//this boolean array will store the occurence of both positive and negated literals
	bool arr[2*var+1];
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	set<int> empty;
	for(int i=0; i<clauses;i++) query.push_back(empty);

	//here, we take input to 'query' and also assign the values for boolean array
	for(int i=0; i<clauses;i++){
		while(1){
			fscanf(fp,"%d",&temp);
			if(temp==0) break;
			else if(temp>0) arr[temp]=1;
			else arr[var+abs(temp)]=1;
			query[i].insert(temp);
		}
	}
	//pure literals
	for(int i=1;i<=var;i++){
		if(arr[i]&&!arr[var+i]){ currassgn.insert(i); query.erase(remove_if(query.begin(),query.end(),[&i](set<int> j){return j.find(i)!=j.end();}),query.end());}
		else if(!arr[i]&&arr[var+i]){ currassgn.insert(-i); query.erase(remove_if(query.begin(),query.end(),[&i](set<int> j){return j.find(-i)!=j.end();}),query.end());}
	}
	//remove tautology
	//here we find if any clause has both 'g' and '-g'
	for(int i=1;i<=var;i++){
		for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();){
			if(it->find(i)!=it->end()&&it->find(-i)!=it->end()) it=query.erase(it);
			else it++;
		}
	}

	//this part prints the output of the code
	if(solve(query)){
		cout<<"SATISFIABLE"<<endl;
		cout<<"-----------------"<<endl;
		for(int i=1;i<=var;i++){
			if(currassgn.find(i)!=currassgn.end()) printf("%d ",i);
			else if(currassgn.find(-i)!=currassgn.end()) printf("%d ",-i);
			else printf("%d ",i);
		}
		cout<<"\n------------------"<<endl;
	}
	else cout<<"UNSAT"<<endl;
	cout<<"Number of DP calls: "<<cnt<<endl;
	printf("---------Time(in seconds)=%f--------\n",(double)(clock()-start)/CLOCKS_PER_SEC);
	fclose(fp);
	return 0;
}
