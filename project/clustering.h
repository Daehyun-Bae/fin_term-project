#include <vector>

using namespace std;

#define REPEAT 10

int numPlant, Month = 1;
double Max, Min;

void setVar(vector<city> cityList, vector<plant> plantList, int month) {
	Month = month;
	numPlant = plantList.size();
	Max = Min = cityList[0].getData(month);
	for (int i = 0; i < cityList.size(); i++) {
		if (cityList[i].getData(month) < Min) Min = cityList[i].getData(month);
		if (cityList[i].getData(month) > Max) Max = cityList[i].getData(month);
	}
	//printf("set variable... OK...\n");
}
double getWeight(double x) {			// scaling weight in range [1,2]
	return ((x - Min) / (Max - Min)) + 1;
}

double wDist(plant p, city c, int weighted) {
	if (weighted > 0) return dist(p, c)*getWeight(c.getData(Month));
	else return dist(p, c);
}

void clustering(vector<city> *cityList, vector<plant> *plantList, int month, int weighted) {
	int i, j, k; // j - plant || i - city;
	double tmp_min = INFINITY;
	setVar(*cityList, *plantList, month);
	//Initializeing
	for (i = 0; i < plantList->size(); i++) { plantList->at(i).clearCluster(); }
	for (i = 0; i < cityList->size(); i++) { if (cityList->at(i).getAssign()) cityList->at(i).chAssign(); }

	for (int e = 1; e <= REPEAT; e++) {
		//printf("%d iterate\n", e);
		for (i = 0; i < cityList->size(); i++) {
			if (cityList->at(i).getAssign()) continue;
			for (j = 0; j < plantList->size(); j++) {

				if (wDist(plantList->at(j), cityList->at(i), weighted) < tmp_min) {
					if (plantList->at(j).getUsage(month) + cityList->at(i).getData(month) < plantList->at(j).get_yield()) {
						tmp_min = wDist(plantList->at(j), cityList->at(i), weighted);
						k = j;			// keep the plant number
					}
				}
			}
			//cityList->at(i).assignPlant(plantList[k].getId());
			//printf("city %d ---- plant %d\n", i + 1, k + 1);
			/*if (plantList->at(k).getUsage(month) + cityList->at(i).getData(month) > plantList->at(k).get_yield()) {
				printf("overflow\n");
				//plantList->at(k).switchOFF();
				continue;
			}*/
			plantList->at(k).pushCity(cityList->at(i));
			cityList->at(i).chAssign();
			tmp_min = INFINITY;
		}
	}
}

void move(vector<plant> *plantList, int month) {
	//vector<plant> p_tmp = *plantList;
	//vector<city> c_tmp = *cityList;
	int MP, OP;
	double minUsage = INFINITY;
	for (int p = 0; p < plantList->size(); p++) {
		if (plantList->at(p).chkOverflow(month)) {
			OP = p;
		}
		if (((plantList->at(p).getUsage(month) / plantList->at(p).get_yield()) * 100) < minUsage) {
			minUsage = ((plantList->at(p).getUsage(month) / plantList->at(p).get_yield()) * 100);
			MP = p;
		}
	}
	plantList->at(MP).pushCity(plantList->at(OP).popMincity(month));
}