#ifndef LOADWIDGET_H
#define LOADWIDGET_H

#include <QWidget>
#include <QTimer>
#include "mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class LoadWidget; }
QT_END_NAMESPACE

class LoadWidget : public QWidget
{
    Q_OBJECT

public:
    LoadWidget(QWidget *parent = nullptr);
    ~LoadWidget();

public slots:
    void on_jumpToMainWindow();

private:
    Ui::LoadWidget *ui;
    MainWindow* m_widget;
};
#endif // LOADWIDGET_H
