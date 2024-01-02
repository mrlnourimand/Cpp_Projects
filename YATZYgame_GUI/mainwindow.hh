/* Yatzy_gui
 *
 * Desc:
 * The project aims at implementation a graphical user interface for the
 * given yatzy game. Yatzy game has five dices that are thrown by the players
 * in desired order. At start, each player has three turns to roll up dices,
 * but it is not mandatory to use them all. Moreover, it is not required to use
 * the turns alternately, but a player can, for example, take all their turns
 * one after the other (without giving turn to other players between one’s
 * rolls).
 *
 * A player can lock part of the dices such that only unlocked dices will be
 * rolled in the next roll. After all turns of all players have been used or
 * after dropping out the game, the program tells who was the winner or between
 * which players the result is tie. Modules functions and gameengine were
 * already implemented, only a things were modified (listed in the following).
 * Here, only the grapgical user interface is added to the game.

 * Program author
 * Name: Maral Nourimand
 * Student number: 151749113
 * UserID: txmano
 * E-Mail: maral.nourimand@tuni.fi
 *
 * Notes about the program and it's implementation (if any):
 * I have modified a few changes in the gameengin module:
 * 1. a vector of bool added to Player struct to store the locked status of
 *  each dice
 * 2. Method give_turn() is not void anymore, instead it returns true/false.
 *  If there is no more player to change the turn for, it returns false.
 * 3. Return value(string) added for report_winner() Method to get the message
 *  of the winner directly.
 * 4. Five methods were added to the gameengine module (getId(), getRollsLeft(),
 *  getLatestpoints(), gameReset(), toggleLock(int diceIndex))
 *
 *  I got some help from OpenAI(ChatGPT 3.5) to implement the method
 *  diceButtonClicked().
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <gameengine.hh>
#include <functions.hh>
#include <QTimer>

// If no user input is received, the default number of players is 2.
const int NUMBER_OF_PLAYERS = 2;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // change turn for the next player(if any)
    void cTurnButtonClicked();

    // reset the game to start a new round
    void resetButtonClicked();

    // roll the dice for the player
    void rollButtonClicked();

    // Increment the elapsed time
    void updateTimeElapsed();

    // lock the dice which is clicked
    void diceButtonClicked();

    // Number od players received as a user input, otherwise it is 2.
    void userInputReceived();

private:
    GameEngine engine;
    QTimer* gameTimer;    
    Ui::MainWindow *ui;
    QString originalStyleSheet;

    int elapsedSeconds;
    uint numOfPlayers_ = NUMBER_OF_PLAYERS;

    // required number of players added to the game engine
    void setPlayers();

    //whose turn it is and how many tries left for them get updated
    void updateLabels();

    // the image of the dices get updated
    void updateDices();

};
#endif // MAINWINDOW_HH
