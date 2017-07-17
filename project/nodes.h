#include <vector>
#include <iostream>
#include "regression.h"


class position {
private:
	double x;
	double y;
public:
	position() {
		x = 0; y = 0;
	}
	double posX() {
		return x;
	}
	double posY() {
		return y;
	}
	void setPos(double x_val, double y_val){
		x = x_val; y = y_val;
	}
};
class city {
	int plant_id;
	int city_id;
	bool assigned;
	position pos;
	std::vector<double> reg_usage;
	std::vector<std::vector<double>> ann_usage;
public:
	city() {
		plant_id = 0; city_id = 0;
		assigned = false;
		reg_usage.resize(12, 0);
	}
	double get_posX() {
		return pos.posX();
	}
	double get_posY() {
		return pos.posY();
	}
	void set_pos(double x, double y) {
		pos.setPos(x, y);
	}
	void setID(int n) { city_id = n; }
	void assignPlant(int n) { plant_id = n; }
	bool getAssign() { return assigned; }
	void chAssign() { assigned = !assigned; }
	int getPlantID() { return plant_id; }
	int getCityID() { return city_id; }
	void insertData(std::vector<double> vec) {
		ann_usage.push_back(vec);
		//vec.clear();
	}
	double getData(int month) {
		return ann_usage[ann_usage.size() - 1][month - 1];		// return latest year's data
	}
	void clearData() {
		ann_usage.clear();
	}
	void printData() {
		int i, j;
		cout << "ID: " << city_id << endl;
		std::cout << "Position X: " << get_posX() << "Y: " << get_posY() << std::endl;
		for (i = 0; i < ann_usage.size(); i++) {
			for (j = 0; j < ann_usage[i].size(); j++)
				std::cout << "\t" << ann_usage.at(i).at(j);
			std::cout << std::endl;
		}
	}
	void Regression() {
		reg_usage.clear();
		regression(ann_usage, &reg_usage);
		/*
		printf("Regression result======\n");
		for (int i = 0; i < reg_usage.size(); i++) std::cout << "\t" << reg_usage[i];
		std::cout << std::endl;
		*/
		ann_usage.push_back(reg_usage);
		ann_usage.erase(ann_usage.begin());
		//printData();
	}
};
class plant {
	int plant_id;
	position pos;
	double yield;
	double storage;
	bool isFull;
	vector<city> cluster;
public:
	plant() {
		plant_id = 0;
		yield = 0;
		storage = 10000;
	}
	plant(int id, double x, double y, double yld) {
		plant_id = id;
		pos.setPos(x, y);
		yield = yld;
		isFull = false;
		storage = 10000;
	}
	void updateCity(vector<city> city_list) {
		for (int i = 0; i < cluster.size(); i++) {
			for (int c = 0; c < city_list.size(); c++) {
				if (cluster[i].getCityID() == city_list[c].getCityID())
					cluster[i] = city_list[c];
			}
		}
	}
	city popMincity(int month) {
		city res = cluster[0];
		int k = 0;
		for (int i = 0; i < cluster.size(); i++) {
			if (cluster[i].getData(month) < res.getData(month)) {
				res = cluster[i];
				k = i;
			}
		}
		cluster.erase(cluster.begin() + k);
		return res;
	}
	double get_posX() {
		return pos.posX();
	}
	double get_posY() {
		return pos.posY();
	}
	double get_yield() {
		return yield;
	}
	bool isfull() { return isFull; }
	int getId() { return plant_id; }
	void clearCluster() {
		cluster.clear();
		isFull = false;
		//storage += yield;
		//yield = 0;
	}
	void switchOFF() { isFull = true; }
	void switchON() { isFull = false; }
	void pushCity(city c) {
		//printf("city ID %d ...\n", c.getCityID());
		for (int i = 0; i < cluster.size(); i++) if (cluster[i].getCityID() == c.getCityID()) {
			//printf("city ID %d already exist at plant %d\n", c.getCityID(), plant_id);
			return;
		}
		
		cluster.push_back(c);
		//printf("city ID %d is pushed...\n", c.getCityID());
	}
	void showInfo(int m) {
		std::cout << yield << "\t" << getUsage(m) << " (" << (getUsage(m)/yield)*100 << "%)";
		std::cout << "\t";
		for (int i = 0; i < cluster.size(); i++) printf(" %d", cluster[i].getCityID());
		std::cout << std:: endl;
	}
	void showCluster(int m) {
		for (int i = 0; i < cluster.size(); i++) printf(" %d", cluster[i].getCityID());
		std::cout << std::endl;
	}
	void setYield(double y) {
		yield = y;
	}
	void storing(double acc) {
		storage += acc;
	}
	double getUsage(int m) {
		double res = 0;
		for (int i = 0; i < cluster.size(); i++) res += cluster[i].getData(m);
		return res;
	}
	bool chkOverflow(int m) {
		if (getUsage(m) > yield) return true;
		else return false;
	}
};

double dist(plant src, city dst) {
	return sqrt(pow(src.get_posX() - dst.get_posX(), 2) + pow(src.get_posY() - dst.get_posY(), 2));
}