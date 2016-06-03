//
//  LaserManager.cpp
//  LaserLibrary
//
//  Created by Laura del Pino Díaz on 23/3/16.
//  Copyright © 2016 Laura del Pino Díaz. All rights reserved.
//

#include "LaserManager.hpp"

char* fake[1];
int LaserManager::vertex[4];
qrk::Connection_information LaserManager::information(1, fake);
qrk::Urg_driver LaserManager::urg;

using namespace std;


bool LaserManager::open(){
	if (!LaserManager::urg.open(
		LaserManager::information.device_or_ip_name(),
		LaserManager::information.baudrate_or_port_number(),
		LaserManager::information.connection_type()
                                )){
		cout << "Not opened" << endl;
        return false;
    }
	urg.set_scanning_parameter(urg.deg2step(-90), urg.deg2step(90), 0);
	urg.start_measurement(qrk::Urg_driver::Distance, qrk::Urg_driver::Infinity_times, 0);
    return true;
}

void LaserManager::setWorkSpaceVertex(int right, int top, int left, int bottom){
	LaserManager::vertex[0] = right;
	LaserManager::vertex[1] = top;
	LaserManager::vertex[2] = left;
	LaserManager::vertex[3] = bottom;

}

//TODO calculate centroide
//TODO calculate x and y values for every point in data.
PlanePoint LaserManager::readLaser(){
	std::vector<long> data;
	long time_stamp = 0;
	/***/
	if (urg.get_distance(data, &time_stamp)) {
		
		PlanePoint point = getMinDistance(data);
		
		return point;
	}
    else{
        for (int i = 0; i < 5; i++) {
            if (urg.get_distance(data, &time_stamp)) {
                PlanePoint point = getMinDistance(data);
                return point;
            }else{
                
                printf("bloqueado");
            }
        }
        urg.close();
        LaserManager::open();
        
        printf("Noob\n");
        return *new PlanePoint(0, 0);
    }
	
}


PlanePoint LaserManager::getMinDistance(std::vector<long> data)
{
	int indexMin = 0;
	long minDistance =  std::numeric_limits<long>::max();
	for (int i = 0; i < data.size(); i++)
	{
		int long distance = data[i];
		double radian = urg.index2rad(i) + 1.57077963267948966;
		long x = (long)(distance * cos(radian));
		long y = (long)(distance * sin(radian));
		if ((distance < minDistance)
			&& (y < LaserManager::vertex[1])
			&& (y > LaserManager::vertex[3])
			&& (x < LaserManager::vertex[0])
			&& (x > -LaserManager::vertex[2])
            && (y > 0)
			)
		{
			minDistance = data[i];
			indexMin = i;
		}
	}
	int long distance = data[indexMin];
	double radian = urg.index2rad(indexMin) + 1.57077963267948966;
	long x = (long)(distance * cos(radian));
	long y = (long)(distance * sin(radian));
	return *new PlanePoint(x,y);
}

