/*
 *	easing.net's formula's / code translated into c++
 */

#include <cmath>
#include <cstdlib>

using namespace std;

inline float constrain(float val, float big, float small){
	return min( max(val, big), small);
}

float easeInSine(float x) {
	x = constrain(x, 0.0, 1.0);
	return 1.0 - cos((x * M_PI) / 2.0);
}

float easeOutSine(float x){
	x = constrain(x, 0.0, 1.0);
	return sin((x * M_PI) / 2.0);
}

float easeInOutSine(float x){
	x = constrain(x, 0.0, 1.0);
	return -1.0 *(cos(M_PI * x) - 1.0) / 2.0;
}

float easeInQuad(float x){
	x = constrain(x, 0.0, 1.0);
	return x * x;
}

float easeOutQuad(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - (1.0 - x) * (1.0 - x);
}

float easeInOutQuad(float x){
	x = constrain(x, 0.0, 1.0);
	
	float result = 0.0;
	
	if(x < 0.5){
		result = 2.0 * x * x;
	} else {
		result = 1.0 - pow((-2.0 * x + 2.0), 2.0) / 2.0;
	}
	
	return result;
}

float easeInCubic(float x){
	x = constrain(x, 0.0, 1.0);
	return x * x * x;
}

float easeOutCubic(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - pow(1.0 - x, 3.0);
}

float easeInOutCubic(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if( x < 0.5 ){
		result = 4.0 * x * x * x;
	} else{
		result = 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;
	}
	return result;
}

float easeInQuart(float x){
	x = constrain(x, 0.0, 1.0);
	return x * x * x * x;
}

float easeOutQuart(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - pow(1.0 - x, 4.0);
}

float easeInOutQuart(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if( x < 0.5 ){
		result = 8.0 * x * x * x * x ;
	} else{
		result = 1.0 - pow(-2.0 * x + 2.0, 4.0) / 2.0;
	}
	return result;
}

float easeInQuint(float x){
	x = constrain(x, 0.0, 1.0);
	return x * x * x * x * x;
}

float easeOutQuint(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - pow(1.0 - x, 5.0);
}

float easeInOutQuint(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if( x < 0.5 ){
		result = 16.0 * x * x * x * x * x ;
	} else{
		result = 1.0 - pow(-2.0 * x + 2.0, 5.0) / 2.0;
	}
	return result;
}

float easeInExpo(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if (x == 0.0){
		result = 0.0;
	} else{
		result = pow(2.0, 10.0 * x - 10.0);
	}
	return result;
}

float easeOutExpo(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if(x == 1.0){
		result = 1.0;
	} else{
		result = 1.0 - pow(2.0, -10.0 * x);
	}
	return result;
}

float easeInOutExpo(float x){
	x = constrain(x, 0.0, 1.0);
	float result = 0.0;
	if(x == 0.0){
		result =  0.0;
	} else if (x == 1.0) {
		result = 1.0;
	} else if(x < 0.5){
		result = pow(2.0, 20 * x - 10) / 2.0;
	} else {
		result = (2.0 - pow(2.0, -20.0 * x + 10.0)) / 2.0;
	}
	return result;
}

float easeInCirc(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - sqrt(1.0 - pow(x, 2.0));
}

float easeOutCirc(float x){
	x = constrain(x, 0.0, 1.0);
	return sqrt(1.0 - pow(x - 1.0, 2.0));
}

float easeInOutCirc(float x){
	x = constrain(x, 0.0, 1.0);
	
	float result;
	
    if (x < 0.5) {
        result = (1.0 - sqrt(1.0 - pow(2.0 * x, 2.0))) / 2.0;
    } else {
        result = (sqrt(1.0 - pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
    }
    return result;
}

float easeInBack(float x){
	x = constrain(x, 0.0, 1.0);
	const float c1 = 1.70158;
	const float c3 = c1 + 1.0;

	return c3 * x * x * x - c1 * x * x;
}

float easeOutBack(float x){
	x = constrain(x, 0.0, 1.0);
	const float c1 = 1.70158;
	const float c3 = c1 + 1.0;

	return 1.0 + c3 * pow(x - 1.0, 3.0) + c1 * pow(x - 1.0, 2.0);
}

float easeInOutBack(float x){
	x = constrain(x, 0.0, 1.0);
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;
	float result = 0.0;

	if( x < 0.5){
		result = (pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0;
	} else{
		result = (pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) + 2.0) / 2.0;
	}
	return result;
}

float easeInElastic(float x){
	x = constrain(x, 0.0, 1.0);
	const float c4 = (2.0 * M_PI) / 3.0;
	float result = 0.0;
	if(x == 0){
		result = 0.0;
	} else if(x == 1.0){
		result = 1.0;
	} else{
		result = -1.0 * pow(2.0, 10.0 * x - 10.0) * sin((x * 10.0 - 10.75) * c4);
	} 
	return result;
}

float easeOutElastic(float x){
	x = constrain(x, 0.0, 1.0);
	const float c4 = (2.0 * M_PI) / 3.0;
	float result = 0.0;
	
	if(x == 0){
		result = 0.0;
	} else if(x == 1.0){
		result = 1.0;
	} else{
		result = pow(2.0, -10.0 * x) * sin((x * 10.0 - 0.75) * c4) + 1.0;
	}
	return result;
}

float easeInOutElastic(float x){
	x = constrain(x, 0.0, 1.0);
	const float c5 = (2.0 * M_PI) / 4.5;
	float result = 0.0;
	
	if(x == 0){
		result = 0.0;
	} else if(x == 1.0){
		result = 1.0;
	} else if (x < 0.5){
		result = -1.0 * (pow(2.0, 20.0 * x - 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0;
	} else {
		result = (pow(2.0, -20.0 * x + 10.0) * sin((20.0 * x - 11.125) * c5)) / 2.0 + 1.0;
	}
	return result;
}

float easeOutBounce(float x){
	const float n1 = 7.5625;
	const float d1 = 2.75;
	
	float result = 0.0;
	
	x = constrain(x, 0.0, 1.0);

	if (x < 1.0 / d1) {
		result = n1 * x * x;
	} else if (x < 2.0 / d1) {
		result = n1 * (x -= 1.5 / d1) * x + 0.75;
	} else if (x < 2.5 / d1) {
		result = n1 * (x -= 2.25 / d1) * x + 0.9375;
	} else {
		result = n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
	
	return result;
}

float easeInBounce(float x){
	x = constrain(x, 0.0, 1.0);
	return 1.0 - easeOutBounce(1.0 - x);
}

float easeInOutBounce(float x){
	float result = 0.0;
	
	x = constrain(x, 0.0, 1.0);
	
	if(x < 0.5){
		result = ( (1.0 - easeOutBounce(1.0 - 2.0 * x)) / 2.0);
	} else {
		result = (1.0 + easeOutBounce(2.0 * x - 1.0)) / 2.0;
	}
	return result;
}