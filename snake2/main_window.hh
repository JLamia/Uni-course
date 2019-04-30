/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: First Last, student#, first.last@tuni.fi                 #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include<vector>
#include <random>

using namespace std;


/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);
    QGraphicsScene* scene_;              /**< Manages drawable objects. */

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    //QGraphicsScene* scene_;          // a surface for

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */

    //vector<QGraphicsEllipseItem*> head_;   // drawing a circle

    vector<QGraphicsEllipseItem*> snake_;

    bool is_right_;
    bool is_down_;
    bool is_up_;
    bool is_left_;
    void set_dir(const int);
    bool flag_ = false;
    bool need_to_grow = false;


private slots:

    /* \brief Start the game.
     */

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */


    //void on_playButton_clicked();

    void on_pushButton_clicked();

    void snake_growing();

    bool empty_place(qreal x, qreal y);

    void tail_moving(qreal x, qreal y);

    void on_upButton_clicked();

    bool cicked_yourself();

    void on_leftButton_clicked();

    void move();

    void on_downButton_clicked();

    void on_rightButton_clicked();

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */

    void draw_food();

    void check_eating();

    void game_over();

    int score_;

    bool food_exists;

    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */
    QGraphicsEllipseItem* food_ = nullptr; /**< The food item in the scene. */
    QTimer timer_;                      /**< Triggers the Snake to move. */
    std::default_random_engine rng_;    /**< Randomizes food locations. */

    const int STEP = 20;            // how much to move at a time
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 260;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 680;
    const int UP = 1;
    const int DOWN = 2;
    const int LEFT = 3;
    const int RIGHT = 4;
    const int INIT = 0;
    const int FOOD_SIZE = 10;


};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
