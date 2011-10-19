#include "keyboardwindow.h"
#include "ui_keyboardwindow.h"
#include <QDebug>

bool KeyboardWindow::opened=0;

KeyboardWindow::KeyboardWindow(NavigationWindow *parent,QWidget *view) :
    QFullScreenFrame(parent),
    ui(new Ui::KeyboardWindow)
{
    ui->setupUi(this);
    textEdit=ui->textLineEdit;
    //Number buttons
    signalMapper.setMapping(ui->b0_button,ui->b0_button);
    signalMapper.setMapping(ui->b1_button,ui->b1_button);
    signalMapper.setMapping(ui->b2_button,ui->b2_button);
    signalMapper.setMapping(ui->b3_button,ui->b3_button);
    signalMapper.setMapping(ui->b4_button,ui->b4_button);
    signalMapper.setMapping(ui->b5_button,ui->b5_button);
    signalMapper.setMapping(ui->b6_button,ui->b6_button);
    signalMapper.setMapping(ui->b7_button,ui->b7_button);
    signalMapper.setMapping(ui->b8_button,ui->b8_button);
    signalMapper.setMapping(ui->b9_button,ui->b9_button);
    connect(ui->b0_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b1_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b2_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b3_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b4_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b5_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b6_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b7_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b8_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(ui->b9_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));


    //Letters button
    signalMapper.setMapping(ui->bq_button,ui->bq_button);
    signalMapper.setMapping(ui->bw_button,ui->bw_button);
    signalMapper.setMapping(ui->be_button,ui->be_button);
    signalMapper.setMapping(ui->br_button,ui->br_button);
    signalMapper.setMapping(ui->bt_button,ui->bt_button);
    signalMapper.setMapping(ui->by_button,ui->by_button);
    signalMapper.setMapping(ui->bu_button,ui->bu_button);
    signalMapper.setMapping(ui->bi_button,ui->bi_button);
    signalMapper.setMapping(ui->bo_button,ui->bo_button);
    signalMapper.setMapping(ui->bp_button,ui->bp_button);
    signalMapper.setMapping(ui->ba_button,ui->ba_button);
    signalMapper.setMapping(ui->bs_button,ui->bs_button);
    signalMapper.setMapping(ui->bd_button,ui->bd_button);
    signalMapper.setMapping(ui->bf_button,ui->bf_button);
    signalMapper.setMapping(ui->bg_button,ui->bg_button);
    signalMapper.setMapping(ui->bh_button,ui->bh_button);
    signalMapper.setMapping(ui->bj_button,ui->bj_button);
    signalMapper.setMapping(ui->bk_button,ui->bk_button);
    signalMapper.setMapping(ui->bl_button,ui->bl_button);
    signalMapper.setMapping(ui->bz_button,ui->bz_button);
    signalMapper.setMapping(ui->bx_button,ui->bx_button);
    signalMapper.setMapping(ui->bc_button,ui->bc_button);
    signalMapper.setMapping(ui->bv_button,ui->bv_button);
    signalMapper.setMapping(ui->bb_button,ui->bb_button);
    signalMapper.setMapping(ui->bn_button,ui->bn_button);
    signalMapper.setMapping(ui->bm_button,ui->bm_button);
    signalMapper.setMapping(ui->dot_button,ui->dot_button);
    signalMapper.setMapping(ui->comma_button,ui->comma_button);
    signalMapper.setMapping(ui->slash_button,ui->slash_button);

    connect(ui->bq_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bw_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->be_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->br_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bt_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->by_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bu_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bi_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bo_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bp_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->ba_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bs_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bd_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bf_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bg_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bh_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bj_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bk_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bl_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bz_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bx_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bc_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bv_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bb_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bn_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->bm_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->dot_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->comma_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));
    connect(ui->slash_button, SIGNAL(clicked()),&signalMapper, SLOT(map()));


    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));
    opened=true;
}

KeyboardWindow::~KeyboardWindow(){
    delete ui;
    opened=false;
}



void KeyboardWindow::buttonClicked(QWidget *w){
    int curs=ui->textLineEdit->cursorPosition();
    QString s=ui->textLineEdit->text();
    ui->textLineEdit->setText(s.left(curs)+((QPushButton*)w)->text()+s.right(s.length()-curs));
    ui->textLineEdit->setCursorPosition(curs+1);
    ui->textLineEdit->activateWindow();
}

void KeyboardWindow::on_closeButton_clicked(){
    emit close(this);
}

void KeyboardWindow::on_space_button_clicked(){
    int curs=ui->textLineEdit->cursorPosition();
    QString s=ui->textLineEdit->text();
    ui->textLineEdit->setText(s.left(curs)+" "+s.right(s.length()-curs));
    ui->textLineEdit->setCursorPosition(curs+1);
    ui->textLineEdit->activateWindow();
}

void KeyboardWindow::on_back_button_clicked()
{
    int curs=ui->textLineEdit->cursorPosition();
    QString s=ui->textLineEdit->text();
    if(curs>=1){
        ui->textLineEdit->setText(s.left(curs-1)+s.right(s.length()-curs));
        ui->textLineEdit->setCursorPosition(curs-1);
    }
}
