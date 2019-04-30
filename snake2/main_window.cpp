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
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: First Last, student#, first.last@tuni.fi                 #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <fruit.hh>
#include <QThread>
#include<vector>
#include<QDebug>

using namespace std;


MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent), food_exists(false), score_(0){

    ui_.setupUi(this);

    // We need a graphics scene in which to draw a circle
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates:
    int left_margin = 10; // x coordinate
    int top_margin = 270; // y coordinate
    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui_.graphicsView->setGeometry(left_margin, top_margin,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_.graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1,
    // because the circle is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);


    //ui_.graphicsView->setScene(&scene_);
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::move);
}

void MainWindow::draw_food()
{
    QBrush redBrush(Qt::green);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
    qreal new_x = qrand() % ((BORDER_RIGHT + 1) - 10) + 10;
    qreal new_y = qrand() % ((BORDER_DOWN + 1) - 10) + 10;
    //qreal new_x = 100;
    //qreal new_y = 50;
    food_ = scene_->addEllipse(new_x, new_y, FOOD_SIZE, FOOD_SIZE, blackPen, redBrush);
    food_exists = true;
}

bool MainWindow::empty_place(qreal x, qreal y)
{
    QRectF rect = scene_->sceneRect();
    return rect.contains(x, y);
}

void MainWindow::snake_growing()
{
    flag_ = true;
    int tail = snake_.size() - 1;
    qreal tail_x = snake_[tail]->x();
    qreal tail_y = snake_[tail]->y();
    qreal new_tail_y = 0, new_tail_x = 0;
    qDebug() << tail << " " << tail_x << " " << tail_y;
    if (is_right_)
    {
        if (empty_place(tail_x - STEP, tail_y))
            {
                new_tail_x = tail_x - STEP;
                new_tail_y = tail_y;
            }
        else if (empty_place(tail_x, tail_y + STEP))
        {
             new_tail_y = tail_y + STEP;
             new_tail_x = tail_x;
        }
        else if (empty_place(tail_x, tail_y - STEP))
        {
            new_tail_y = tail_y - STEP;
            new_tail_x = tail_x;
        }
    }
    if (is_left_)
    {
        if (empty_place(tail_x + STEP, tail_y))
            {
                new_tail_x = tail_x + STEP;
                new_tail_y = tail_y;
            }
        else if (empty_place(tail_x, tail_y + STEP))
        {
             new_tail_y = tail_y + STEP;
             new_tail_x = tail_x;
        }
        else if (empty_place(tail_x, tail_y - STEP))
        {
            new_tail_y = tail_y - STEP;
            new_tail_x = tail_x;
        }
    }
    if (empty_place(tail_x + STEP, tail_y))
    {
        new_tail_x = tail_x + STEP;
        new_tail_y = tail_y;
    }
    else if (empty_place(tail_x - STEP, tail_y) and !is_left_)
    {
        new_tail_x = tail_x - STEP;
        new_tail_y = tail_y;
    }
    else if (empty_place(tail_x, tail_y + STEP) and !is_down_)
    {
         new_tail_y = tail_y + STEP;
         new_tail_x = tail_x;
    }
    else if (empty_place(tail_x, tail_y - STEP) and !is_up_)
    {
        new_tail_y = tail_y - STEP;
        new_tail_x = tail_x;
    }
    else
        game_over();
    QBrush blueBrush(Qt::blue);
    QPen blackPen(Qt::black);
    QGraphicsEllipseItem* new_tail = scene_->addEllipse(new_tail_x, new_tail_y, STEP, STEP, blackPen, blueBrush);
    qDebug() << new_tail_x << " " << new_tail_y;
    qDebug() << "OK";
    snake_.push_back(new_tail);
}

void MainWindow::check_eating()
{
    if (snake_[0]->collidesWithItem(food_))
    {
        score_++;
        //snake_growing();
        need_to_grow = true;
        food_exists = false;
        scene_->removeItem(food_);
    }
}

void MainWindow::tail_moving(qreal head_x, qreal head_y)
{
    qreal next_x = head_x;
    qreal next_y = head_y;
    qreal cur_x, cur_y;
    for (size_t i = 1; i < snake_.size(); i++)
    {
        cur_x = snake_[i]->x();
        cur_y = snake_[i]->y();
        qreal deltaX = next_x - cur_x;
        qreal deltaY = next_y - cur_y;
        snake_[i]->moveBy(deltaX, deltaY);
        next_x = cur_x;
        next_y = cur_y;
    }
    if (need_to_grow)
    {
        QBrush blueBrush(Qt::blue);
        QPen blackPen(Qt::black);
        snake_.push_back(scene_->addEllipse(next_x, next_y, STEP, STEP, blackPen, blueBrush));
        qDebug() << snake_[1]->x() << " " << snake_[1]->y();
        need_to_grow = false;
        QThread::msleep(500);
    }
}

void MainWindow::game_over()
{
    ui_.textBrowser->clearHistory();
    ui_.textBrowser->setPlainText("GAME OVER");
    ui_.pushButton->setEnabled(true);
    score_ = 0;
    ui_.lcdNumber->clearFocus();
    ui_.lcdNumber->display(score_);

    scene_->setForegroundBrush(QBrush(Qt::lightGray, Qt::CrossPattern));
    scene_->removeItem(food_);
    scene_->removeItem(snake_[0]);
    food_ = nullptr;
    //snake_[0] = nullptr;
    //delete(snake_);
    timer_.stop();
}

bool MainWindow::cicked_yourself()
{
    for (size_t i = 1; i < snake_.size(); i++)
    {
        if (snake_[0]->collidesWithItem(snake_[i]))
            return true;
    }
    return false;
}

void MainWindow::move()
{
    // Making the earlier text to disappear

    // Current position of the circle
    check_eating();

    if (!food_exists)
        draw_food();

    //if (flag_)
  //  {
       /* qDebug() << snake_[0]->x() << " " << snake_[0]->y();
        qDebug() << snake_[1]->x() << " " << snake_[1]->y();
        qDebug() << "OK";
    }*/

    ui_.lcdNumber->display(score_);

    qreal current_x = snake_[0]->x();
    qreal current_y = snake_[0]->y();

    qDebug() << current_x << " " << current_y;


    // Calculating the displacement that can be either
    // positive or negative, depending on the direction
    qreal deltaX = 0, deltaY = 0;
    if(is_right_)
        deltaX = static_cast<qreal>(STEP);  // right (positive step)
    else if (is_left_)
        deltaX = static_cast<qreal>(-STEP); // left (negative step)
    else if(is_down_)
        deltaY = static_cast<qreal>(STEP);  // down (positive step)
     else if (is_up_)
        deltaY = static_cast<qreal>(-STEP); // up (negative step)

    // New position of the circle
    current_x += deltaX;
    current_y += deltaY;

    // If the new position is in the scene, the circle can be moved there
    // otherwise moving stops
    QRectF rect = scene_->sceneRect();
    if(rect.contains(current_x, current_y)) {
        snake_[0]->moveBy(deltaX, deltaY);
        if (cicked_yourself())
        {
            game_over();
            return;
        }
        tail_moving(current_x - deltaX, current_y - deltaY);
    } else {
        game_over();
    }
}

void MainWindow::on_pushButton_clicked()
{
    // EXAMPLE: How to create new graphics items in the scene.
    scene_->setForegroundBrush(Qt::NoBrush);
    set_dir(INIT);
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    blackPen.setWidth(2);
    ui_.lcdNumber->setEnabled(false);
    QGraphicsEllipseItem* head_ = scene_->addEllipse(0, 0, STEP, STEP, blackPen, redBrush);
    snake_.push_back(head_);

    ui_.lcdNumber->setEnabled(true);
    ui_.lcdNumber->setStyleSheet("background-color: black");
    ui_.lcdNumber->display(score_);
    ui_.textBrowser->clearHistory();
    ui_.textBrowser->setPlainText("GAME ON");
    ui_.pushButton->setEnabled(false);
    timer_.start(1000);
}

void MainWindow::set_dir(const int n)
{
    if (n == UP)
    {
        is_up_ = true;
        is_down_ = false;
        is_left_ = false;
        is_right_ = false;
    }
    if (n == DOWN)
    {
        is_up_ = false;
        is_down_ = true;
        is_left_ = false;
        is_right_ = false;
    }
    if (n == LEFT)
    {
        is_up_ = false;
        is_down_ = false;
        is_left_ = true;
        is_right_ = false;
    }
    if (n == RIGHT)
    {
        is_up_ = false;
        is_down_ = false;
        is_left_ = false;
        is_right_ = true;
    }
    if (n == INIT)
    {
        is_up_ = false;
        is_down_ = false;
        is_left_ = false;
        is_right_ = false;
    }
}

void MainWindow::on_leftButton_clicked()
{
    set_dir(LEFT);
}

void MainWindow::on_upButton_clicked()
{
    set_dir(UP);
}

void MainWindow::on_downButton_clicked()
{
    set_dir(DOWN);
}

void MainWindow::on_rightButton_clicked()
{
    set_dir(RIGHT);
}
