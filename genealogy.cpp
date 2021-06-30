#include <iomanip>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include<iostream>
#include <locale>
#include <vector>
using namespace std;

#define MAXFAMILY 100 // storing nums at their corresponding index 1:1 ; index 0 is null place 
#define MAXPERSONS 100
#define MAXCHILDREN 11
#define maxChars 256
#define maxVars 14 // max ints on Family... command

struct vertex_s{
	struct vertex_s *rep;
	int depth =0;
};
// matrix for path finding
vector<int> adj[MAXPERSONS+MAXFAMILY];

// pop function for vect to remove first element
void popFront(vector<int> &v){
    for(size_t i = 1; i < v.size(); i++)
        v[i-1] = v[i];
    v.pop_back();
}
struct personNode;
struct familyNode{
	 int famNumber=0;
	 personNode* husband; // point to one husband in family
	 personNode* wife;    // point to one wife in family
	 personNode* children[MAXCHILDREN]; // point to up to MAXCHILDREN children in family
	 vertex_s* rep = new vertex_s; // for union-find algorithm: representative
	 
};
struct personNode{
	int number=0;
	familyNode* parentage; // points to family that this person is a child of
	familyNode* marriage;  // points to family that this person is a parent of
	vertex_s* rep = new vertex_s; // representative for union find

};

personNode *personNodes[MAXPERSONS];
familyNode *familyNodes[MAXFAMILY];

void initializeNodes(){
	for (int i = 0;i <MAXFAMILY;i++){
		familyNode* temp = new familyNode;
		temp->famNumber = 0;
		familyNodes[i] = temp;
	}
	for (int i = 0;i <MAXPERSONS;i++){
		personNode* temp = new personNode;
		temp->number = 0;
		personNodes[i] = temp;
	}
}

// add an edge in the adjacency matrix between two people 
void addEdge(int famNum, int personNum){
    adj[famNum+MAXPERSONS].push_back(personNum);
    adj[personNum].push_back(famNum+MAXPERSONS);
}
	// union part of union-find; 
void unionF(familyNode *a, personNode *b){
	// find representative of a
	vertex_s *repA = a->rep;
	while (repA->rep !=NULL)
		repA = repA->rep;
	//find rep of b
	vertex_s* repB = b->rep;	
	while (repB->rep !=NULL)
		repB = repB->rep;
	
	if (repA == repB)
		return;
	// if depth of A is greater, B's rep points to A's
	if (repA->depth >= repB->depth){ 
		repB->rep = repA;
		// increment A depth
		if (repA->depth == repB->depth)
			repA->depth++;
		}
	else
	repA->rep = repB;
}
	// set husband in family
int setHusband(familyNode* fam, int num){
	if (personNodes[num]->number < 1){ // person is new
		personNodes[num]->number = num;
		personNodes[num]->marriage = fam;
		familyNodes[fam->famNumber]->husband = personNodes[num];
		unionF(familyNodes[fam->famNumber], personNodes[num]);
		addEdge(fam->famNumber, personNodes[num]->number);
		return 1;
	}
	else if (personNodes[num]->marriage != NULL){ // already a parent
		cout << "	Person " << num << " is already a parent." << endl;
		return -1;
	}
	else if (personNodes[num]->parentage != NULL){ // person is a child
		personNodes[num]->marriage = fam;
		familyNodes[fam->famNumber]->husband = personNodes[num];
		unionF(familyNodes[fam->famNumber], personNodes[num]);
		addEdge(fam->famNumber, personNodes[num]->number);
		return 0;
	}
	return -2;
}
	// set wife in family
int setWife(familyNode* fam, int num){
	if (personNodes[num]->number < 1){ // person is new
		personNodes[num]->number = num;
		personNodes[num]->marriage = fam;
		familyNodes[fam->famNumber]->wife=personNodes[num];
		unionF(familyNodes[fam->famNumber], personNodes[num]);
		addEdge(fam->famNumber, personNodes[num]->number);
		return 1;
	}
	else if (personNodes[num]->marriage != NULL){ // already a parent
		cout << "	Person " << num << " is already a parent." << endl;
		return -1;
	}
	else if (personNodes[num]->parentage != NULL){ //person is a child
		personNodes[num]->marriage = fam;
		familyNodes[fam->famNumber]->wife = personNodes[num];
		unionF(familyNodes[fam->famNumber], personNodes[num]);
		addEdge(fam->famNumber, personNodes[num]->number);
		return 0;
	}
	return -2;
}
	// set children in family, if given children
int setChildren(familyNode* fam, int inputs[]){

	for (int j = 3;j<maxVars && inputs[j] != 0;j++){
		
		int num = inputs[j];
		if (personNodes[num]->number < 1){ // this person is new
			int set = 0;
			for (int i =0;i<MAXCHILDREN && set == 0;i++){
				if (fam->children[i] == NULL){
					personNodes[num]->parentage = fam;
					personNodes[num]->number = num;
					//personNodes[num]->rep = new vertex_s;
					fam->children[i] = personNodes[num];
					set =1;
					familyNodes[fam->famNumber]->children[i] = personNodes[num];
					unionF(familyNodes[fam->famNumber], personNodes[num]);
					addEdge(fam->famNumber, personNodes[num]->number);
				}
			}
		}
		else if (personNodes[num]->parentage != NULL){ // this person is a child alrdy
			cout << "	Child " << num << " is already parented." << endl;
			return -1;
		}
		else if (personNodes[num]->marriage != NULL){ // this person is a parent alrdy
				personNodes[num]->parentage = fam;
				int set = 0;
				for (int i =0;i<MAXCHILDREN && set ==0;i++){
					if (fam->children[i] == NULL){
						personNodes[num]->parentage = fam;
						fam->children[i] = personNodes[num];
						set = 1;
						familyNodes[fam->famNumber]->children[i] = personNodes[num];
						unionF(familyNodes[fam->famNumber], personNodes[num]);
					addEdge(fam->famNumber, personNodes[num]->number);
					}
				}	
			}
		}
	return 0;
}
	// set family and place it in familyNodes[familyNum] upon valid command
int setFamily(int inputs[]){
	familyNode *temp = new familyNode;
	temp->famNumber = inputs[0];
	int hubError;
	int wifeError;
	int childError;
	for (int i = 1;i <maxVars && inputs[i] != 0;i++){
		if (inputs[i] > 99 || inputs[i] < 1){
			cout << "Family and Person numbers must be within [1,99] " << endl;
			return -1;
		}
	}
	if (personNodes[inputs[1]]->marriage != NULL && inputs[1] != 0 ){
			cout << "	Husband " << inputs[1] << " is already married." <<endl;
			return -1;
		}
	if (personNodes[inputs[2]]->marriage != NULL && inputs[2] != 0){
			cout << "	Wife " << inputs[2] << " is already married." <<endl;
			return -1;
		}
	
	// check if family is already taken 
	if (familyNodes[temp->famNumber]->famNumber == 0){
		hubError = setHusband(temp, inputs[1]);
		if (hubError == -1)
			return -1;
		wifeError = setWife(temp, inputs[2]);
		if (wifeError == -1)
			return -1;
		childError = setChildren(temp, inputs);
		if (childError == -1)
			return -1;
		
		
		// put new family vertex into array of family nodes
		familyNodes[temp->famNumber]->famNumber = temp->famNumber;
		return temp->famNumber;
		}
		else{
			cout << "	Family "<< temp->famNumber <<" has already been entered." << endl;
			return -1;
		}
	return 0;
}



	// check if there is a relation between two people
bool find(personNode* a, personNode* b){
	// rep of a
	vertex_s *repA = a->rep;
	while (repA->rep !=NULL)
		repA = repA->rep;
	//find rep of b
	vertex_s* repB = b->rep;	
	while (repB->rep !=NULL)
		repB = repB->rep;
    // are they the same
    return (repA == repB); 
    
}
	// print family after family command
void printFamily(int num){
	if (familyNodes[num]->famNumber > 0){
		cout << "Family " <<familyNodes[num]->famNumber << " has husband " << 
		familyNodes[num]->husband->number << ", wife " << 
		familyNodes[num]->wife->number;
		if (familyNodes[num]->children[0] != NULL)
			cout << ", and children ";
		for (int j = 0; j<MAXCHILDREN;j++){
			if (familyNodes[num]->children[j] != NULL){
				cout  << " " << familyNodes[num]->children[j]->number;
			}
		}
	cout << "."<<endl;
	}
}

 	// Scan User-entered command and split into tokens
void scan(string command, string arr[]){
	for (int i = 0; i < maxChars && arr[i] != ""; i++){
		arr[i] = "";
	}
		
	char * str = new char[command.size() + 1];
	copy(command.begin(), command.end(), str);
	str[command.size()] = '\0';
	char * pch;
	pch = strtok (str," \n\t");
	int i = 0;
	string s;
	while (pch != NULL){
		s = pch;
		arr[i] = pch;
		pch = strtok (NULL, " \n\t");
		i++;
  	}
  	delete[] str;
  	// make every non-capital.
	// i don't care about capitalization for commands
	locale loc;
	string str1;
	for (int i = 0 ; i < maxChars && arr[i] != "";i++){
		str1 = arr[i];
  		for (size_t j=0; j<str1.length(); ++j){
  			arr[i][j] = tolower(str1[j],loc);
  		}
    	}
	
}

// check the tokens of the command line, organize given integers, 
// and check for which kind of command the user typed
int parse(string array[], int inputs[]){
    	if(array[0] == "family"){
		// assume formatting is correct by user :/
		int j =0; // index of inputs
    		for (int i=0; i<maxChars && array[i+1] != "";i++){

    			if(i%2 == 0){
    				if (stoi(array[i+1]) < 100 && stoi(array[i+1]) >= 0){
    					inputs[j] = stoi(array[i+1]);
    					j++;
    				}
    				else
    					return -1;
    			}
    		}
    	
    		return 0;
    	}
    		else if(array[0] == "relate"){
    			inputs[0] = stoi(array[1]);
    			inputs[1] = stoi(array[2]);

    			return 1;
    	}
    	else
    		return -1;
}
// breadth first search function
bool bfSearch(int person1, int person2, int step[]){
	vector<int> queue;
	int size = MAXPERSONS+MAXFAMILY;
	bool haveVisit[size];
	for (int i =1;i<size;i++){
		haveVisit[i] = false;
		step[i] = -1;
	}
	queue.push_back(person1);
	
	while(queue.size() != 0){
		int p = queue[0];
		popFront(queue);
		
		for (size_t i =0;i<adj[p].size();i++){
			if (haveVisit[adj[p][i]] == false){
				haveVisit[adj[p][i]] = true;
				step[adj[p][i]] = p;
				queue.push_back(adj[p][i]);
				if(adj[p][i] == person2){
					step[person1] = -1;
					return true;
				}
			}
		}
	}
	return false;
	 // else false
}
	// print path from person 1 to person 2
void printPath(int person1, int person2){
	int step[MAXPERSONS+MAXFAMILY];
	if (bfSearch(person1, person2, step)){
		cout << "Relation: ";
		int ind = person2;
		vector<int> path;
		path.push_back(ind);
		while(step[ind]!=-1){
			path.push_back(step[ind]);
			ind = step[ind];
		}
		for (int i = path.size() -1;i>=0;i--){
			if (path[i] <MAXPERSONS){
				cout << "person " << path[i];
				if(i!=0){
					cout << "->";
				}
			}
			else
				cout << "family " << path[i] - MAXPERSONS << "->";
		}
		cout << endl;
	}
	else
		cout << "Persons " << person1 << "and " << person2 << " are not related." <<endl;
}
	// if relation command entered, print path if there is a relation 
void findRelation(int inputs[]){
	if (find(personNodes[inputs[0]], personNodes[inputs[1]])){
		printPath(personNodes[inputs[0]]->number, personNodes[inputs[1]]->number);
	}
	else
		cout << "Persons " << inputs[0] << " and " <<  inputs[1] << " are not related." <<endl;
}
int main(){
	initializeNodes();
	string array[maxChars]; // holds tokens that are retrieved by scanning method
	int inputs[maxVars]; // holds integer values for family, husb, wife, children
	string command;
	int commandCheck = -1239;
	int famSet;
	 for (;getline(cin, command);) {
	 	famSet = -1;
		for (int i = 0;i<maxVars;i++)
			inputs[i] = 0;
		scan(command,  array);
		commandCheck = parse(array, inputs);
		if (commandCheck == 0) // family command
			famSet = setFamily(inputs);
		else if (commandCheck == 1)// relate commanxd
			findRelation(inputs);
		else if (commandCheck == -1) // unknown command
			cout << "Syntax error." << endl; 
		if (famSet > 0){
			printFamily(famSet);
		}	
		else if(famSet == 0)
			cout << "Family must be [1,99]" << endl;
    }
	return 0;
}
