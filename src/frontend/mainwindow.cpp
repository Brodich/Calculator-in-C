#include "mainwindow.h"

#include "./ui_mainwindow.h"
//#include "qcustomplot.h"
//#include "./mainwindow.ui"

#define SIZE_VECTORS_GRAPH 2000
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->customplot->addGraph();

  //  this->setStyleSheet("background-color: #ffffff;");

  double min_X = -10;
  double max_X = 10;
  double min_Y = -10;
  double max_Y = 10;

  ui->customplot->xAxis->setLabel("X");
  ui->customplot->yAxis->setLabel("Y");
  ui->customplot->yAxis->setRange(min_Y, max_Y);
  ui->customplot->xAxis->setRange(min_X, max_X);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this,
          SLOT(add_symbol()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_divide, SIGNAL(clicked()), this,
          SLOT(add_symbol()));
  connect(ui->pushButton_power, SIGNAL(clicked()), this,
          SLOT(add_symbol()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->pushButton_bracket_1, SIGNAL(clicked()), this, SLOT(add_symbol()));
  connect(ui->pushButton_bracket_2, SIGNAL(clicked()), this, SLOT(add_symbol()));

  connect(ui->result_show, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_equal_clicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::add_symbol() {
  QPushButton* button = (QPushButton*)sender();

  if (ui->result_show->text() == "0") {
    ui->result_show->setText(button->text());
  } else {
    ui->result_show->setText(ui->result_show->text() + button->text());
  }
}

void MainWindow::on_pushButton_clear_clicked() {
  ui->result_show->setText("0");
}

void MainWindow::on_pushButton_equal_clicked() {
  int code = 0;
  double result = 0;
  QString str_show_line = ui->result_show->text();
  QString str_x_value = ui->line_x_value->text();

  replace_x_value(&str_show_line, str_x_value);

  code = get_result(str_show_line, &result);

  if (code == SUCCESS) {
    ui->result_show->setText(QString::number(result, 'g', 16));
  }

  handling_errors(code);
}

void MainWindow::handling_errors(int code) {
  switch (code) {
    case SUCCESS:
      break;
    case ERROR:
      ui->result_show->setText("ERROR");
      break;
    case ERROR_DIV_BY_ZERO:
      ui->result_show->setText("ERROR_DIV_BY_ZERO");
      break;
    case ERROR_NOT_CORRECT_SYMBOLE:
      ui->result_show->setText("ERROR_NOT_CORRECT_SYMBOLE");
      break;
    case ERROR_MANY_DOTS:
      ui->result_show->setText("ERROR_MANY_DOTS");
      break;
    case ERROR_BRACKET:
      ui->result_show->setText("ERROR_BRACKET");
      break;
    case ERROR_NAN:
      ui->result_show->setText("ERROR_NAN");
      break;
    default:
      ui->result_show->setText("ERROR");
      break;
  }
}

void MainWindow::on_pushButton_graph_clicked() {
  double min_X = ui->doubleSpinBox_Xmin->value();
  double max_X = ui->doubleSpinBox_Xmax->value();
  double min_Y = ui->doubleSpinBox_Ymin->value();
  double max_Y = ui->doubleSpinBox_Xmax->value();

  ui->customplot->addGraph();
  //  ui->customplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
  ui->customplot->graph(0)->setLineStyle(QCPGraph::lsLine);
  //  ui->customplot->graph(0)->setScatterSkip(SIZE_VECTORS_GRAPH - 2);

  QVector<double> x(SIZE_VECTORS_GRAPH), y(SIZE_VECTORS_GRAPH);

  ui->customplot->setInteractions(QCP::iRangeDrag |
                                  QCP::iRangeZoom);  // QCP::iSelectPlottables
  ui->customplot->xAxis->setLabel("X");
  ui->customplot->yAxis->setLabel("Y");
  ui->customplot->yAxis->setRange(min_Y, max_Y);
  ui->customplot->xAxis->setRange(min_X, max_X);

  double step = (max_X - min_X) / (SIZE_VECTORS_GRAPH - 1);
  double copy_min_x = min_X;

  int code = 0;
  double result = 0;
  QString str_show_line = ui->result_show->text();
  QString copy = ui->result_show->text();

  QString str_min_x = QString::number(copy_min_x, 'f', 16);

  replace_x_value(&copy, str_min_x);
  code = get_result(copy, &result);
  int i = 0;
  if (code == SUCCESS) {
    while (i < SIZE_VECTORS_GRAPH && code == SUCCESS) {
      x[i] = copy_min_x;
      y[i] = get_y_value(copy_min_x, str_show_line, &code);
      copy_min_x += step;
      //      qDebug() << "copy_min_x " + QString::number(copy_min_x);
      i++;
    }

    ui->customplot->graph(0)->setData(x, y);
    //    ui->customplot->rescaleAxes();
    ui->customplot->replot();
    ui->customplot->update();
  }
  handling_errors(code);
}

double MainWindow::get_y_value(double copy_min_x, QString str_show_line,
                               int* code) {
  double result = 0;
  QString str_min_x = QString::number(copy_min_x, 'f', 16);
  replace_x_value(&str_show_line, str_min_x);
  *code = get_result(str_show_line, &result);
  return (result);
}

void MainWindow::replace_x_value(QString* str_show_line, QString str_x_value) {
  size_t pos = str_show_line->indexOf('x');

  if (pos != std::string::npos) {
    str_show_line->replace("x", "(" + str_x_value + ")");
  }
}

int MainWindow::get_result(QString str_show_line, double* result) {
  int code = 0;
  char polish[255] = {0};

  QByteArray array_input = str_show_line.toUtf8();
  char* input = array_input.data();

  code = ft_from_infix_to_postfix(input, polish);

  if (code == SUCCESS) {
    code = ft_calcul_polish(polish, result);
  }
  return (code);
}
