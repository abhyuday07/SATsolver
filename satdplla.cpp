//Copyright 2018 Pandey A.
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
ofstream fp1;
set<int> currassgn;
/*solve function recursively solves the problem
using semantic tableaux but instead stops the propogation
once a contradiction is reached. If p is included ever in
the track then ~p is never included. The check is O(1).*/
pair<int,int> isempty(vector<set<int>> query){
	pair<int,int> check;
	check.first=0;
	check.second=0;
	int all=1;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		if(it->size()==0){ check.first=1;}
		else all=0;
		if(it->size()==1){ check.second=*((*it).begin());}
	}
	//cout<<check.first<<"."<<check.second<<endl;
	if(all==1) check.first=-1;
	return check;
}
vector<set<int>> eliminateall(vector<set<int>> query, int rem){
	vector<set<int>> query1;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		query1.push_back(*it);
	}
	// int a=0;
	// fp1<<"------------23456789-------------\n";
	// for(auto it=query.begin();it!=query.end();it++){
	// 	for(auto it1=it->begin();it1!=it->end();it1++){
	// 		fp1<<*it1<<" ";
	// 	}
	// 	fp1<<endl;
	// }
	// fp1<<"----------------------------\n";
	query1.erase(remove_if(query1.begin(),query1.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query1.end());
	for(std::vector<set<int>>::iterator it=query1.begin();it!=query1.end();it++){
		set<int>::iterator it2 = it->find(-rem);
			if(it2!=(it->end())) it->erase(it2);
	}
	// fp1<<"------------234567err9-------------\n";
	// for(auto it=query1.begin();it!=query1.end();it++){
	// 	for(auto it1=it->begin();it1!=it->end();it1++){
	// 		fp1<<*it1<<" ";
	// 	}
	// 	fp1<<endl;
	// }
	// fp1<<"----------------------------\n";
	return query1;
}
int solve(vector<set<int>> query){
	cnt++;
	pair<int,int> check=isempty(query);
	//cout<<check.first<<"."<<check.second<<endl;
	if(check.first==1){
		//cout<<"unsat"<<endl;
		return 0;
	}
	if(check.first==-1){
		//cout<<"sat"<<endl;
		return 1;
	}
	if(check.second!=0){
		currassgn.insert(check.second);
		vector<set<int>> query1=eliminateall(query,check.second);
		return solve(query1);
	}
	int l=*((query.begin())->begin());
	set<int> base1,base2;
	//cout<<l<<endl;
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
	FILE* fp=fopen("sat.txt","r");
	fp1.open("o.txt");
	int var=0;
	fscanf(fp,"p cnf %d %d",&var,&clauses);
	vector<set<int>> query;
	int temp=0;
	set<int> empty;
	for(int i=0; i<clauses;i++){ query.push_back(empty);}
	//Query input
	for(int i=0; i<clauses;i++){
		while(1){
			fscanf(fp,"%d",&temp);
			if(temp==0) break;
			query[i].insert(temp);
			//cout<<*(query[i].begin())<<endl;
		}
	}
	solve(query);
	cout<<"-------------"<<endl;
	for(auto it1=currassgn.begin();it1!=currassgn.end();it1++){
		cout<<*it1<<" ";
	}
	cout<<"------------------"<<endl;
	cout<<cnt;
	fclose(fp);
	return 0;
}
