#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_plusRow_clicked();

    void on_minusRow_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_random_clicked();

    void on_sum_clicked();

    void on_max_clicked();

    void on_table_cellChanged(int row, int column);

    void on_min_clicked();

    void on_avg_clicked();

    void on_mul_clicked();

    void on_sort_clicked();

    void on_del_clicked();

    void on_search_clicked();

    void on_fileOpen_clicked();

    int MessageBox();

    void on_fileSave_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
