#ifndef GENERIC6DOFSPRINGWIDGET_H
#define GENERIC6DOFSPRINGWIDGET_H

#include <QWidget>

namespace Ui {
class Generic6DOFSpringWidget;
}

class Generic6DOFSpringWidget : public QWidget
{
    Q_OBJECT

public:
    Generic6DOFSpringWidget(QWidget *parent = 0);
    ~Generic6DOFSpringWidget();

private:
    Ui::Generic6DOFSpringWidget *ui;
};

#endif // GENERIC6DOFSPRINGWIDGET_H
