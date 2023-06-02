#include "initializewidget.h"
#include "ui_initializewidget.h"

InitializeWidget::InitializeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InitializeWidget)
{
    ui->setupUi(this);
    QTimer::singleShot(100, this, &InitializeWidget::on_jumpToMainWindow);
}

InitializeWidget::~InitializeWidget()
{
    delete ui;
}

void InitializeWidget::on_jumpToMainWindow()
{
    m_widget = new MainWindow(this);
    m_widget->show();
    this->hide();
}

