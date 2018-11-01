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
vector<set<int>> eliminateall(vector<set<int>> query, int rem){
	vector<set<int>> query1;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		query1.push_back(*it);
	}
	query1.erase(remove_if(query1.begin(),query1.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query1.end());
	for(std::vector<set<int>>::iterator it=query1.begin();it!=query1.end();it++){
		set<int>::iterator it2 = it->find(-rem);
			if(it2!=(it->end())) it->erase(it2);
	}
	return query1;
}
int solve(vector<set<int>> query){
	pair<int,int> check=isempty(query);

	if(check.first==1){
		return 0;
	}

	if(check.first==-1){
		return 1;
	}
	cnt++;
	if(check.second!=0){
		currassgn.insert(check.second);
		vector<set<int>> query1=eliminateall(query,check.second);
		return solve(query1);
	}

	int l=max_occ(query);
	set<int> base1,base2;
	base1.insert(l);
	base2.insert(-l);
	query.push_back(base1);
	if(solve(query)==1){ currassgn.insert(l); return 1;}
	else{
		query.pop_back();
		query.push_back(base2);
		currassgn.insert(-l); 
		return solve(query);
	}

}
int main(){
	clock_t start=clock();
	FILE* fp=fopen("sat5.txt","r");
	fscanf(fp,"p cnf %d %d",&var,&clauses);
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
	if(solve(query)) cout<<"SAT"<<endl;
	// cout<<max_occ(query,var);
	else cout<<"UNSAT"<<endl;
	cout<<"----------------"<<endl;
	for(auto it1=currassgn.begin();it1!=currassgn.end();it1++){
		cout<<*it1<<" ";
	}
	cout<<"\n------------------"<<endl;
	cout<<cnt<<endl;
	printf("---------%f--------\n",(double)(clock()-start)/CLOCKS_PER_SEC);
	fclose(fp);
	return 0;
}
