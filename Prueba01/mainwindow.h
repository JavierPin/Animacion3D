#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMatrix4x4>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void interpolacion_lineal();
    void interpolacion_esferica();


private slots:
    void on_bt_lineal_clicked();

    void on_bt_esferica_clicked();

    void playLineal();
    void playEsferica();

    void step(float t);


    void on_bt_play_clicked();

    void on_bt_stop_clicked();

    void on_sb_t_valueChanged(double arg1);


    void on_hs_t_sliderMoved(int position);


    void on_bt_play_2_clicked();

    void on_bt_stop_2_clicked();

private:
    Ui::MainWindow *ui;
     bool stop=false;
};

#endif // MAINWINDOW_H
