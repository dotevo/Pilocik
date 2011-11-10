#ifndef KEYBOARDWINDOW_H
#define KEYBOARDWINDOW_H

#include "qfullscreenframe.h"
#include <QSignalMapper>
#include <QLineEdit>

namespace Ui {
    class KeyboardWindow;
}

/**
 * @brief Fullscreen widget with touch keyboard. Use TLineEdit to work.
 *
 */
class KeyboardWindow : public QFullScreenFrame
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     * @param text  Text edit widget.
     * @param view  Widget with data. Ex. QListWidget with data to show.
     */
    explicit KeyboardWindow(NavigationWindow *parent,QWidget *view=0);
    /**
     * @brief
     *
     */
    ~KeyboardWindow();
    static bool opened; /**< TODO */
    QLineEdit *textEdit;
private:
    Ui::KeyboardWindow *ui; /**< Ui object */
    QSignalMapper signalMapper; /**< TODO */

    void changeEvent(QEvent *);

private slots:
    /**
     * @brief Alfanumerical button has clicked.
     *
     * @param w
     */
    void buttonClicked(QWidget *w);
    /**
     * @brief Close button has clicked.
     *
     */
    void on_closeButton_clicked();
    /**
     * @brief
     *
     */
    void on_space_button_clicked();

    /**
     * @brief
     *
     */
    void on_back_button_clicked();

signals:
    /**
     * @brief Signal close.
     *
     * @param win
     */
    void close(KeyboardWindow*win);
};

#endif // KEYBOARDWINDOW_H
