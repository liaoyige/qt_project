#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPushButton PushButton;
    QPixmap  pix;
    QImage image;
    QImage *newImage;
    QLabel *label;
    QLabel *greylabel;
    QLabel *binlabel;

    int hold;

    QImage * greyImage(QImage *img);
    QImage * binImage(QImage *img);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_spinBox_valueChanged(int arg1);



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
