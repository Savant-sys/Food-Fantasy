#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <vector>

#include "mainwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "restaurant.h"

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(Restaurant restaurant, MainWindow *parent = nullptr);

private:
    QHBoxLayout *layout;
    QLabel *nameLabel;
    QComboBox *menuItems;
    QPushButton *addNewItemButton;
    QPushButton *deleteItemButton;
    QPushButton *editItemButton;
    MainWindow *parent;

    Restaurant restaurant;

private slots:
    void on_AddMenuItemClicked();

    void on_DeleteMenuItemClicked();

    void on_EditMenuItemClicked();

signals:

};

#endif // MENUWIDGET_H
