#ifndef TLINEEDIT_H
#define TLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class TLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit TLineEdit(QWidget *parent = 0);
    void focusInEvent ( QFocusEvent * e );

    void setVisibleWidget(QWidget*widget);
private:
    QWidget *visWidget;

};

#endif // TLINEEDIT_H
