#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "../qcustomplot.h"
#include "../qcustomplot/qcustomplot.h"

extern "C"
{
#include "../c_files/smart_calc.h"
}

//#ifdef __cplusplus
//extern "C" {
//#endif
//#include "c_files/smart_calc.h"

//#ifdef __cplusplus
//}
//#endif


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;


private slots:
    void digits_numbers();
    int get_result(QString str_show_line, double* result);
    void replace_x_value(QString* str_show_line, QString str_x_value);
    double get_y_value(double copy_min_x, QString str_show_line, int* code);
    void handling_errors(int code);

    void on_pushButton_clear_clicked();
    void on_pushButton_equal_clicked();
    void on_pushButton_graph_clicked();
};
#endif // MAINWINDOW_H
