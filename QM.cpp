#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#define SIZE 10
using namespace std;
class QM;
class File;
class Data{
public:
	bool same = 0;
	bool check = 0;
	bool dontCare = 0;
	int one = 0;
	vector<int>number;
	vector<char>algebra;
};
class QM{
	
public:
	QM(){
		for (int i = 0; i < SIZE; ++i){
			initializer.algebra.push_back('?');
			initializer.dontCare = 0;
			initializer.check = 0;
		}
	}
	void bit(const vector<int>&input){
		for (int i = 0; i < input.size(); ++i){
			Data oneterm;
			int counter = input.at(i);
			if (counter < 0){
				counter *= -1;
				oneterm.dontCare = 1;
			}
			oneterm.number.push_back(counter);
			while (counter != 0){
				if (counter % 2 == 1){
					oneterm.algebra.insert(oneterm.algebra.begin(), '1');
					++oneterm.one;
				}
				else{ oneterm.algebra.insert(oneterm.algebra.begin(), '0'); }
				counter /= 2;
			}

			while (oneterm.algebra.size() != SIZE){
				oneterm.algebra.insert(oneterm.algebra.begin(), '0');
			}
			terms.push_back(oneterm);
		}

	}
	void grouping(vector<Data>&G1, vector<Data>&G2){
		if (G1.size() != 0){
			for (int i = 0; i < G1.size() - 1; ++i){
				for (int j = i + 1; j < G1.size(); ++j){
					if (G1.at(i).same == 0 && G1.at(j).same == 0){
						int sum = 0;
						int dash = 0;
						for (int k = 0; k < SIZE; ++k){
							sum += different(G1.at(i).algebra.at(k), G1.at(j).algebra.at(k));
							if (different(G1.at(i).algebra.at(k), G1.at(j).algebra.at(k)) == 1){ dash = k; }
						}
						if (sum == 1){ addDash(i, j, dash, G1, G2); }
					}
				}
			}
			markTheSame(G2);
		}
	}
	//if the terms in the group are the same,delete one of it
	void markTheSame(vector<Data>&G){
		for (int i = 0; i < G.size(); ++i){
			for (int j = i + 1; j < G.size(); ++j){
				if (G.at(i).algebra == G.at(j).algebra){
					G.at(j).same = 1;

				}
			}
		}
	}
	bool different(char element1, char element2){
		if (element1 == '?'){ return 0; }
		if (element1 == element2){ return 0; }
		else{ return 1; }
	}
	//check the terms that have been simplified and record them
	void addDash(int i, int j, int dash, vector<Data>&GO, vector<Data>&GT){

		//check
		GO.at(i).check = 1;
		GO.at(j).check = 1;
		GT.push_back(initializer);
		GT.at(GT.size() - 1).algebra = GO.at(i).algebra;
		GT.at(GT.size() - 1).one = GO.at(i).one - 1;
		GT.at(GT.size() - 1).algebra.at(dash) = '-';

		for (int a = 0; a < GO.at(i).number.size(); ++a){
			GT.at(GT.size() - 1).number.push_back(GO.at(i).number.at(a));
		}

		for (int b = 0; b < GO.at(j).number.size(); ++b){
			GT.at(GT.size() - 1).number.push_back(GO.at(j).number.at(b));
		}


	}
	void sort(vector<Data>&minterm){

		int test;
		for (int i = 0; i < minterm.size() - 1; ++i){

			if (minterm.at(i).one>minterm.at(i + 1).one){

				Data temp = minterm.at(i + 1);
				minterm.at(i + 1) = minterm.at(i);
				minterm.at(i) = temp;
				i = -1;
			}
		}
	}
		
	void ifCheck(vector<Data>&G){
		for (int a = 0; a < G.size(); ++a){
			if (G.at(a).check == 0 && G.at(a).same != 1 && G.at(a).dontCare != 1){
				map.push_back(G.at(a));
			}
		}
	}

	//find all essential prime implicants
	void findEssential(const vector<Data>&minterm){

		vector<vector<int>>emp;
		vector<int>x;

		//initialize the essential prime map
		for (int a = 0; a < minterm.size(); ++a){
			x.push_back(0);
		}
		for (int b = 0; b < map.size(); ++b){
			emp.push_back(x);
		}

		//fill in the array
		for (int i = 0; i < map.size(); ++i){
			for (int j = 0; j < map.at(i).number.size(); ++j){
				for (int k = 0; k < minterm.size(); ++k){
					if (map.at(i).number.at(j) == minterm.at(k).number.at(0)){
						emp[i][k] = 1;
					}
				}
			}
		}

		//if that term has been covered, that column would be 2
		for (int i = 0; i < minterm.size(); ++i){
			int count = 0;
			int record = 0;
			for (int j = 0; j < map.size(); ++j){
				if (emp[j][i] == 1){
					++count;
					record = j;
				}
			}
			if (count == 1){
				for (int a = 0; a < minterm.size(); ++a){
					if (emp[record][a] == 1){
						for (int b = 0; b < map.size(); ++b){
							emp[b][a] = 2;
						}
					}
				}
				emp[record][i] = 1;
			}
		}

		//dont care
		for (int i = 0; i < minterm.size(); ++i){
			if (minterm.at(i).dontCare == 1){
				for (int j = 0; j < map.size(); ++j){
					emp[j][i] = 2;
				}
			}
		}

		//record every prime implicant which cover that term 
		for (int i = 0; i < minterm.size(); ++i){
			vector<int>prime;
			for (int j = 0; j < map.size(); ++j){
				if (emp[j][i] == 1){
					prime.push_back(j);
				}
			}

			if (prime.size() != 0){
				petrick.push_back(prime);
				mark.push_back(prime.size());
			}
		}


	}
	
	//find all possible solution
	void Petrick(){
		vector<int>initial;

		for (int i = 0; i < mark.at(0); ++i){
			solution.push_back(initial);
			solution.at(i).push_back(petrick.at(0).at(i));
		}

		for (int i = 1; i < petrick.size(); ++i){

			for (int z = 0; z < solution.size(); ++z){
				for (int a = 1; a < mark.at(i); ++a){
					solution.insert(solution.begin() + z, solution.at(z));
					++z;
				}
			}
			for (int b = 0; b < solution.size(); ++b){
				solution.at(b).push_back(petrick.at(i).at((b + mark.at(i)) % mark.at(i)));
			}
		}
		bestSolution();
	}

	string decoder(vector<char>boolean)const{
		string result;
		for (int i = 0; i < boolean.size(); ++i){
			if (boolean.at(i) == '1'){
				char x = i + 'a';
				result.push_back(x);
			}
			else if (boolean.at(i) == '0'){
				char y = i + 'a';
				result.push_back(y);
				result.push_back('\'');
			}
			else{}
		}
		return result;
	}

	//record the same term s
	bool Repeat(int column, int number){
		int counter = 0;
		for (int i = 0; i < solution.at(column).size(); ++i){
			if (number == solution.at(column).at(i)){
				++counter;
			}
		}
		if (counter == 1){ return 0; }
		else{ return 1; }
	}

	//choose the best solution
	void bestSolution(){
		for (int i = 0; i < solution.size(); ++i){
			for (int j = 0; j < solution.at(i).size(); ++j){
				//delete the same terms
				if (Repeat(i, solution.at(i).at(j)) == 1){
					solution.at(i).erase(solution.at(i).begin() + j);
					--j;
				}
			}
		}

		//pick one of the answer
		finalAnswer.push_back(solution.at(0));
		//cout << finalAnswer.at(0).size();

		//if the answer is greater,delete the old one;if as good as the old one, record it
		for (int i = 1; i < solution.size(); ++i){
			if (finalAnswer.at(0).size()>solution.at(i).size()){
				finalAnswer.clear();
				finalAnswer.push_back(solution.at(i));
			}
			else if (finalAnswer.at(0).size() == solution.at(i).size()){
				finalAnswer.push_back(solution.at(i));
			}
		}
		//cout << finalAnswer.at(0).size();
	}

	void receive(vector<Data>&input){
		input = terms;
	}

	void output(vector<Data>minterm, File &file);

	void printResult(const vector<Data>&minterm, File &file)const;
private:
	vector<Data>terms;
	Data initializer;
	vector<Data>map;
	vector<int>mark;
	vector<vector<int>>petrick;
	vector<vector<int>>solution;
	vector<vector<int>>finalAnswer;
};
class File{
	friend int main();
	friend void QM::output(vector<Data>,File &file);
	friend void QM::printResult(const vector<Data>&minterm,File &file)const;
public:
	void setFileIn(string file){
		inFile.open(file, ios::in);
		if (!inFile){
			cerr << "Failed opening input file" << endl;
			exit(1);
		}
	}

	void setFileOut(string file){
		outFile.open(file, ios::out);
		if (!outFile){
			cerr << "Failed opening output file" << endl;
			exit(1);
		}
	}

	void readInput(vector<int>&input){
		while (inFile>> line){
			temp.push_back(line);
		}
		transform(input);
		
	}
	
	void transform(vector<int>&input){

		for (int i = 0; i < temp.size(); ++i){
			int counter = 0;
			int dec = pow(10, (temp.at(i).size() - 1));

			for (int j = 0; j < temp.at(i).size(); ++j){
				
				char text = temp.at(i).at(j);
				if (text == '('){
					dec /= 100;
					continue;
				}
				else if (text == ')'){
					counter *= -1;//don't care
				}
				else{
					counter += (text - '0')*dec;
					dec /= 10;
				}
			}
			input.push_back(counter);
		
		}
	}



private:
	ifstream inFile;
	ofstream outFile;
	vector<string>temp;
	string line;
	char text;
};

void QM::output(vector<Data>minterm, File &file){
	sort(minterm);

	for (int i = 0; i < minterm.size(); ++i){

		//checked or not ,don't care  
		if (minterm.at(i).same == 1){
			cout << 'x' << " ";
			file.outFile << 'x' << " ";
		}
		else if (minterm.at(i).dontCare == 1){
			cout << 'd' << " ";
			file.outFile << 'd' << " ";
		}
		else if (minterm.at(i).check == 1){
			cout << 'v' << " ";
			file.outFile << 'v' << " ";
		}
		else{
			cout << "  ";
			file.outFile << "  ";
		}


		for (int j = 0; j < SIZE; ++j){
			cout << minterm.at(i).algebra.at(j);
			file.outFile << minterm.at(i).algebra.at(j);
		}
		cout << " ";
		for (int k = 0; k < minterm.at(i).number.size(); ++k){
			cout << minterm.at(i).number.at(k);
			file.outFile << minterm.at(i).number.at(k);;
			if (k != minterm.at(i).number.size() - 1){ 
				cout << ','; 
				file.outFile << ',';
			}
		}
		cout << endl;
		file.outFile << endl;

		if (i <= minterm.size() - 2){
			if (i != minterm.size() - 1 && minterm.at(i).one != minterm.at(i + 1).one){
				cout << "-------------------" << endl;
				file.outFile << "-------------------" << endl;
			}
		}

	}


}
void QM::printResult(const vector<Data>&minterm, File &file)const{
	cout << "======================Result======================" << endl;
	file.outFile << "======================Result======================" << endl;
	cout << "                    |";
	file.outFile << "                    |";
	for (int i = 0; i < minterm.size(); ++i){
		cout << setw(4) << minterm.at(i).number.at(0);
		file.outFile << setw(4) << minterm.at(i).number.at(0);
		if (i != minterm.size() - 1){
			cout << ','; 
			file.outFile << ',';
		}
	}
	cout << endl;
	file.outFile << endl;

	cout << "--------------------+-----------------------------" << endl;
	file.outFile << "--------------------+-----------------------------" << endl;

	for (int i = 0; i < map.size(); ++i){
		cout << left << setw(20) << decoder(map.at(i).algebra);
		file.outFile << left << setw(20) << decoder(map.at(i).algebra);
		cout << '|';
		file.outFile << '|';

		for (int j = 0; j < minterm.size(); ++j){
			for (int k = 0; k < map.at(i).number.size(); ++k){
				if (map.at(i).number.at(k) == minterm.at(j).number.at(0)){
					if (j == 0){ 
						cout << right << setw(4) << 'x'; 
						file.outFile << right << setw(4) << 'x';
					}
					else{
						cout << right << setw(5) << 'x'; 
						file.outFile << right << setw(5) << 'x';
					}

					break;
				}

				if (k == map.at(i).number.size() - 1){
					if (j == 0){
						cout << "    ";
						file.outFile << "    ";
					}
					else{
						cout << "     ";
						file.outFile << "    ";
					}
				}

			}
		}
		cout << endl;
		file.outFile << endl;
	}
	cout << "--------------------+-----------------------------" << endl;
	file.outFile << "--------------------+-----------------------------" << endl;
	cout << "F(A,B,C,D,E,F,G,H,I,J) =";
	file.outFile << "F(A,B,C,D,E,F,G,H,I,J) =";
	for (int a = 0; a < finalAnswer.at(0).size(); ++a){
		cout << decoder(map.at(finalAnswer.at(0).at(a)).algebra);
		file.outFile << decoder(map.at(finalAnswer.at(0).at(a)).algebra);
		if (a != finalAnswer.at(0).size() - 1){ 
			cout << '+';
			file.outFile << '+';
		}
	}
	cout << endl << "==================================================" << endl;
	file.outFile << endl << "==================================================" << endl;
}
int main(){
	
	vector<int>input;

	vector<Data>terms;
	vector<Data>G1;
	vector<Data>G2;
	vector<Data>G3;
	vector<Data>G4;
	vector<Data>G5;
	vector<Data>G6;
	vector<Data>G7;
	vector<Data>G8;
	vector<Data>G9;
	vector<Data>G10;

	string fileName;

	File file;
	file.setFileIn("input.txt");
	file.setFileOut("output.txt");
	file.readInput(input);
	
	QM qm;
	qm.bit(input);
	qm.receive(terms);
	
	//simplify the minterm
	qm.grouping(terms, G1);
	qm.grouping(G1, G2);
	qm.grouping(G2, G3);
	qm.grouping(G3, G4);
	qm.grouping(G4, G5);
	qm.grouping(G5, G6);
	qm.grouping(G6, G7);
	qm.grouping(G7, G8);
	qm.grouping(G8, G9);
	qm.grouping(G9, G10);

	qm.ifCheck(terms);
	qm.ifCheck(G1);
	qm.ifCheck(G2);
	qm.ifCheck(G3);
	qm.ifCheck(G4);
	qm.ifCheck(G5);
	qm.ifCheck(G6);
	qm.ifCheck(G7);
	qm.ifCheck(G8);
	qm.ifCheck(G9);
	qm.ifCheck(G10);

	//find all essential prime implicants
	qm.findEssential(terms);

	//use Petrick's method to final the best solution
	qm.Petrick();

	//output
	if (terms.size() != 0){
		cout << "====== Round 1 ======" << endl;
		qm.output(terms,file);
		cout << endl;
		file.outFile << endl;
	}
	if (G1.size() != 0){
		cout << "====== Round 2 ======" << endl;
		qm.output(G1, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G2.size() != 0){
		cout << "====== Round 3 ======" << endl;
		qm.output(G2, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G3.size() != 0){
		cout << "====== Round 4 ======" << endl;
		qm.output(G3, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G4.size() != 0){
		cout << "====== Round 5 ======" << endl;
		qm.output(G4, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G5.size() != 0){
		cout << "====== Round 6 ======" << endl;
		qm.output(G5, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G6.size() != 0){
		cout << "====== Round 7 ======" << endl;
		qm.output(G6, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G7.size() != 0){
		cout << "====== Round 8 ======" << endl;
		qm.output(G7, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G8.size() != 0){
		cout << "====== Round 9 ======" << endl;
		qm.output(G8, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G9.size() != 0){
		cout << "====== Round 10 ======" << endl;
		qm.output(G9, file);
		cout << endl;
		file.outFile << endl;
	}
	if (G10.size() != 0){
		cout << "====== Round 11 ======" << endl;
		qm.output(G10, file);
		cout << endl;
		file.outFile << endl;
	}

	qm.printResult(terms, file);

	return 1;
}
