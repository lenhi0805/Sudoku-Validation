#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

int string_convert_to_int(char a) {
	int ia = a - '0';
	return ia;
}

void init(vector<vector<int>>& row, vector<vector<int>>& col, vector<vector<int>>&square) {
	for (int i = 0; i < 9; i++) {
		vector<int>k;
		row.push_back(k);
		col.push_back(k);
		square.push_back(k);
	}
}

vector<vector<int>>input_sudoku(ifstream& fin) {
	vector<vector<int>>sudoku;
	for (int i = 0; i < 9; i++) {
		vector<int>line;
		string row;
		getline(fin, row);
		for (int j = 0; j < 9; j++) {
			if (isdigit(row[j])) line.push_back(string_convert_to_int(row[j]));
			else if (row[j] == ' ') line.push_back(0);
		}
		sudoku.push_back(line);
	}
	return sudoku;
}

int check_none_repeat(int a[], vector<int>&error) {
	bool repeat = false;
	for (int i = 1; i < 10; i++) {
		if (a[i] > 1) {
			error.push_back(i);
			repeat = true;
		}
	}
	if (repeat == true) return 0; //invalid
	else if (a[0] != 0) return -1; //unsolved
	else return 1;//solved
}

int check_none_repeat_square(vector<vector<int>>sudoku, int row, int col, vector<int>&error) {
	int a[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = row; i < row + 3; i++) {
		for (int j = col; j < col + 3; j++) {
			a[sudoku[i][j]]++;
		}
	}
	return (check_none_repeat(a,error));
}

int check(vector<vector<int>>sudoku, vector<vector<int>>&col_error, vector<vector<int>>&row_error, vector<vector<int>>&square_error) {
	int type;
	vector<int>error;
	bool unsolved = false;
	bool is_invalid = false;
	for (int i = 0; i < 9; i++) {
		int a[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		error.empty();
		for (int col = 0; col < 9; col++) {
			a[sudoku[i][col]]++;
		}
		type = check_none_repeat(a, row_error[i]);
		if (type == 0) {
			is_invalid = true;
		}
		else if (type == -1) unsolved = true;
		error.empty();
		int b[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int row = 0; row < 9; row++) {
			b[sudoku[row][i]]++;
		}
		type = check_none_repeat(b, col_error[i]);
		if (type == 0) {
			is_invalid = true;
		}
		else if (type == -1) unsolved = true;
	}
	int k = 0;
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 9; j += 3) {
			error.empty();
			if (check_none_repeat_square(sudoku, i, j, square_error[k]) == 0) {
				is_invalid = true;
			}
			k++;
		}
	}
	if (is_invalid == true) return 0;
	else if (unsolved == true) return -1;
	else return 1;
}

void find_error_row(vector<vector<int>>row) {
	for (int i = 0; i < 9; i++) {
		if (row[i].size() != 0) {
			for (int j = 0; j < row[i].size(); j++) {
				cout << "row " << i + 1 << " has multiple " << row[i][j] << "s ";
			}
		}
	}
}

void find_error_col(vector<vector<int>>col) {
	for (int i = 0; i < 9; i++) {
		if (col[i].size() != 0) {
			for (int j = 0; j < col[i].size(); j++) {
				cout << "column " << i + 1 << " has multiple " << col[i][j] << "s ";
			}
		}
	}
}

string position(int i) {
	string pos = "";
	if (i < 3) pos += "upper ";
	else if (i < 6) {}
	else pos += "lower ";
	if (i % 3 == 0) pos += "left ";
	else if (i % 3 == 1) pos += "middle ";
	else pos += "right ";
	return pos;
}

void find_error_square(vector<vector<int>>square) {
	for (int i = 0; i < 9; i++) {
		if (square[i].size() != 0) {
			string pos = position(i);
			cout << pos << "has multiple ";
			for (int j = 0; j < square[i].size(); j++) {
				cout << square[i][j] << "s ";
			}
		}
	}
}



int main() {
	
	string filename;
	ifstream fin;
	cout << "Please enter file name: ";
  cin >> filename;
	fin.open(filename);
	while (!fin.eof()) {
		string id;
		getline(fin, id);
	vector<vector<int>>col_error;
	vector<vector<int>>row_error;
	vector<vector<int>>square_error;
  init(col_error, row_error, square_error);
		if (id != "") {
			vector<vector<int>>v = input_sudoku(fin);
			cout << id << "\t";
			int result = check(v,col_error,row_error,square_error);
			if (result == 1) cout << "solved";
			else if (result == 0) {
				cout << "invalid ";
				find_error_row(row_error);
				find_error_col(col_error);
				find_error_square(square_error);
			}
			else {
				cout << "valid";
			}
			cout << endl;
		}
	}
}
