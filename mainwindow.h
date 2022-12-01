#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>

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
    void on_btnShowJson_clicked();
    void on_btnReadJson_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel* viewModel = new QStandardItemModel;
};
#endif // MAINWINDOW_H
