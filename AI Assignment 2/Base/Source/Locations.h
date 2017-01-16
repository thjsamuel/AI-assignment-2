#ifndef LOCATIONS_H
#define LOCATIONS_H

#include "Vector3.h"

// Entrance and exit locations
#define ENTRANCE			Vector3(20, 8)
#define BEFORE_ENTRANCE		Vector3(20, 30)
#define EXIT				Vector3(150, 10)

// Kitchen appliances
#define STOVE				Vector3(5, 85)
#define FRIDGE				Vector3(25, 89)

// Where the food is collected by waiter inside
#define COUNTER				Vector3(40, 70)

// For the Cleaner
#define BEFORE_TRASHBIN		Vector3(93, 70)
#define TRASHBIN			Vector3(93, 91.8)

// For them to answer nature's call
#define BEFORE_TOILET		Vector3(118.3, 70)
#define TOILET				Vector3(118.3, 91.8)

#define USHERING            ENTRANCE + Vector3(-8, 0)

// Seats
#define SEAT_1				Vector3(7.48, 65.6)
#define SEAT_2				Vector3(7.48, 48.6)
#define SEAT_3				Vector3(7.48, 30.6)
#define SEAT_4				Vector3 (50.6, 24.3)
#define SEAT_5				Vector3 (70.6, 24.3)
#define SEAT_6				Vector3 (90.6, 24.3)
#define SEAT_7				Vector3 (125.48, 65.6)
#define SEAT_8				Vector3 (125.48, 48.6)
#define SEAT_9				Vector3 (125.48, 30.6)

#endif // LOCATIONS_H