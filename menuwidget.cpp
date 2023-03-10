#include "menuwidget.h"

MenuWidget::MenuWidget(Restaurant restaurant, MainWindow *parent)
    : restaurant{restaurant}, parent{parent}
{

    //Creating all the UI elements dynamically for each restaurant column in the list widget.
    layout = new QHBoxLayout(this);
    nameLabel = new QLabel(restaurant.getName());
    nameLabel->setFixedWidth(110);
    addNewItemButton = new QPushButton("Add Item");
    addNewItemButton->setFixedSize(100, 40);
    menuItems = new QComboBox();
    deleteItemButton = new QPushButton("Delete item");
    deleteItemButton->setFixedSize(100, 40);
    editItemButton = new QPushButton("Edit item");
    editItemButton->setFixedSize(100, 40);

    //Iterating over the menu to add items to the menu ComboBox
    for (int i = 0; i < this->restaurant.getMenu().size(); i++)
    {
        menuItems->addItem(QString(this->restaurant.getMenu()[i].name + ": " + QString::number(this->restaurant.getMenu()[i].price)));
    }

    //Run on_AddMenuItemClicked() method in this class when "Add Items" button has been clicked
    addNewItemButton->connect(addNewItemButton, SIGNAL(clicked()), this, SLOT(on_AddMenuItemClicked()));
    //Run on_AddRestaurantClicked() method in this class when "Add Restaurant" button has been clicked
    deleteItemButton->connect(deleteItemButton, SIGNAL(clicked()), this, SLOT(on_DeleteMenuItemClicked()));

    editItemButton->connect(editItemButton, SIGNAL(clicked()), this, SLOT(on_EditMenuItemClicked()));

    //Adding all input widgets to our QHBoxLayout that groups them all together.
    layout->addWidget(nameLabel);
    layout->addWidget(menuItems);
    layout->addWidget(addNewItemButton);
    layout->addWidget(deleteItemButton);
    layout->addWidget(editItemButton);
}

/* FUNCTIONS CALLED ON BUTTON CLICK */

void MenuWidget::on_AddMenuItemClicked(){
    qInfo() << "Add item clicked.";

    if (this->restaurant.getMenu().size() >= 8){
        QMessageBox popup;
        popup.critical(0, "Error", "Maximum of 8 items per restaurant.");
    } else {
        this->parent->addItem(this->restaurant, this->restaurant.getMenu());
    }
}

void MenuWidget::on_DeleteMenuItemClicked(){
    qInfo() << "Delete item clicked.";

    //Index of menu ComboBox - necessary to know since we need to know which item they want.
    int index = this->menuItems->currentIndex();

    std::vector<MenuItem>updatedMenu = this->restaurant.getMenu();

    if (updatedMenu.size() > 0){
        updatedMenu.erase(updatedMenu.begin() + index);

        this->parent->deleteItem(this->restaurant, updatedMenu);
    } else {
        QMessageBox popup;
        popup.critical(0, "Error", "Menu is already empty.");
    }
}

void MenuWidget::on_EditMenuItemClicked(){
    qInfo() << "Edit item clicked.";

    int index = this->menuItems->currentIndex();
    MenuItem selected = this->restaurant.getMenu()[index];

    this->parent->editItem(this->restaurant, index);

    qInfo() << "Editing " << selected.name;
}
