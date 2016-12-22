#include "include/UserInterface/generic6dofspringwidget.h"
#include "ui_generic6DofSpringWidget.h"

Generic6DOFSpringWidget::Generic6DOFSpringWidget(QWidget *parent) :
    QWidget(parent),
  ui(new Ui::Generic6DOFSpringWidget())
{

    ui->setupUi(this);

}

Generic6DOFSpringWidget::~Generic6DOFSpringWidget()
{

}
