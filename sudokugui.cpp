#include "sudokugui.h"
#include "ui_sudokugui.h"
SudokuGui::SudokuGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SudokuGui)
{
    ui->setupUi(this);
    generateLabels();
    //Static array holding input sudoku puzzle to be solved. 0s represent free spaces
    int inputSudoku[81]      =   {3,0,6, 5,0,8, 4,0,0,
                                  5,2,0, 0,0,0, 0,0,0,
                                  0,8,7, 0,0,0, 0,3,1,

                                  0,0,3, 0,1,0, 0,8,0,
                                  9,0,0, 8,6,3, 0,0,5,
                                  0,5,0, 0,9,0, 6,0,0,

                                  1,3,0, 0,0,0, 2,5,0,
                                  0,0,0, 0,0,0, 0,7,4,
                                  0,0,5, 2,0,6, 3,0,0};
    //board = SudokuBoard(inputSudoku);
    board = SudokuBoard();
    displaySudokuBoard(board);

    connect(ui->solveButton, SIGNAL(released()), this, SLOT(solveCurrentBoard()));
}

SudokuGui::~SudokuGui()
{
    delete ui;
}

void SudokuGui::generateLabels()
{
    QGridLayout *layout = ui->gridLayout0;
    QGridLayout *grid1 = ui->gridLayout1;
    QGridLayout *grid2 = ui->gridLayout2;
    QGridLayout *grid3 = ui->gridLayout3;
    QGridLayout *grid4 = ui->gridLayout4;
    QGridLayout *grid5 = ui->gridLayout5;
    QGridLayout *grid6 = ui->gridLayout6;
    QGridLayout *grid7 = ui->gridLayout7;
    QGridLayout *grid8 = ui->gridLayout8;
    QGridLayout *grid9 = ui->gridLayout9;
    lineEditsArray = new QLineEdit*[81];
    for(int id=0; id<81; id++)
    {
        lineEditsArray[id] = new QLineEdit(this);
        QSizePolicy lineEditSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        lineEditsArray[id]->setSizePolicy(lineEditSizePolicy);
        QFont font( "Arial", 18, QFont::Bold);
        lineEditsArray[id]->setText(tr(""));
        lineEditsArray[id]->setFont(font);
        lineEditsArray[id]->setStyleSheet("QLineEdit {"
        "border-style: solid;"
        "border-width: 1px;"
        "border-color: black; "
        "}");
        lineEditsArray[id]->setAlignment(Qt::AlignCenter);
        lineEditsArray[id]->setMaxLength(1);
        //layout->addWidget(lineEditsArray[id], int(id/9), id % 9);
        if(id % 9 < 3 && id/9 <3)
            grid1->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 3)
            grid2->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 3)
            grid3->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 6)
            grid4->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 6)
            grid5->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 6)
            grid6->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 9)
            grid7->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 9)
            grid8->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 9)
            grid9->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        //Possibly want to change this to the textEdited signal - its possible user could change value and forget to press return
        connect(lineEditsArray[id], SIGNAL(returnPressed()), this, SLOT(updateNumberSquare()));
    }
}
void SudokuGui::updateNumberSquare()
{
    QObject* obj = sender(); //gets a pointer to the object which sent the signal
    for(int i = 0; i < 81; i++)
    {
        if(obj == lineEditsArray[i])
        {
            board[i].setVal(lineEditsArray[i]->text().toInt());
            std::cout<<board<<std::endl;
        }
    }
}
void SudokuGui::displaySudokuBoard(SudokuBoard &s)
{
    for(int i = 0; i < 81; i++)
    {
        if(!(s[i].getVal() == 0))
            lineEditsArray[i]->setText(tr("%1").arg(s[i].getVal()));
    }
    return;
}

void SudokuGui::solveCurrentBoard()
{
    //TODO check case that unsolvable
    board.SolveSudoku();
    displaySudokuBoard(board);
}
