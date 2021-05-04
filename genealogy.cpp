#include <iomanip>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include<iostream>
#include <locale>
using namespace std;

#define MAXFAMILY 100
#define MAXPERSONS 100
#define MAXCHILDREN 11
#define maxChars 256
#define maxVars 14 // max ints on Family... command

struct personNode;
struct familyNode{
	 int famNumber;
	 personNode* husband;
	 personNode* wife;
	 personNode* children[MAXCHILDREN]; 
	 
	 familyNode():famNumber(0),
	 		husband(NULL),
	 		wife (NULL){}
			//children({NULL}){}
	 
};
struct personNode{
	int number;
	familyNode* parentage;
	familyNode* marriage ;
	
	 personNode():number(0),
	 		parentage(NULL),
	 		marriage(NULL){}
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
int setHusband(familyNode* fam, int num){
	if (personNodes[num]->number < 1){
		personNode *husband = new personNode;
		husband->number = num;
		husband->marriage = fam;
		fam->husband = husband;
		personNodes[num] = husband;
		return 1;
	}
	else if (personNodes[num]->marriage != NULL){
		cout << "Error: Person " << num << " already a parent." << endl;
		return -1;
	}
	else if (personNodes[num]->parentage != NULL){
		personNodes[num]->marriage = fam;
		fam->husband = personNodes[num];
		return 0;
	}
		
	return -2;
}
	
int setWife(familyNode* fam, int num){
	if (personNodes[num]->number < 1){
		personNode *wife = new personNode;
		wife->number = num;
		wife->marriage = fam;
		fam->wife = wife;
		personNodes[num] = wife;
		return 1;
	}
	else if (personNodes[num]->marriage != NULL){
		cout << "Error: Person " << num << " already a parent." << endl;
		return -1;
	}
	else if (personNodes[num]->parentage != NULL){
		personNodes[num]->marriage = fam;
		fam->wife = personNodes[num];
		return 0;
	}
		
	return -2;
}
	
int setChildren(familyNode* fam, int inputs[]){

	for (int j = 3;j<maxVars && inputs[j] != 0;j++){
		
		int num = inputs[j];
		if (personNodes[num]->number < 1){ // this person is new
			personNode *child = new personNode;
			int set = 0;
			for (int i =0;i<MAXCHILDREN && set == 0;i++){
				if (fam->children[i] == NULL){
					personNodes[num]->parentage = fam;
					personNodes[num]->number = num;
					fam->children[i] = personNodes[num];
					set =1;
				}
			}
		}
		else if (personNodes[num]->parentage != NULL){ // this person is a child alrdy
			cout << "Error: Person " << num << "already a child of a family." << endl;
			return -1;
		}
		else if (personNodes[num]->marriage != NULL){ // this person is a parent alrdy
			if (num != 0){
				personNodes[num]->parentage = fam;
				for (int i =0;i<MAXCHILDREN;i++){
					if (fam->children[i] == NULL){
						personNodes[num]->parentage = fam;
						fam->children[i] = personNodes[num];
					}
				}	
			}
		}
		
		
	}
	
	return 0;
}
int setFamily(int inputs[]){

	familyNode *temp = new familyNode;
	temp->famNumber = inputs[0];
	int hubError;
	int wifeError;
	int childError;
	for (int i = 1;i <maxVars && inputs[i] != 0;i++){
		for (int j = i+1;j<maxVars-1;j++){
			if (inputs[i] == inputs[j] ){
				cout << "Same person cannot be in a family twice. " << endl;
				return -1;
			}
		}
		if (inputs[i] > 99 || inputs[i] < 1){
			cout << "Family and Person numbers must be within [1,99] " << endl;
			return -1;
		}
	}
	if (personNodes[inputs[1]]->marriage != NULL ||personNodes[inputs[2]]->marriage != NULL ){
			cout << "Person " << inputs[1] << " already a parent." <<endl;
			return -1;
		}
		
		cout << "Setting family " << endl;
	
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
		familyNodes[temp->famNumber] = temp;
		return 1;
		}
		else{
			cout << "Error: Family already exists." << endl;
			return -1;
		}
		
	return 0;
}

void setRelation(int inputs[]){
	
}

void printFamily(){
	for (int i = 0 ; i< MAXFAMILY ;i++){
		if (familyNodes[i]->famNumber > 0){
			cout << "Family " << familyNodes[i]->famNumber << " has husband " << 
			familyNodes[i]->husband->number << ", wife " << 
			familyNodes[i]->wife->number;
			if (familyNodes[i]->children[0] != NULL)
				cout << ", and children ";
			for (int j = 0; j<MAXCHILDREN;j++){
				if (familyNodes[i]->children[j] != NULL){
					cout  << familyNodes[i]->children[j]->number << " ";
				}
			}
			cout << endl;
		}
	}
}
void scan(string command, string arr[]){
	for (int i = 0; i < maxChars && arr[i] != ""; i++){
		arr[i] = "";
	}
		
	char * str = new char[command.size() + 1];
	copy(command.begin(), command.end(), str);
	str[command.size()] = '\0';
	char * pch;
	//printf ("Splitting string \"%s\" into tokens:\n",str);
	pch = strtok (str," \n\t");
	int i = 0;
	string s;
	while (pch != NULL){
		//printf ("%s\n",pch);
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
  		for (int j=0; j<str1.length(); ++j){
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
int main(){
	initializeNodes();
	string array[maxChars]; // holds tokens that are retrieved by scanning method
	int inputs[maxVars]; // holds integer values for family, husb, wife, children
	string command;
	int commandCheck = -1239;
	int famSet;
	 for (command;getline(cin, command);) {
		for (int i = 0;i<maxVars;i++)
			inputs[i] = 0;
		scan(command,  array);
		commandCheck = parse(array, inputs);
		if (commandCheck == 0) // family command
			famSet = setFamily(inputs);
		else if (commandCheck == 1) // relate command
			setRelation(inputs);
		else if (commandCheck == -1)
			cout << "Syntax error." << endl; 
		if (famSet == 1){
			printFamily();
		}	
    }
    
	return 0;
}
