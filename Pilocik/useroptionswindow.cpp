#include "useroptionswindow.h"
#include "ui_useroptionswindow.h"
#include "navigationwindow.h"
#include "settings.h"
#include <QDebug>

UserOptionsWindow::UserOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::UserOptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);

    QStringList languages;

    languages.append("System");
    languages.append("English");
    languages.append("Polish");

    QString langLocale = Settings::getInstance()->getLanguage();

    int index = 0;
    if (!langLocale.isEmpty()) {
        for (int i = 0; i < languages.length(); i++)
        {
            if (langLocale.compare(Settings::getInstance()->getLocale(languages.at(i)), Qt::CaseInsensitive) == 0) {
                index = i;
                break;
            }
        }
    }

    ui->languageComboBox->addItems(languages);
    ui->languageComboBox->setCurrentIndex(index);
    //ui->languageComboBox->insertSeparator(1);
}

UserOptionsWindow::~UserOptionsWindow(){
    delete ui;
}

void UserOptionsWindow::on_okButton_clicked(){
    setVisible(false);
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
