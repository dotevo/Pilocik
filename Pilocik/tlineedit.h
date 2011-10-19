#ifndef TLINEEDIT_H
#define TLINEEDIT_H

#include <QLineEdit>
#include <QFocusEvent>

class KeyboardWindow;

/**
 * @brief Class with support KeyboardWindow.
 *
 * @class TLineEdit tlineedit.h "Pilocik/tlineedit.h"
 */
class TLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @fn TLineEdit
     * @param parent
     */
    explicit TLineEdit(QWidget *parent = 0);
    /**
     * @brief Function works like "clicked"
     *
     * @fn focusInEvent
     * @param e
     */
    void focusInEvent ( QFocusEvent * e );

    /**
     * @brief Set it up to show widget on KeyboardWindow
     *
     * @fn setVisibleWidget
     * @param widget
     */
    void setVisibleWidget(QWidget*widget);
private:
    QWidget *visWidget; /**< TODO */
public slots:
    void keyboardClosed(KeyboardWindow*win);

};

#endif // TLINEEDIT_H
