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
int cnt=0;
int clauses=0;
int var=0;
set<int> currassgn;
int max_occ(vector<set<int>>& query){
	int pos[var+1],occur[var+1];
	for(int i=0;i<var+1;i++){ occur[i]=0; pos[i]=0; }
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(auto j=it->begin();j!=it->end();j++){  occur[abs(*j)]++; if(*j>0) pos[*j]++; }
	}
	int idx=distance(occur,max_element(occur+1,occur+var+1));
	if(2*pos[idx]>=occur[idx]) return idx;
	else return -idx;
}
int is_pure(vector<set<int>>& query){
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
set<int> purify(vector<set<int>>& query){
	bool arr[2*var+1];
	set<int> pure;
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(set<int>::iterator j=it->begin();j!=it->end();j++){
			if(*j>0) arr[*j]=1;
			else arr[var+abs(*j)]=1; 
		}
	}
	for(int i=1;i<=var;i++){
		if(arr[i]&&!arr[var+i]) pure.insert(i);
		else if(!arr[i]&&arr[var+i]) pure.insert(-i);
	}
	for(std::set<int>::iterator it=pure.begin(); it!=pure.end();it++){
		int rem=*it;
		query.erase(remove_if(query.begin(),query.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query.end());
	}
	return pure;
}
pair<int,int> isempty(vector<set<int>>& query){
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
set<int> eliminateall(vector<set<int>>& query){
	set<int> singular;
	while((isempty(query)).second!=0){
		for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
				if(it->size()==1)  singular.insert(*(it->begin()));
		}
		for(std::set<int>::iterator iter=singular.begin();iter!=singular.end();iter++){
			//cout<<r;
		int rem = *iter;
		query.erase(remove_if(query.begin(),query.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query.end());
			for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){ 
				set<int>::iterator it2=it->find(-rem); 
				if((it2)!=(it->end())) it->erase(it2);
			}
		}
	}
	return singular;
}
int solve(vector<set<int>>& query){
	pair<int,int> check=isempty(query);

	if(check.first==1) return 0;
	if(check.first==-1) return 1;
	cnt++;
	if(check.second!=0){
		vector<set<int>> query1;
		query1=query;
		set<int> q=eliminateall(query1);
		if(solve(query1)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
			return 1;
		}
		else return 0;
	}
	if(is_pure(query)){
		vector<set<int>> query1;
		query1=query;
		set<int> q=purify(query1);
		if(solve(query1)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++)
			currassgn.insert(*iter); 
			return 1;
		}
		else return 0;
	}
	int l=max_occ(query);
	//int l=*((query.begin())->begin());
	//for(auto it=(query.begin())->begin();it!=(query.begin())->end();it++)
	//cout<<"---"<<*it<<"---";
	//int l=max_occ(query);
	set<int> s1,s2;
	s1.insert(l);
	s2.insert(-l);
	//int l=*((query.begin())->begin());
	vector<set<int>> query1;
	query1=query;
	query1.push_back(s1);
	set<int> q=eliminateall(query1);
	if(solve(query1)==1){
		for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
		return 1;
	}
	else{
		//query1=query;
		query.push_back(s2);
		q=eliminateall(query);
		if(solve(query1)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
			return 1;
		}	
		return 0;
	}
}
int main(){
	clock_t start=clock();
	FILE* fp=fopen("sat.txt","r");
	fscanf(fp,"p cnf %d %d\n",&var,&clauses);
	vector<set<int>> query;
	int temp=0;
	bool arr[2*var+1];
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	set<int> empty;
	for(int i=0; i<clauses;i++) query.push_back(empty);
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
	for(int i=1;i<=var;i++){
		for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();){
			if(it->find(i)!=it->end()&&it->find(-i)!=it->end()) it=query.erase(it);
			else it++;
		}
	}
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