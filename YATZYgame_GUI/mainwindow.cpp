#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initially Roll and Change Turn are disabled.
    // Reset Button will start the game
    ui->rollButton->setDisabled(true);
    ui->cTurnButton->setDisabled(true);

    // Initialize the gameTimer
    gameTimer = new QTimer(this);

    // Save the original style sheet
    originalStyleSheet = this->styleSheet();

    // Connect the timer's timeout signal to the slot updateTimeElapsed
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateTimeElapsed);

    //connect Roll/Reset/ChangeTurn buttons
    connect(ui->cTurnButton, &QPushButton::clicked, this,
            &MainWindow::cTurnButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this,
            &MainWindow::resetButtonClicked);
    connect(ui->rollButton, &QPushButton::clicked, this,
            &MainWindow::rollButtonClicked);

    // connect lock dice buttons
    connect(ui->diceButton0, &QPushButton::clicked, this,
            &MainWindow::diceButtonClicked);
    connect(ui->diceButton1, &QPushButton::clicked, this,
            &MainWindow::diceButtonClicked);
    connect(ui->diceButton2, &QPushButton::clicked, this,
            &MainWindow::diceButtonClicked);
    connect(ui->diceButton3, &QPushButton::clicked, this,
            &MainWindow::diceButtonClicked);
    connect(ui->diceButton4, &QPushButton::clicked, this,
            &MainWindow::diceButtonClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::cTurnButtonClicked()
{
    ui->textBrowserStatus->setText("");

    // if there isn't any other active player to change turn
    if (!engine.give_turn()){
        ui->textBrowserStatus->setText("No more player left to change turn!");
    }else{
        engine.give_turn();
        updateLabels();

        // empty the dices
        QString imagePath =  ":/Resources/";
        ui->diceButton0->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
        ui->diceButton1->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
        ui->diceButton2->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
        ui->diceButton3->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
        ui->diceButton4->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
    }
}

void MainWindow::setPlayers()
{
    for(unsigned int i = 0; i < numOfPlayers_; ++i)
    {
        Player player = {i + 1, INITIAL_NUMBER_OF_ROLLS, {}, {},
                         {false, false, false, false, false}};
        engine.add_player(player);
    }
}

void MainWindow::updateLabels()
{
    QString player = QString::number(engine.getId());
    QString rolls = QString::number(engine.getRollsLeft());
    ui->labelPlayer->setText(player);
    ui->labelTries->setText(rolls);
}

void MainWindow::updateDices()
{

    std::vector<int> dices = engine.getLatestpoints();

    // Assuming the PNG images are in the same directory as your executable.
    QString imagePath =  ":/Resources/"; 

    QSize iconSize(40, 40); // Set the desired size for the icons

    ui->diceButton0->setIcon(QIcon(QPixmap(imagePath + QString::number
                                           (dices.at(0)) + ".png")));
    ui->diceButton1->setIcon(QIcon(QPixmap(imagePath + QString::number
                                           (dices.at(1)) + ".png")));
    ui->diceButton2->setIcon(QIcon(QPixmap(imagePath + QString::number
                                           (dices.at(2)) + ".png")));
    ui->diceButton3->setIcon(QIcon(QPixmap(imagePath + QString::number
                                           (dices.at(3)) + ".png")));
    ui->diceButton4->setIcon(QIcon(QPixmap(imagePath + QString::number
                                           (dices.at(4)) + ".png")));

    // Set the icon size for the buttons
    ui->diceButton0->setIconSize(iconSize);
    ui->diceButton1->setIconSize(iconSize);
    ui->diceButton2->setIconSize(iconSize);
    ui->diceButton3->setIconSize(iconSize);
    ui->diceButton4->setIconSize(iconSize);
}

void MainWindow::diceButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        // the object names are diceButton0, diceButton1, ..., diceButton4
        QString buttonName = clickedButton->objectName();

        // Extract the last character as the dice index
        int diceIndex = buttonName.right(1).toInt();

        engine.toggleLock(diceIndex);
        updateDices(); // Update the UI
    }
}


void MainWindow::resetButtonClicked()
{
    engine.gameReset();
    setPlayers();
    updateLabels();

    // Start/reset the timer when the game is reset
    elapsedSeconds = 0; // Reset the elapsed time
    gameTimer->start(1000); // Start the timer with a 1-second interval

    // Restore the original background color
    this->setStyleSheet(originalStyleSheet);

    // enable Roll & Change Turn buttons when the game is reset
    ui->rollButton->setDisabled(false);
    ui->cTurnButton->setDisabled(false);

    // empty the dices for the new game round
    QString imagePath =  ":/Resources/";
    ui->diceButton0->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
    ui->diceButton1->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
    ui->diceButton2->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
    ui->diceButton3->setIcon(QIcon(QPixmap(imagePath + "empty.png")));
    ui->diceButton4->setIcon(QIcon(QPixmap(imagePath + "empty.png")));

    ui->textBrowserStatus->setText("");  //empty the status textBrowser
}


void MainWindow::rollButtonClicked()
{
    if (engine.getRollsLeft() <= 0){
        ui->textBrowserStatus->setText("No more rolls left!\n");
    }

    engine.roll();
    updateLabels();
    updateDices();
    if (engine.is_game_over()){
        gameTimer->stop();

        // Change the background color to green when the game is over
        this->setStyleSheet("background-color: green;");

        string winner_text = engine.report_winner();
        string time = "\n\nTime spent for this round in seconds: ";
        ui->textBrowserStatus->setText(QString::fromStdString(winner_text+time)
                                       +QString::number(elapsedSeconds));
        // disable Roll & Change Turn buttons when the game is over
        ui->rollButton->setDisabled(true);
        ui->cTurnButton->setDisabled(true);
    }
}

void MainWindow::updateTimeElapsed()
{
    elapsedSeconds++;
}


void MainWindow::userInputReceived()
{
    numOfPlayers_ = ui->lineEditUserInput->text().toUInt();
}

