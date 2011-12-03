#include "useroptionswindow.h"
#include "ui_useroptionswindow.h"
#include "navigationwindow.h"
#include "settings.h"
#include <QDebug>
#include <QDir>

UserOptionsWindow::UserOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::UserOptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);

    QDir dir(QDir::currentPath() + "/lang");
    qDebug() << dir.path();
    QStringList filter;
    filter << "*.qm";
    dir.setNameFilters(filter);
    QFileInfoList list = dir.entryInfoList();

    ui->languageComboBox->addItem(tr("System"));
    for (int i = 0; i < list.size(); ++i) {
        QString a=list.at(i).fileName().split(".").at(0);
        ui->languageComboBox->addItem(Settings::getInstance()->getLanguageFromLocale(a));
    }

    QString lang = Settings::getInstance()->getStartLanguage();

    if(lang.isNull())
        ui->languageComboBox->setCurrentIndex(0);
    //Selected language
    else
        ui->languageComboBox->setCurrentIndex(ui->languageComboBox->findText(lang));

    ui->guiStylePath->setText(Settings::getInstance()->getLayoutStylePath());
}

UserOptionsWindow::~UserOptionsWindow(){
    delete ui;
}

void UserOptionsWindow::on_okButton_clicked(){
    setVisible(false);
    Settings::getInstance()->modifyCoreSettings("layoutStylePath", ui->guiStylePath->text());

    QFile layoutStyleFile(Settings::getInstance()->getLayoutStylePath());
    if (!layoutStyleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QString layoutStyle = layoutStyleFile.readAll();
    NavigationWindow::main->setStyleSheet(layoutStyle);
    NavigationWindow::main->style()->unpolish(NavigationWindow::main);
    NavigationWindow::main->style()->polish(NavigationWindow::main);
    layoutStyleFile.close();

    emit closed();
}

void UserOptionsWindow::on_languageComboBox_currentIndexChanged(const QString &lang)
{
    if (Settings::getInstance()->getLanguage().compare(lang, Qt::CaseInsensitive) != 0) {
        Settings::getInstance()->setLanguage(lang);
        Settings::getInstance()->reloadTranslation();
    }

    //this->ui->retranslateUi(this);
}

void UserOptionsWindow::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
