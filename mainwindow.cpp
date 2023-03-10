#include "mainwindow.h"

#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loginpopup = nullptr;

    //Initialize our current trip pointer to nullptr
    this->currentTrip = nullptr;
    //use our db object to populate our restaurants vector with Restaurant objects
    if(db.getRestaurants(this->restaurants)){
        qInfo() << "Successfully got all the restaurants";
    }

    //set up ui
    //This is the list widget that holds each restaurant row
    QListWidget *restaurantList = ui->restaurantList;

    //Create a RestaurantWidget object for each restaurant and add it to the restaurantList list widget.
    for (int i = 0; i < this->restaurants.size(); i++)
    {
        RestaurantWidget *restaurantItem = new RestaurantWidget(restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(restaurantList);
        restaurantList->addItem(item);
        item->setSizeHint(restaurantItem->minimumSizeHint());
        restaurantList->setItemWidget(item, restaurantItem);
    }

    ui->priceText->setValidator(new QDoubleValidator(0, 100, 2, this));

}

//Purely for adding restaurant to UI list
void MainWindow::addToList(QString name)
{
    QListWidget* customList = ui->customList;

    if(std::find(nameList.begin(), nameList.end(), name) == nameList.end())
    {

       customList->addItem(name);
       nameList.push_back(name);

    }
    else
    {
        qInfo() << "Cannot add same Restaurant to the trip!";
    }


}

//Purely for adding menu item to UI list
void MainWindow::addToMenuList(QString name, QString restaurantName)
{
    QListWidget* customList = ui->menuList;

    if(std::find(nameList.begin(), nameList.end(), restaurantName) != nameList.end())
    {
        customList->addItem(name);
        menuList.push_back(name);
    }
    else
    {
        qInfo() << "Cannot add Menu Item without adding Restaurant to trip!";
    }
}

//Needed for calculating the trip information
void MainWindow::addRestaurant(Restaurant restaurant){

    qInfo() << restaurant.getName() << "Test";


    if(std::find(nameList.begin(), nameList.end(), restaurant.getName()) == nameList.end() )
    {
        //Restaurant hasn't been added yet, good to add
        selectedRestaurants.push_back(restaurant);
    } else {
        QMessageBox popup;
        popup.critical(0, "Error", "Cannot add the same restaurant twice.");
    }
}

void MainWindow::addMenuItem(Restaurant restaurant, MenuItem item){

    if(std::find(nameList.begin(), nameList.end(), restaurant.getName()) != nameList.end())
    {
        //Restaurant this menu items belongs to has been added to trip already. Safe to add menu item.
        selectedItems.insert(std::pair<int, MenuItem>(restaurant.getID(), item));
    }
    else
    {
        QMessageBox popup;
        popup.critical(0, "Error", "Cannot add menu item before adding restaurant to trip.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_planTrip_clicked()
{
    //Set this to true if we run into input validation
    bool errorExists = false;

    if(this->nameList.size() > 0 && this->menuList.size() > 0){
        if (ui->startDominos->isChecked()){
            std::cout << "Start from dominos checked and were calling";
            if(std::find(nameList.begin(), nameList.end(), "Domino's Pizza") != nameList.end())
            {
                //set current trip pointer in this class to a new trip object which will encapsulate all the information of our trip. access with getters.
                this->currentTrip = new Trip(ui->startSaddleback->isChecked(), ui->startDominos->isChecked(), this->selectedRestaurants, this->selectedItems);
            } else {
                QMessageBox popup;
                errorExists = true;
                popup.critical(0, "Error", "Cannot start a trip from Domino's without adding it first.");
            }
        } else {
            this->currentTrip = new Trip(ui->startSaddleback->isChecked(), ui->startDominos->isChecked(), this->selectedRestaurants, this->selectedItems);
        }
    } else {
        QMessageBox popup;
        errorExists = true;
        popup.critical(0, "Error", "Cannot start a trip without adding any menu items or restaurants.");
    }

    if (!errorExists){
        //Input is properly validated, continue with displaying trip information to UI

        qInfo() << "Total money spent: " << this->currentTrip->getTotalSpent();
        ui->totalSpentLabel->setText("$" + QString::number(this->currentTrip->getTotalSpent()));


        ui->distanceLabel->setText(QString::number(this->currentTrip->getTotalDistance()) + " mi.");

        std::queue<Restaurant> route = this->currentTrip->getRoute();

        qInfo() << "Most efficient route: ";

        int count = 1;
        if (this->currentTrip->isFromSaddleback()){
            ui->routeList->addItem("1. Saddleback College");
            count++;
        }

        while (route.empty() != true){
            qInfo() << route.front().getName();
            ui->routeList->addItem(QString::number(count) + ". " + route.front().getName());
            count++;
            route.pop();
        }

        qInfo() << "Money spent per restaurant: ";

        std::map<int,double> moneySpent = this->currentTrip->getMoneySpent();
        std::map<int,double>::iterator it;
        std::vector<Restaurant> allRestaurants = this->restaurants;
        for (it = moneySpent.begin(); it != moneySpent.end(); it++){
            int id = it->first;
            for (int i = 0; i < allRestaurants.size(); i++){
                if (allRestaurants[i].getID() == id){
                    qInfo() << "Money spent at " << allRestaurants[i].getName() << ": " << it->second;
                    QString string = "Money spent at " + allRestaurants[i].getName() + ": $" + QString::number(it->second);
                    ui->spentList->addItem(string);
                }
            }

        }
    }

}


void MainWindow::on_openListButton_clicked()
{
    popup = new listOfRestaurants(this->restaurants, this);
    popup->show();
}


void MainWindow::on_clearButton_clicked()
{
    this->nameList.clear();
    this->menuList.clear();

    this->ui->customList->clear();
    this->ui->menuList->clear();
    this->ui->routeList->clear();
    this->ui->spentList->clear();
    this->ui->startDominos->setChecked(false);
    this->ui->startSaddleback->setChecked(false);
    this->ui->customTrip->setChecked(false);

    //Reset trip information
    currentTrip = nullptr;
    selectedRestaurants.clear();
    selectedItems.clear();
}


void MainWindow::on_customTrip_clicked()
{
    QMessageBox popup;
    popup.information(0, "Info", "The first restaurant you add to the trip will be the starting restaurant.");
}


void MainWindow::on_pushButton_clicked()    //admin login
{
    this->loginpopup = new Login();
    loginpopup->show();
}


void MainWindow::on_startDominos_clicked()
{
    int numRestaurantsToVisit = this->ui->numRestaurants->value();
    if (numRestaurantsToVisit > 0){
        //all restaurants were visiting (including dominos)

        //clear any currently selected restaurants
        this->selectedRestaurants.clear();
        this->selectedItems.clear();

        std::vector<Restaurant> restaurantsFromDominos;
        //previous restaurant to compare for in algorithm
        Restaurant *previous;
        //set previous equal to dominos restaurant
        for (int i = 0; i < restaurants.size(); i++){
            if (restaurants[i].getID() == 3){
                //3 is domino's id
                previous = &restaurants[i];
                qInfo() << "found dominos successfully";
            }
        }

        restaurantsFromDominos.push_back(*previous);
        double distanceTotal = 0;

        for (int i = 0; i < numRestaurantsToVisit - 1; i++){
            qInfo() << "Ran";
            double distance = 10000000;
            Restaurant* closest;


            for (int w = 0; w < this->restaurants.size(); w++){
                double distance2 = 0;
                if (previous->getID() > 10){
                    qInfo() << "New one";
                    qInfo() << w;
                    distance2 = previous->getDistances().at(w);
                } else {
                    distance2 = restaurants[w].getDistances().at(previous->getID() - 1);
                }
                //need some sort of alternate way for if previous is a new restaurant.. check size of .getDistances like in Trip
                if (distance2 < distance){
                    //found new restaurant, but first make sure it isnt in restaurantsFromDominos already!
                    bool found = false;
                    for (int j = 0; j < restaurantsFromDominos.size(); j++){
                        if (restaurantsFromDominos[j].getID() == restaurants[w].getID()){
                            qInfo() << "oops.. same restaurant";
                            found = true;
                        }
                    }
                    if (found == false){
                        qInfo() << "new restaurant! " << restaurants[w].getName() << "dis: " << distance2;
                        closest = &restaurants[w];
                        distance = distance2;
                    }
                }
            }

            restaurantsFromDominos.push_back(*closest);
            previous = closest;
            distanceTotal += distance;

        }

        selectedRestaurants = restaurantsFromDominos;

        //add restaurants to ui now
        for (int i = 0; i < selectedRestaurants.size(); i++){
            this->addToList(selectedRestaurants[i].getName());
        }

        QString restaurantsStr;
        for (int i = 0; i < restaurantsFromDominos.size(); i++){
            restaurantsStr += restaurantsFromDominos[i].getName() + " \n";
        }


        QMessageBox popup;
        popup.information(0, "Info", "The restaurants you will be visiting are: \n \n" + restaurantsStr + "\nPlease add items for each of these. \n" + "Total distnace: " + QString::number(distanceTotal) + " mi.");


    } else {
        QMessageBox popup;
        popup.critical(0, "Error", "Enter number of restaurants to visit first.");
    }

            //add to selected restaurants from here after
}


void MainWindow::on_pushButton_2_clicked()
{
    if (this->loginpopup != nullptr){
        if (this->loginpopup->getAdminStatus() == true){
            if (this->restaurants.size() > 10){
                QMessageBox popup;
                popup.critical(0, "Error", "Restaurant file has already been read!");
            }

            qInfo() << "Logged in as admin, good to go.";

            std::vector<double> distances;
            std::vector<MenuItem> menu;

            std::vector<Restaurant> newRestaurants = this->db.readFile();

            this->db.addRestaurants(newRestaurants);

            this->db.getRestaurants(this->restaurants);

            QListWidget *restaurantList = ui->restaurantList;
            restaurantList->clear();

            //Create a RestaurantWidget object for each restaurant and add it to the restaurantList list widget.
            for (int i = 0; i < this->restaurants.size(); i++)
            {
                RestaurantWidget *restaurantItem = new RestaurantWidget(restaurants[i], this);
                QListWidgetItem *item = new QListWidgetItem(restaurantList);
                restaurantList->addItem(item);
                item->setSizeHint(restaurantItem->minimumSizeHint());
                restaurantList->setItemWidget(item, restaurantItem);
            }


            qInfo() << newRestaurants[1].getName();
            qInfo() << newRestaurants[1].getID();

            distances = newRestaurants[1].getDistances();
            qInfo() << newRestaurants[1].getSaddlebackDistance();
            menu = newRestaurants[1].getMenu();

            for (auto i = distances.begin(); i != distances.end(); i++)
            {
                qInfo() << *i;
            }

            for (auto j = menu.begin(); j != menu.end(); j++)
            {
                qInfo() << j->name << " " << j->price;
            }


            //qInfo() << newRestaurants[1].getName();




        } else {
            QMessageBox popup;
            popup.critical(0, "Error", "This is an admin only feature.");
        }
    } else {
        QMessageBox popup;
        popup.critical(0, "Error", "This is an admin only feature. \nLog in first!");
    }
}


void MainWindow::on_menuAdmin_clicked()
{
    qInfo() << this->loginpopup;
    if (this->loginpopup != nullptr){
        if (this->loginpopup->getAdminStatus() == true){
            qInfo() << "Logged in as admin, good to go.";

            //loop through restaurants and create a MenuWidget for each one
            QListWidget *menuList = ui->menuAdminList;
            menuList->clear();

            for (int i = 0; i < restaurants.size(); i++){
                MenuWidget *menuItem = new MenuWidget(this->restaurants[i], this);
                QListWidgetItem *item = new QListWidgetItem(menuList);
                menuList->addItem(item);
                item->setSizeHint(menuItem->minimumSizeHint());
                menuList->setItemWidget(item, menuItem);
            }


        } else {
            QMessageBox popup;
            popup.critical(0, "Error", "This is an admin only feature.");
        }
    } else {
        QMessageBox popup;
        popup.critical(0, "Error", "This is an admin only feature. \nLog in first!");
    }
}

void MainWindow::deleteItem(Restaurant restaurant, std::vector<MenuItem> newMenu){
    //menu is already updated for this function call
    this->db.modifyMenu(restaurant, newMenu);

    //update restaurants
    this->db.getRestaurants(this->restaurants);

    //update ui

    QListWidget *restaurantList = ui->restaurantList;
    restaurantList->clear();

    //Create a RestaurantWidget object for each restaurant and add it to the restaurantList list widget.
    for (int i = 0; i < this->restaurants.size(); i++)
    {
        RestaurantWidget *restaurantItem = new RestaurantWidget(restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(restaurantList);
        restaurantList->addItem(item);
        item->setSizeHint(restaurantItem->minimumSizeHint());
        restaurantList->setItemWidget(item, restaurantItem);
    }

    //loop through restaurants and create a MenuWidget for each one
    QListWidget *menuList = ui->menuAdminList;
    menuList->clear();

    for (int i = 0; i < restaurants.size(); i++){
        MenuWidget *menuItem = new MenuWidget(this->restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(menuList);
        menuList->addItem(item);
        item->setSizeHint(menuItem->minimumSizeHint());
        menuList->setItemWidget(item, menuItem);
    }
}

void MainWindow::editItem(Restaurant restaurant, int index){
    std::vector<MenuItem> newMenu = restaurant.getMenu();
    MenuItem editing = restaurant.getMenu()[index];
    newMenu.erase(newMenu.begin() + index);
    double newPrice = std::stod(ui->priceText->text().toStdString());
    editing.price = newPrice;

    newMenu.push_back(editing);

    this->db.modifyMenu(restaurant, newMenu);

    //update restaurants
    this->db.getRestaurants(this->restaurants);

    //update ui

    QListWidget *restaurantList = ui->restaurantList;
    restaurantList->clear();

    //Create a RestaurantWidget object for each restaurant and add it to the restaurantList list widget.
    for (int i = 0; i < this->restaurants.size(); i++)
    {
        RestaurantWidget *restaurantItem = new RestaurantWidget(restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(restaurantList);
        restaurantList->addItem(item);
        item->setSizeHint(restaurantItem->minimumSizeHint());
        restaurantList->setItemWidget(item, restaurantItem);
    }

    //loop through restaurants and create a MenuWidget for each one
    QListWidget *menuList = ui->menuAdminList;
    menuList->clear();

    for (int i = 0; i < restaurants.size(); i++){
        MenuWidget *menuItem = new MenuWidget(this->restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(menuList);
        menuList->addItem(item);
        item->setSizeHint(menuItem->minimumSizeHint());
        menuList->setItemWidget(item, menuItem);
    }
}

void MainWindow::addItem(Restaurant restaurant, std::vector<MenuItem> newMenu){
    QString name = ui->itemnameText->toPlainText();
    double price = std::stod(ui->priceText->text().toStdString());

    MenuItem newItem;
    newItem.name = name;
    newItem.price = price;

    std::vector<MenuItem> menu = restaurant.getMenu();
    menu.push_back(newItem);

    this->db.modifyMenu(restaurant, menu);

    //update restaurants
    this->db.getRestaurants(this->restaurants);

    //update ui
    QListWidget *restaurantList = ui->restaurantList;
    restaurantList->clear();

    //Create a RestaurantWidget object for each restaurant and add it to the restaurantList list widget.
    for (int i = 0; i < this->restaurants.size(); i++)
    {
        RestaurantWidget *restaurantItem = new RestaurantWidget(restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(restaurantList);
        restaurantList->addItem(item);
        item->setSizeHint(restaurantItem->minimumSizeHint());
        restaurantList->setItemWidget(item, restaurantItem);
    }

    //loop through restaurants and create a MenuWidget for each one
    QListWidget *menuList = ui->menuAdminList;
    menuList->clear();

    for (int i = 0; i < restaurants.size(); i++){
        MenuWidget *menuItem = new MenuWidget(this->restaurants[i], this);
        QListWidgetItem *item = new QListWidgetItem(menuList);
        menuList->addItem(item);
        item->setSizeHint(menuItem->minimumSizeHint());
        menuList->setItemWidget(item, menuItem);
    }
}

