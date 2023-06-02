#include "loadwidget.h"
#include "ui_loadwidget.h"

LoadWidget::LoadWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoadWidget)
{
    ui->setupUi(this);
    QTimer::singleShot(1000, this, &LoadWidget::on_jumpToMainWindow);
}

LoadWidget::~LoadWidget()
{
    delete ui;
}

void LoadWidget::on_jumpToMainWindow()
{
    m_widget = new MainWindow;
    m_widget->show();
    this->hide();
}

