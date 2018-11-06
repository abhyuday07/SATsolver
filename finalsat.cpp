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
//this variable will count the no of calls made to dp
int cnt=0;
int clauses=0;
int var=0;
//set currassgn will contain satisfied literals
set<int> currassgn;

//this fuction will return the return the literal which will have maximum occurence
int max_occ(vector<set<int>>& query){
	//the array pos contains the occurence of positive terms and occur contains occurence of all the terms
	int pos[var+1],occur[var+1];
	//initialising both the terms
	for(int i=0;i<var+1;i++){ occur[i]=0; pos[i]=0; }
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		//iterating through the query and storing the occurrence
		for(auto j=it->begin();j!=it->end();j++){  occur[abs(*j)]++; if(*j>0) pos[*j]++; }
	}
	int idx=distance(occur,max_element(occur+1,occur+var+1));
	if(2*pos[idx]>=occur[idx]) return idx;
	else return -idx;
}


//this will check for pure literals
int is_pure(vector<set<int>>& query){
	// the first half of the bool array will deal with positive literals and second half will deal with negative literals
	bool arr[2*var+1];
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(set<int>::iterator j=it->begin();j!=it->end();j++){
			if(*j>0) arr[*j]=1;
			else arr[var+abs(*j)]=1;
		}
	}
	for(int i=1;i<=var;i++){
		//check for pure literals
		if((arr[i]&&!arr[var+i])||(!arr[i]&&arr[var+i])) return 1;
	}
	return 0;
}


//this function will eliminate all the pure literals
set<int> purify(vector<set<int>>& query){
	// the first half of the bool array will deal with positive literals and second half will deal with negative literals
	bool arr[2*var+1];
	//this set will contain all the pure literals
	set<int> pure;
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		for(set<int>::iterator j=it->begin();j!=it->end();j++){
			if(*j>0) arr[*j]=1;
			else arr[var+abs(*j)]=1;
		}
	}
	//entering values in the set named pure
	for(int i=1;i<=var;i++){
		//only positive literals exist
		if(arr[i]&&!arr[var+i]) pure.insert(i);
		//only neagted literals exist
		else if(!arr[i]&&arr[var+i]) pure.insert(-i);
	}
	//erasing pure literals from query
	for(std::set<int>::iterator it=pure.begin(); it!=pure.end();it++){
		int rem=*it;
		query.erase(remove_if(query.begin(),query.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query.end());
	}
	return pure;
}


//this function will check for empty clauses
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


//this function eliminates all the singular clauses i.e. the clauses with only one literal
set<int> eliminateall(vector<set<int>>& query){
	//the set singular will contain all the singular literals
	set<int> singular;
	pair<int,int> check=isempty(query);
	while(check.first==0&&check.second!=0){
		for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
		// check the size of a clause if it is one and then insert it inot the set singular
			if(it->size()==1)  singular.insert(*(it->begin()));
		}
		for(std::set<int>::iterator iter=singular.begin();iter!=singular.end();iter++){
		int rem = *iter;
		//removing all the singular clauses
		query.erase(remove_if(query.begin(),query.end(),[&rem](set<int> i){return i.find(rem)!=i.end();}),query.end());
			for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();it++){
				set<int>::iterator it2=it->find(-rem);
				if((it2)!=(it->end())) it->erase(it2);
			}
		}
		check=isempty(query);
	}
	return singular;
}

//this function will solve the clauses and return either SAT(1) or UNSAT(0)
int solve(vector<set<int>>& query){
	//checking if our set is empty
	pair<int,int> check=isempty(query);
	if(check.first==1) return 0;
	if(check.first==-1) return 1;
	cnt++;
	if(check.second!=0){
		//all the singular clauses are eliminated
		set<int> q=eliminateall(query);
		//condition to return if query is satisfiable
		if(solve(query)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
			return 1;
		}
		else return 0;
	}
	//if the query contains pure literals
	if(is_pure(query)){
		//purifying the query
		set<int> q=purify(query);
		if(solve(query)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++)
			currassgn.insert(*iter);
			return 1;
		}
		else return 0;
	}
	//the maximum occuring literal is now removed and checked if the query is still satisfiable
	int l=max_occ(query);
	set<int> s1,s2;
	s1.insert(l);
	s2.insert(-l);
	//query is duplicated to query
	vector<set<int>> query1;
	query1=query;
	query1.push_back(s1);
	//check by assuming the maximum occuring literal to be true
	set<int> q=eliminateall(query1);
	if(solve(query1)==1){
		for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
		return 1;
	}
	else{
		//now assuming the maximum occuring literal to be false
		query.push_back(s2);
		q=eliminateall(query);
		if(solve(query)==1){
			for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
			return 1;
		}
		return 0;
	}
}


int main(){
	clock_t start=clock();
	//opening the file containing the SAT encoding in DIMACS format
	FILE* fp=fopen("sat.txt","r");
	fscanf(fp,"p cnf %d %d\n",&var,&clauses);
	//query is a vector of sets
	vector<set<int>> query;
	int temp=0;
	//this boolean array will store the occurence of both positive and negated literals
	bool arr[2*var+1];
	//the boolean array is initialised to zero
	for(int i=0;i<2*var+1;i++) arr[i]=0;
	set<int> empty;
	//we initialise query with empty sets
	for(int i=0; i<clauses;i++) query.push_back(empty);

	//here, we take input to 'query' and also assign the values for boolean array
	for(int i=0; i<clauses;i++){
		while(1){
			fscanf(fp,"%d",&temp);
			//condition to break if 0 is encountered
			if(temp==0) break;
			else if(temp>0) arr[temp]=1;
			else arr[var+abs(temp)]=1;
			//taking input into query
			query[i].insert(temp);
		}
	}

	//pure literals
	for(int i=1;i<=var;i++){
		//this case focuses on literals of the form 'a'
		if(arr[i]&&!arr[var+i]){ currassgn.insert(i); query.erase(remove_if(query.begin(),query.end(),[&i](set<int> j){return j.find(i)!=j.end();}),query.end());}
		//this case focuses on literals of the form 'a'
		else if(!arr[i]&&arr[var+i]){ currassgn.insert(-i); query.erase(remove_if(query.begin(),query.end(),[&i](set<int> j){return j.find(-i)!=j.end();}),query.end());}
	}

	//remove tautology
	for(int i=1;i<=var;i++){
		//finding literal 'a' and '-a'
		for(std::vector<set<int>>::iterator it=query.begin();it!=query.end();){
			if(it->find(i)!=it->end()&&it->find(-i)!=it->end()) it=query.erase(it);
			else it++;
		}
	}

	//implementing repeated unit propagation
	set<int> q=eliminateall(query);

	//check if the clauses are satisfiable
	if(solve(query)){
		cout<<"SATISFIABLE"<<endl;
		for(std::set<int>::iterator iter=q.begin();iter!=q.end();iter++) currassgn.insert(*iter);
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
