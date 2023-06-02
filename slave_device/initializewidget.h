#ifndef INITIALIZEWIDGET_H
#define INITIALIZEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InitializeWidget; }
QT_END_NAMESPACE

class InitializeWidget : public QWidget
{
    Q_OBJECT

public:
    InitializeWidget(QWidget *parent = nullptr);
    ~InitializeWidget();

public slots:
    void on_jumpToMainWindow();
private:
    Ui::InitializeWidget *ui;
    MainWindow* m_widget;

};
#endif // INITIALIZEWIDGET_H
