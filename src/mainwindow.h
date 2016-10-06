#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void buttonClicked();
    void moveAndResizeButton();
    void showSettingsDialog();
protected:
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent* e);
    void loadSettings();
    void saveSettings();
private:
    static double distance2d(QPointF p1, QPointF p2);
    static QPointF middle2d(QPointF p1, QPointF p2);
    static QPointF rect_center(QPointF r, QPointF s);
    static double fittsDistance(QPointF p, QPointF r, QPointF s);
    static double fittsSize(QPointF p, QPointF r, QPointF s);
    void writeToFile(bool error);
    Ui::MainWindow *ui;
    QPushButton* pushButton_;
    QFile* outFile_;
    static const int MAX_MOTION_PAUSE_MS;
    QPoint lastMotionStart_;
    QPoint lastPoint_;
    QDateTime lastMotionStartTime_;
    QDateTime lastPointTime_;
    bool firstMotionFlag_;
    //! Возраст экспериментатора (полных лет)
    int age_;
    //! Опыт работы с компьютером (лет)
    int compSkillYear_;
    quint32 ctr_;
};

#endif // MAINWINDOW_H
