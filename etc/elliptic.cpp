#include <iostream>
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;
using namespace std;

class point {
public:

	int a;
	int b;
	int256_t x;
	int256_t y;

	point(int a, int b, int256_t x, int256_t y) {

		cpp_int val = boost::multiprecision::pow(cpp_int(x), 3);
		int256_t v(val);

		assert(y * y == v + a * x + b || (x == NULL && y == NULL));
		//if x=null and y=null , poit at infinity

		this->a = a;
		this->b = b;
		this->x = x;
		this->y = y;

	}

	bool equiv(point& c) {
		if (this->a == c.a && this->b == c.b && this->x == c.x && this->y == c.y) {
			return true;
		}
		else {
			return false;
		}
	}

	point add(point& c) {
		assert(this->a == c.a && this->b == c.b);

		point res(this->a, this->b, 0, 0);
		//if there is a point at infinity ; (Null, Null)
		if (this->x == NULL) { 
			res.x = c.x;
			res.y = c.y;
		}
		else if (c.x == NULL) {
			res.x = this->x;
			res.y = this->y;
		}
		else if ((this->x == c.x) && (this->y == - c.y) && (this->y != NULL)){ //parallel with y-axis
			res.x = NULL;
			res.y = NULL;
		}
		else if (this->x != c.x) {
			int256_t s = (c.y - this->y) / (c.x - this->x);
			res.x = s * s - this->x - c.x;
			res.y = s * (this->x - res.x) - this->y;
		}
		else if ((this->x == c.x) && (this->y == c.y) && (this->y != NULL)) {
			int256_t s = (3 * c.x * c.x + c.a) / (2 * c.y);
			res.x = s * s - 2 * c.x;
			res.y = s * (c.x - res.x) - c.y;
		}
		else if ((this->x == c.x) && (this->y == c.y) && (this->y == NULL)) {
			res.x = NULL;
			res.y = NULL;
		}
			
		return res;

	}

};

int main() {

	point p1(5, 7, -1, -1);
	//point p2(5, 7, -1, -2); this point is not in curve
	point p2(5, 7, -1, 1);

	point result = p1.add(p2);
	cout << result.x << endl;
	cout << result.y << endl;

	
}
