/*WINDOW*/
QWidget > QTabWidget{
    background: #ff0000;
}

QGraphicsView{
    background: black;
    border-radius: 0px;
    border: 0px solid rgb(0, 255, 0);
}

QLabel {
    color: rgb(0, 255, 0);
    border: 0;
    background: black;
    font-weight: bold;
    font-family: "Verdana";
}

QPushButton{
    background: black;
    border-radius: 0px;
    color: rgb(0, 255, 0);
    border: 1px solid rgb(0, 255, 0);
    width: 90px
}

QPushButton:hover{
    background: black;
    color: rgb(0, 255, 0);
}

QPushButton:pressed{
    background: black;
    color: rgb(0, 255, 0);
}

QPushButton:disabled{
    background: black;
    border: 1px solid rgb(0, 155, 0);
    color: rgb(155, 0, 0);
}

#menuPanel{
    background:black;
    border-bottom-left-radius: 0px;
    border-bottom-right-radius: 0px;
}

#routeFrame{
    background: rgb(0, 255, 0);
    border-radius: 0px;
}

QLineEdit{
    background: black;
    border-radius: 0px;
    border: 1px solid rgb(0, 255, 0);
}

QListView{
    background: black;
    border-radius: 0px;
    border: 1px solid rgb(180, 180, 180 );
}

/*RADIOBUTTON*/
QRadioButton{
    color: rgb(0, 255, 0);
    font: bold;
    font-size: 12px;
}

QRadioButton::indicator{
    width: 20px;
    height: 20px;
}

QRadioButton::indicator::checked {
    background: rgb(0, 255, 0);
    border-radius: 11px;
    border: 1px solid rgb(0, 255, 0);
}

QRadioButton::indicator::unchecked {
    background: qradialgradient(cx:0, cy:0, radius: 1,
                            fx:0.5, fy:0.5,
                            stop:0 #f9f9f9,
                            stop:1 #a3a3a3);
    border-radius: 11px;
    border: 1px solid #b8b8b8;
}
/*=======================================*/

/*CHECKBOX*/
QCheckBox{
    color: rgb(0, 255, 0);
    font: bold ;
    font-size: 12px;
}

QCheckBox::indicator{
    width: 20px;
    height: 20px;
}
QCheckBox::indicator::checked {
    background: qradialgradient(cx:0, cy:0, radius: 1,
                            fx:0.5, fy:0.5,
                            stop:0 #406fb5,
                            stop:1 #3b4576);
    border-radius: 4px;
    border: 1px solid #4260ab;
}

QCheckBox::indicator::unchecked {
    background: black;
    border-radius: 4px;
    border: 1px solid rgb(0, 255, 0);
}
/*=======================================*/

QComboBox{
    background: black;
    border-radius: 8px;
    border: 1px solid rgb(0, 255, 0);
    font: bold ;
    font-size: 12px;
}

QComboBox:editable {
    background: black;
}

QComboBox:!editable, QComboBox::drop-down:editable {
    background: black;
}

/* QComboBox gets the "on" state when the popup is open */
QComboBox:!editable:on, QComboBox::drop-down:editable:on {
    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                             stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,
                             stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);
}

QComboBox:on { /* shift the text when the popup opens */
     padding-top: 3px;
     padding-left: 4px;
}

QComboBox::drop-down {
     subcontrol-origin: padding;
     subcontrol-position: top right;
     width: 15px;

     border-left-width: 1px;
     border-left-color: darkgray;
     border-left-style: solid; /* just a single line */
     border-top-right-radius: 3px; /* same radius as the QComboBox */
     border-bottom-right-radius: 3px;
}

QComboBox QAbstractItemView {
    background: black;
    border-radius: 0px;
}


/*SPECYFIC!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*RouteWindow*/
#toButton,#fromButton,#nearestButton{
    background: black;
    border-radius: 8px;
    border: 1px solid rgb(0, 255, 0);
    color:#000;
}

#toButton:pressed,#fromButton:pressed,#nearestButton:pressed,#nearestClrButton:pressed{
    background: black;
}
QFullScreenFrame{
    background: black;
    border-radius: 0px;
}

/*SLIDER*/
QSlider::groove:vertical {
    width: 4px;
    margin: 6px;
    background: black;
    border-radius: 2px;
    border: 1px solid rgb(0, 255, 0);
}

QSlider::handle:vertical {
    background: black;
    border-radius: 4px;
    border: 1px solid rgb(0, 255, 0);
    color:#000;
    height:8px;
    margin: 0 -6px; /* expand outside the groove */
}
/*=============================================================*/

/*TABWIDGET*/
#tabWidget {
    background: black;
}
QTabWidget {
    background: black;
}
QTabWidget::pane {
    background: black;
}
QTabWidget::tab-bar {
    background: black;
    left: 5px;
}
QTabBar::tab {
    background: black;
    min-width: 150px;
    padding: 4px;
    color: rgb(0, 255, 0);
    font-weight: bold;
    height: 30px;
}
QTabBar::tab:selected {
    font-size: 24px;
}
