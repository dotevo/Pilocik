#include "installframe.h"
#include "ui_installframe.h"

InstallFrame::InstallFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InstallFrame)
{
    ui->setupUi(this);
}

InstallFrame::~InstallFrame()
{
    delete ui;
}
