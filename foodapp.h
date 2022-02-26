#ifndef FOODAPP_H
#define FOODAPP_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <vector>

#include "restaurant.h"
#include "database.h"

class FoodApp
{
public:
    FoodApp();
    //test method - delete later
    void printRestaurants() const;

    void getRestaurants() const;

    void startingAtSaddleback() const;

    void planningTrip() const;

    void entireTrip(int idR, bool fromSaddleback) const;
private:
    Database db;
    std::vector<Restaurant> restaurants;
};

#endif // FOODAPP_H
