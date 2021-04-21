#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

#define MAXFAMILY 99
#define MAXPERSONS 99
#define MAXCHILDREN 10

struct personNode{
	int number;
	
};

struct familyNode{
	 personNode* husband;
	 personNode* wife;
	 personNode* children[MAXCHILDREN]; 
	 
	 
};

int main(){
string command;
int n;

while (cin){
	cin >> command;
	cin >> n;
	
	}
}


return 0;
}
