#ifndef TWIDGETMANAGER_H
#define TWIDGETMANAGER_H

#include <QMap>

#include "tmovableframe.h"


/**
 * @brief Singleton. This class is layer to show widgets and is needed to manage them.
 *
 * @class TWidgetManager twidgetmanager.h "Pilocik/twidgetmanager.h"
 */
class TWidgetManager
{
private:
/**
 * @brief
 *
 * @fn TWidgetManager
 */
    TWidgetManager();
    QMap <QString, TMovableFrame*> widgets; /**< TODO */
    QWidget *parent; /**< TODO */
    static TWidgetManager *instance; /**< TODO */
    bool *ROUTING;    // is routing mode
public:
    /**
     * @brief
     *
     * @fn ~TWidgetManager
     */
    ~TWidgetManager();
    /**
     * @brief
     *
     * @fn getInstance
     */
    static TWidgetManager* getInstance();
    /**
     * @brief
     *
     * @fn TWidgetManager
     * @param
     */
    TWidgetManager(TWidgetManager const&);              // Don't Implement
    /**
     * @brief
     *
     * @fn operator =
     * @param
     */
    void operator=(TWidgetManager const&);          // Don't implement

    /**
     * @brief
     *
     * @fn setWidgetVisible
     * @param name
     * @param visible
     */
    void setWidgetVisible(QString name,bool visible);
    /**
     * @brief Get widget with name from manager.
     *
     * @fn getWidget
     * @param name specyfic name
     */
    TMovableFrame* getWidget(QString name);
    /**
     * @brief Get widgets list.
     *
     * @fn getWidgetList
     */
    QList<TMovableFrame*> getWidgetList();
    /**
     * @brief Get visible widgets.
     *
     * @fn getWidgetVisibleList
     */
    QList<TMovableFrame*> getWidgetVisibleList();
    /**
     * @brief Add widget to manager and screen.
     *
     * @fn addWidget
     * @param name Widget name
     * @param w     Widget
     */
    void addWidget(QString name, TMovableFrame* w);
    /**
     * @brief Set mode to the opposite one. (from Moving to Stand or from Stand to Moving)
     *
     * @fn changeMode
     */
    void changeMode();
    /**
     * @brief Set mode. (Moving,Standing)
     *
     * @fn setMode
     * @param mode
     */
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    /**
     * @brief Get mode. (Moving,Standing)
     *
     * @fn getMode
     * @return mode
     */
    TMovableFrame::TMOVABLEMODE getMode();
    /**
     * @brief
     *
     * @fn setParent
     * @param w
     */
    void setParent(QWidget *w);
    /**
     * @brief Sets all widgets in widget manager not to be visible.
     *
     */
    void hideAllWidgets();
    /**
     * @brief Sets all widgets in widget manager to be visible.
     *
     */
    void showAllWidgets();

    /**
      @brief Sets routing mode
      */
    void setRouting(bool mode = true);

};

#endif // TWIDGETMANAGER_H
