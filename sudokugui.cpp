#include "sudokugui.h"
#include "ui_sudokugui.h"

using namespace cv;
using namespace std;

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
    displaySudokuBoard();

    modeButtons = new QButtonGroup(this);
    modeButtons = new QButtonGroup(this);
    modeButtons->addButton(ui->playRadioButton,0);
    modeButtons->addButton(ui->solveRadioButton,1);

    ui->solveButton->setEnabled(false);

    connect(modeButtons, SIGNAL(buttonClicked(int)), this, SLOT(handleModeSelect(int)));
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
        lineEditsArray[id]->setReadOnly(true);

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

        connect(lineEditsArray[id], SIGNAL(textEdited(QString)), this, SLOT(updateNumberSquare()));
    }
}
void SudokuGui::updateNumberSquare()
{
    QObject* obj = sender(); //gets a pointer to the object which sent the signal
    //For manual input do the following
    for(int i = 0; i < 81; i++)
    {
        if(obj == lineEditsArray[i])
        {
            if(board.isLegal(i,lineEditsArray[i]->text().toInt()))
            {
                board[i].setVal(lineEditsArray[i]->text().toInt());
                std::cout<<board<<std::endl;
            }
            else lineEditsArray[i]->setText(tr("")); //Users desired input breaks rules
        }
    }
    //for playing game do the following
}
void SudokuGui::displaySudokuBoard()
{
    for(int i = 0; i < 81; i++)
    {
        if(!(this->board[i].getVal() == 0))
            lineEditsArray[i]->setText(tr("%1").arg(this->board[i].getVal()));
        else
            lineEditsArray[i]->setText(tr(""));
    }
    return;
}

void SudokuGui::solveCurrentBoard()
{
    //TODO check case that unsolvable
    board.SolveSudoku();
    displaySudokuBoard();
}

void SudokuGui::handleModeSelect(int modeSelected)
{
    switch (modeSelected)
    {
    case 0:
        ui->playRadioButton->setEnabled(false);
        ui->solveRadioButton->setEnabled(true);

        //generateGameBoardButton = new QPushButton(this);
        easyGameButton = new QPushButton(this);
        mediumGameButton = new QPushButton(this);
        hardGameButton = new QPushButton(this);
        difficultyButtons = new QButtonGroup(this);
        checkButton = new QPushButton(this);
        hintButton = new QPushButton(this);
        timer = new QLabel(this);
        lives = new QLabel(this);
        timer->setText(tr("Timer: 0:00"));
        lives->setText(tr("Lives: 3"));
        timer->setAlignment(Qt::AlignCenter);
        lives->setAlignment(Qt::AlignCenter);

        difficultyButtons->addButton(easyGameButton, 0);
        difficultyButtons->addButton(mediumGameButton, 1);
        difficultyButtons->addButton(hardGameButton, 1);

        easyGameButton->setText(tr("Easy"));
        mediumGameButton->setText(tr("Medium"));
        hardGameButton->setText(tr("Hard"));
        checkButton->setText(tr("Check"));
        hintButton->setText(tr("Hint"));

        ui->horizontalLayout0->addWidget(easyGameButton);
        ui->horizontalLayout0->addWidget(mediumGameButton);
        ui->horizontalLayout0->addWidget(hardGameButton);

        ui->horizontalLayout1->addWidget(checkButton);
        ui->horizontalLayout1->addWidget(timer);
        ui->horizontalLayout1->addWidget(lives);
        ui->horizontalLayout1->addWidget(hintButton);

        //remove all widgets generated by the solve case
        if(previousModeFlag == 1)
        {
            ui->horizontalLayout0->removeWidget(manualInputButton);
            ui->horizontalLayout0->removeWidget(videoInputButton);
            ui->horizontalLayout0->removeWidget(pictureInputButton);
            delete manualInputButton;
            delete videoInputButton;
            delete pictureInputButton;
        }
        //Play
        //Setup lives
        //Setup timer
        //Setup Generator/Manual input
        //Setup hint functionality
        connect(difficultyButtons, SIGNAL(buttonClicked(int)), this, SLOT(handleGenerateGameBoard(int)));
        break;
    case 1:
        ui->solveRadioButton->setEnabled(false);
        ui->playRadioButton->setEnabled(true);

        manualInputButton = new QPushButton(this);
        videoInputButton = new QPushButton(this);
        pictureInputButton = new QPushButton(this);

        manualInputButton->setText(tr("Manual Input"));
        videoInputButton->setText(tr("Video Input"));
        pictureInputButton->setText(tr("Picture Input"));

        ui->horizontalLayout0->addWidget(manualInputButton);
        ui->horizontalLayout0->addWidget(videoInputButton);
        ui->horizontalLayout0->addWidget(pictureInputButton);

        //remove all widgets generated by the play case
        if(previousModeFlag == 0)
        {
            ui->horizontalLayout0->removeWidget(generateGameBoardButton);

            delete checkButton;
            delete hintButton;
            delete timer;
            delete lives;
            delete easyGameButton;
            delete mediumGameButton;
            delete hardGameButton;
        }

        //connect buttons to handler functions
        connect(manualInputButton, SIGNAL(released()), this, SLOT(handleManualInputSelected()));
        connect(videoInputButton, SIGNAL(released()), this, SLOT(handleVideoInputSelected()));
        connect(pictureInputButton, SIGNAL(released()), this, SLOT(handlePictureInputSelected()));
        break;
    }
    previousModeFlag = modeSelected;
}

void SudokuGui::handleManualInputSelected()
{
    manualInputButton->setFlat(true);
    videoInputButton->setFlat(false);
    pictureInputButton->setFlat(false);

    clearUiAndBoard();

    //Makes all line edits writable
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(false);

    ui->solveButton->setEnabled(true);
}

void SudokuGui::handleVideoInputSelected()
{
    manualInputButton->setFlat(false);
    videoInputButton->setFlat(true);
    pictureInputButton->setFlat(false);

    clearUiAndBoard();

    //Makes all line edits read only
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(true);
}

void SudokuGui::handlePictureInputSelected()
{
    manualInputButton->setFlat(false);
    videoInputButton->setFlat(false);
    pictureInputButton->setFlat(true);

    clearUiAndBoard();

    //Makes all line edits read only
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(true);

    // set the filter for STL file
    QString filter = "Images (*.png *.jpg)";
    // obtain the file name
    QString filename = QFileDialog::getOpenFileName(this, QString("Open STL file"), "./", filter);

    if (filename.isEmpty()) //if user exits file dialog rest of function is skipped
    {
        pictureInputButton->setFlat(false);
        return;
    }

    Mat originalImage = imread(filename.toStdString());

    // TODO Check for failure
    if (originalImage.empty())
    {

    }

    cvtColor(originalImage, originalImage, COLOR_RGB2GRAY);
    //imshow("Grey Image", originalImage);

    Mat blurredImage = originalImage.clone();
    GaussianBlur(originalImage, blurredImage, Size(11,11),0);
    //imshow("Blurred Grey Image", blurredImage);

    Mat mainOutline = Mat(originalImage.size(),  CV_8U);  //CV_8U = 8-bit unsigned int
    adaptiveThreshold(blurredImage, mainOutline, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);
    //imshow("Adaptive Thresholded", mainOutline);

    findMainOutline(mainOutline);

    vector<Vec2f> lines; //vector that will store lines in normal form (p, theta) as this is the output of the hough transform
    //performs the hough transform on the main outline https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
    //args 2 and 3 are the resolution params for p and theta. arg 4 is the threshold. I.e minimum number of intersections required to detect line
    HoughLines(mainOutline, lines, 1, CV_PI/180, 200);

    findAndMergeCloseLines(&lines, mainOutline);

    //Now find the 4 most extreme lines representing the outer 4 edges of the sudoku
    vector<Vec2f> edges;
    findEdges(lines, edges);

    Point2f intersectionPoints[4];
    getEdgeIntersectionPoints(edges, intersectionPoints, mainOutline);

    double imageSideLength = getImageSideLength(intersectionPoints);

    Point2f dst[4];
    dst[0] = Point(0,0);
    dst[1] = Point(imageSideLength-1, 0);
    dst[2] = Point(0, imageSideLength-1);
    dst[3] = Point(imageSideLength-1, imageSideLength-1);

    Mat transformedImage = Mat(Size(imageSideLength, imageSideLength), CV_8U);
    warpPerspective(originalImage, transformedImage, getPerspectiveTransform(intersectionPoints, dst), Size(imageSideLength, imageSideLength));
    
    //Next steps
    //Identify blank squares before sending to identifier
    //Preprocess number images by deskewing and centering them!!!
    //Change clasify function to work one image at a time perhaps
    //Compare the effectiveness of K-nearest neighbor (kNN) and support vector machines(SVM)
    //train algorithm using MNIST data set (big endian)
    //Neaten up code into functions and objects
   
    String windowName = "Transformed image";
    namedWindow(windowName);
    imshow(windowName, transformedImage); // Show our image inside the created window.
    waitKey(0);
    destroyWindow(windowName);

    KNN digitRecogniser;
    digitRecogniser.train("train-images.idx3-ubyte","train-labels.idx1-ubyte");
    //cout << "Test Result: " << digitRecogniser.test("t10k-images.idx3-ubyte","t10k-labels.idx1-ubyte") << endl;
    vector<int> results = digitRecogniser.identifyNumbers(transformedImage);
    board.setBoard(results);
    displaySudokuBoard();
    ui->solveButton->setEnabled(true);
}

void SudokuGui::handleGenerateGameBoard(int difficulty)
{
    clearUiAndBoard();
    

    vector<int> blankIDs;
    this->board.generateSolvableSudoku(blankIDs);

    displaySudokuBoard();
    ui->solveButton->setEnabled(true);

    //Makes only blanks writable
    for (int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(true);
    for (int i = 0; i < blankIDs.size(); i++)
        lineEditsArray[blankIDs[i]]->setReadOnly(false);

}
void SudokuGui::clearUiAndBoard()
{
    //Not using SudokuBoard's clearBoard function as would require two loops to clear both Ui and board data
    for(int i = 0; i < 81; i++)
    {
        this->board[i].setVal(0);
        lineEditsArray[i]->setText(tr(""));
    }
}
