/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_cardId;
    QLabel *label_id;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_carType;
    QLabel *label_type;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_carPlate;
    QSpacerItem *verticalSpacer;
    QLabel *label_screen;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 480);
        MainWindow->setMinimumSize(QSize(800, 480));
        MainWindow->setMaximumSize(QSize(800, 480));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_cardId = new QLabel(centralwidget);
        label_cardId->setObjectName(QStringLiteral("label_cardId"));
        label_cardId->setMinimumSize(QSize(300, 90));
        label_cardId->setMaximumSize(QSize(230, 90));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(36);
        label_cardId->setFont(font);
        label_cardId->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_cardId);

        label_id = new QLabel(centralwidget);
        label_id->setObjectName(QStringLiteral("label_id"));
        label_id->setMinimumSize(QSize(450, 90));
        label_id->setMaximumSize(QSize(450, 90));
        label_id->setFont(font);

        horizontalLayout_3->addWidget(label_id);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_carType = new QLabel(centralwidget);
        label_carType->setObjectName(QStringLiteral("label_carType"));
        label_carType->setMinimumSize(QSize(300, 90));
        label_carType->setMaximumSize(QSize(230, 90));
        label_carType->setFont(font);
        label_carType->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_carType);

        label_type = new QLabel(centralwidget);
        label_type->setObjectName(QStringLiteral("label_type"));
        label_type->setMinimumSize(QSize(450, 90));
        label_type->setMaximumSize(QSize(450, 90));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(24);
        label_type->setFont(font1);

        horizontalLayout_2->addWidget(label_type);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(25, 25, -1, -1);
        label_carPlate = new QLabel(centralwidget);
        label_carPlate->setObjectName(QStringLiteral("label_carPlate"));
        label_carPlate->setMinimumSize(QSize(400, 200));
        label_carPlate->setMaximumSize(QSize(400, 200));
        label_carPlate->setFont(font);
        label_carPlate->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_carPlate);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        label_screen = new QLabel(centralwidget);
        label_screen->setObjectName(QStringLiteral("label_screen"));
        label_screen->setMinimumSize(QSize(320, 240));
        label_screen->setMaximumSize(QSize(320, 240));
        label_screen->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(label_screen);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_cardId->setText(QApplication::translate("MainWindow", "\345\215\241\345\217\267:", 0));
        label_id->setText(QString());
        label_carType->setText(QApplication::translate("MainWindow", "\350\275\246\350\276\206\347\261\273\345\236\213:", 0));
        label_type->setText(QString());
        label_carPlate->setText(QString());
        label_screen->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
