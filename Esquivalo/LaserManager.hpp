//
//  LaserManager.hpp
//  LaserLibrary
//
//  Created by Laura del Pino Díaz on 23/3/16.
//  Copyright © 2016 Laura del Pino Díaz. All rights reserved.
//

#ifndef LaserManager_hpp
#define LaserManager_hpp

#include <iostream>
#include "Urg_driver.h"
#include "Connection_information.h"
#include "math_utilities.h"
#include "urg_utils.h"
#include <vector>
#include <limits>

class PlanePoint{

public:
	long x;
	long y;
	PlanePoint(long x, long y){
		this->x = x;
		this->y = y;
	}

};




class LaserManager{
	static qrk::Connection_information information;
	static int vertex[4];
	static qrk::Urg_driver urg;

public:


	LaserManager(int argc, char** argv);

	static bool open();
	static void setWorkSpaceVertex(int right, int top, int left, int bottom);
	static PlanePoint readLaser();
	static PlanePoint getMinDistance(std::vector<long> data);
	static PlanePoint translateToPlanePoint(double radian, long distance);
    static void discartZeros(std::vector<long>* data );

};

#endif /* LaserManager_hpp */
