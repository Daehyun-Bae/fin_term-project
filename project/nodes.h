#include <vector>

class position {
private:
	float x;
	float y;
public:
	float posX() {
		return x;
	}
	float posY() {
		return y;
	}
};
class city {
	int plant_id;
	int cluster_id;
	position pos;
	std::vector<float> reg_usage;
	std::vector<float[12]> ann_usage;
public:
	float get_posX() {
		return pos.posX();
	}
	float get_posY() {
		return pos.posY();
	}
	float dist(city dst) {
		return sqrt(powf(pos.posX() - dst.get_posX.get_posX(), 2) +
			powf(pos.posY() - dst.get_posX.get_posY(), 2));
	}
};
class plant {
	int plant_id;
	float mon_gen;
	float storage;
};