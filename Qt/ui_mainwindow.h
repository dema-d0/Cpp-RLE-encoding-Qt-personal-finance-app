/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topControlsLayout;
    QLabel *labelFrom;
    QDateEdit *startDateEdit;
    QLabel *labelTo;
    QDateEdit *endDateEdit;
    QPushButton *filterButton;
    QPushButton *loadCsvButton;
    QPushButton *saveCsvButton;
    QTableView *tableView;
    QHBoxLayout *crudButtonsLayout;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topControlsLayout = new QHBoxLayout();
        topControlsLayout->setObjectName("topControlsLayout");
        labelFrom = new QLabel(centralwidget);
        labelFrom->setObjectName("labelFrom");

        topControlsLayout->addWidget(labelFrom);

        startDateEdit = new QDateEdit(centralwidget);
        startDateEdit->setObjectName("startDateEdit");

        topControlsLayout->addWidget(startDateEdit);

        labelTo = new QLabel(centralwidget);
        labelTo->setObjectName("labelTo");

        topControlsLayout->addWidget(labelTo);

        endDateEdit = new QDateEdit(centralwidget);
        endDateEdit->setObjectName("endDateEdit");

        topControlsLayout->addWidget(endDateEdit);

        filterButton = new QPushButton(centralwidget);
        filterButton->setObjectName("filterButton");

        topControlsLayout->addWidget(filterButton);

        loadCsvButton = new QPushButton(centralwidget);
        loadCsvButton->setObjectName("loadCsvButton");

        topControlsLayout->addWidget(loadCsvButton);

        saveCsvButton = new QPushButton(centralwidget);
        saveCsvButton->setObjectName("saveCsvButton");

        topControlsLayout->addWidget(saveCsvButton);


        verticalLayout->addLayout(topControlsLayout);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");

        verticalLayout->addWidget(tableView);

        crudButtonsLayout = new QHBoxLayout();
        crudButtonsLayout->setObjectName("crudButtonsLayout");
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");

        crudButtonsLayout->addWidget(addButton);

        editButton = new QPushButton(centralwidget);
        editButton->setObjectName("editButton");

        crudButtonsLayout->addWidget(editButton);

        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName("deleteButton");

        crudButtonsLayout->addWidget(deleteButton);


        verticalLayout->addLayout(crudButtonsLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestione Transazioni", nullptr));
        labelFrom->setText(QCoreApplication::translate("MainWindow", "Da:", nullptr));
        labelTo->setText(QCoreApplication::translate("MainWindow", "A:", nullptr));
        filterButton->setText(QCoreApplication::translate("MainWindow", "Filtra", nullptr));
        loadCsvButton->setText(QCoreApplication::translate("MainWindow", "Carica CSV", nullptr));
        saveCsvButton->setText(QCoreApplication::translate("MainWindow", "Salva CSV", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Aggiungi", nullptr));
        editButton->setText(QCoreApplication::translate("MainWindow", "Modifica", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Elimina", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
