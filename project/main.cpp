#include "nodes.h"
#include "clustering.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;
#define COMMA ','
#define NEW_LINE '\n'
#define NUM_CITY 25
#define NUM_PLANT 10
#define MONTH 12
#define THRESHOLD 3
const string CITY = "city";
const string CSV = ".csv";

vector<city> city_list, origin;
vector<plant> plant_list;
vector<double> city_buffer;
city tmp_city;
string file_name;

void read_CityDATA() {
	int i,j;
	string s_buf, token;
	size_t pos;
	for (i = 1; i <= NUM_CITY; i++) {
		file_name = CITY + to_string(i) + CSV;
		ifstream input(file_name);
		if (!(bool)input) {		// Check file exist
			cout << "Cannot find file name " << file_name << endl;
			break;
		}
		getline(input, s_buf, NEW_LINE);	// skip first index line
		while (input.good()) {
			j = 0;
			// read line by line
			getline(input, s_buf, NEW_LINE);
			if (s_buf == "") break;
			pos = s_buf.find(COMMA);
			token = s_buf.substr(0, pos);	// tokenize by COMMA
			token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
			s_buf.erase(0, pos + 1);		// cut first token
			//cout << token << endl;
			while ((pos = s_buf.find(COMMA)) != string::npos) {
				token = s_buf.substr(0, pos);	// tokenize by COMMA
				token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
				//cout << token << endl;
				city_buffer.push_back(round(stod(token)*100)/100);
				s_buf.erase(0, pos + 1);
				j++;
			}
			token = s_buf;
			token = token.substr(1, token.length() - 2);
			city_buffer.push_back(round(stod(token) * 100) / 100);		// Last token
			//cout << token << endl;
			//cout << "city buffer size : "<< city_buffer.size() << endl;
			tmp_city.insertData(city_buffer);
			city_buffer.clear();					// Clear buffer
		}
		city_list.push_back(tmp_city);
		tmp_city.clearData();							// Clear tmp
		//cout << file_name << "is completed" << endl;
	}
}
void read_CityINFO() {
	int j;
	double x_tmp, y_tmp;
	string s_buf, token;
	size_t pos;
	file_name = "cityInfo.csv";
	ifstream input(file_name);
	if (!(bool)input) {		// Check file exist
		cout << "Cannot find file name " << file_name << endl;
		return;
	}
	getline(input, s_buf, NEW_LINE);	// skip first index line
	j = 0;
	while (input.good()) {
		// read line by line
		getline(input, s_buf, NEW_LINE);
		if (s_buf == "") break;
		pos = s_buf.find(COMMA);
		token = s_buf.substr(0, pos);	// tokenize by COMMA
		token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
		s_buf.erase(0, pos + 1);		// cut first token
		//cout << token << endl;
		while ((pos = s_buf.find(COMMA)) != string::npos) {
			token = s_buf.substr(0, pos);	// tokenize by COMMA
			token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
			x_tmp = round(stod(token) * 10) / 10;
			//cout <<"x: " << x_tmp << endl;
			s_buf.erase(0, pos + 1);
		}
		token = s_buf;
		token = token.substr(1, token.length() - 2);
		y_tmp = round(stod(token) * 10) / 10;		// Last token
		//cout << "y: " << y_tmp << endl;
		city_list[j].set_pos(x_tmp, y_tmp);
		city_list[j].setID(j+1);
		j++;
		if (j >= city_list.size()) break;
	}
}
void read_PlantINFO() {
	int id;
	double x_tmp, y_tmp;
	long double gen;
	string s_buf, token;
	size_t pos;
	file_name = "plantInfo.csv";
	ifstream input(file_name);
	if (!(bool)input) {		// Check file exist
		cout << "Cannot find file name " << file_name << endl;
		return;
	}
	getline(input, s_buf, NEW_LINE);	// skip first index line
	while (input.good()) {
		// read line by line
		getline(input, s_buf, NEW_LINE);
		if (s_buf == "") break;
		pos = s_buf.find(COMMA);
		token = s_buf.substr(0, pos);	// tokenize by COMMA
		token = token.substr(1, token.length() - 2);	// treaming first and last double quatation

		id = stoi(token);				// get plant id
		s_buf.erase(0, pos + 1);		// cut first token

		// get x, y coordinate
		pos = s_buf.find(COMMA);
		token = s_buf.substr(0, pos);	// tokenize by COMMA
		token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
		x_tmp = stod(token);

		pos = s_buf.find(COMMA);
		s_buf.erase(0, pos + 1);
		token = s_buf.substr(0, pos);	// tokenize by COMMA
		token = token.substr(1, token.length() - 2);	// treaming first and last double quatation
		y_tmp = stod(token);
		s_buf.erase(0, pos + 1);

		// get yield
		pos = s_buf.find(COMMA);
		token = s_buf;
		token = token.substr(1, token.length() - 2);
		gen = stod(token);		// Last token	get yield
		plant tmp_plant(id, x_tmp, y_tmp, gen);
		plant_list.push_back(tmp_plant);
	}
}
void update() {
	int p;
	for (p = 0; p < plant_list.size(); p++) {
		plant_list[p].updateCity(city_list);
	}
}
int main() {
	//read to read file
	int i,m_buf, cont=0, stop=0;
	cout << "Reading data..." << endl;
	read_CityDATA();
	read_CityINFO();
	cout << "Reading plant INFO..." << endl;
	read_PlantINFO();
	
	/*==================================================================*/
	/*Regression TEST
	cout << "=====CITY 1 regression test=======" << endl;
	city_list[0].printData();
	while (1) {
		if (stop == 1) break;
		for (i = 0; i < THRESHOLD; i++) {
			cout << "Regression processed..." << endl;
			city_list[0].Regression();
			city_list[0].printData();
		}
		cin >> stop;
	}
	*/

	/*
	for (i = 0; i < plant_list.size(); i++) {
		cout << "=====PLANT " << i + 1 << "==========" << endl;
		plant_list[i].showInfo(1);
	}
	*/
	cout << "Type the month want to clustering[1,12]: ";
	cin >> m_buf;
	if (m_buf > 12 || m_buf < 1) return 0;

	/*
	cout << "Clustering for " << m_buf << " month without weighted" << endl;
	clustering(&city_list, &plant_list, m_buf, 0);
	// Show clustering result
	for (int j = 0; j < plant_list.size(); j++) {
	cout << "=====PLANT " << j + 1 << "==========" << endl;
	plant_list[j].showInfo(m_buf);
	}
	*/

	/*===============================
	* Origin data
	================================*/
	// Clustering
	cout << "Original Clustering result of " << m_buf <<" month"<< endl;
	clustering(&city_list, &plant_list, m_buf, 1);
	// Show clustering result
	printf("Plant\tYield\tUsage(%%)\t\tCity\n");
	for (int j = 0; j < plant_list.size(); j++) {
		cout << j + 1 << "\t";
		plant_list[j].showInfo(m_buf);
	}
	origin = city_list;	// back up original data
	while (1) {
		/*===============================
		* Regression
		================================*/
		// Regression
		cout << "Regression processed..." << endl;
		for (i = 0; i < city_list.size(); i++) {
			city_list[i].Regression();
		}

		/*===============================
		* Prediction
		================================*/
		/*
		// Clustering
		cout << "Type the month want to simulate[1,12]: ";
		cin >> m_buf;
		if (m_buf > 12 || m_buf < 1)break;
		cout << "Clustering for " << m_buf << " month" << endl;
		clustering(&city_list, &plant_list, m_buf, 1);
		*/
		update();
		// Show clustering result
		printf("Plant\tYield\tUsage(%%)\t\tCity\n");
		for (int j = 0; j < plant_list.size(); j++) {
			cout << j + 1 << "\t";
			plant_list[j].showInfo(m_buf);
		}
		for (int j = 0; j < plant_list.size(); j++) {
			if (plant_list[j].chkOverflow(m_buf)) {
				printf("Overflow is predicted... re-clustering\n");
				//clustering(&city_list, &plant_list, m_buf, 1);
				move(&plant_list, m_buf);
				cont = stop =0;
				break;
			}
		}
		printf("cont: %d\n", cont);
		cont++;
		// 3번 연속 통과
		if (cont == THRESHOLD) {
			city_list = origin;
			cont = 0;
			if (stop != 1) {
				stop = 1; continue;
			}

			printf("Clustering is finished...\nRecommended clustering is...\n");
			printf("Plant\tCity\n");
			for (int j = 0; j < plant_list.size(); j++) {
				cout << j + 1 << "\t";
				plant_list[j].showCluster(m_buf);
			}
			break;
		}
	}
		
	cin >> m_buf;
}