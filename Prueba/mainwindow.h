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

    void step(float t);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
