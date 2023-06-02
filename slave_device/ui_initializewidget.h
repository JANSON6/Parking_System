/********************************************************************************
** Form generated from reading UI file 'initializewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INITIALIZEWIDGET_H
#define UI_INITIALIZEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InitializeWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *InitializeWidget)
    {
        if (InitializeWidget->objectName().isEmpty())
            InitializeWidget->setObjectName(QStringLiteral("InitializeWidget"));
        InitializeWidget->resize(800, 480);
        InitializeWidget->setMinimumSize(QSize(800, 480));
        InitializeWidget->setMaximumSize(QSize(800, 480));
        gridLayout = new QGridLayout(InitializeWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 72, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(182, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        label = new QLabel(InitializeWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(400, 300));
        label->setMaximumSize(QSize(400, 300));
        label->setFrameShape(QFrame::Box);

        gridLayout->addWidget(label, 1, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(182, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 72, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 2, 1, 1);


        retranslateUi(InitializeWidget);

        QMetaObject::connectSlotsByName(InitializeWidget);
    } // setupUi

    void retranslateUi(QWidget *InitializeWidget)
    {
        InitializeWidget->setWindowTitle(QApplication::translate("InitializeWidget", "InitializeWidget", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InitializeWidget: public Ui_InitializeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITIALIZEWIDGET_H
