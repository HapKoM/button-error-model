#include <QPushButton>
#include <QDateTime>
#include <QDebug>
#include <QTextStream>
#include <QMouseEvent>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QLabel>
#include <qmath.h>

#include <qglobal.h>

#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#include <QGuiApplication>
#else
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QtGui/QAction>
#endif

#include <QSettings>
#include <QTimer>
#include <QScreen>
#include <QCursor>
#include <QKeyEvent>

#include <limits>
#include <random>

#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


// Функция для совместимости с Qt4.6
qint64 toMSecsSinceEpoch(const QDateTime& dt) {
    qDebug() << dt.toTime_t() << dt.time().msec();
    quint64 secSinceEpoch = dt.toTime_t();
    return secSinceEpoch*1000 + dt.time().msec();
}

// Функция для совместимости с Qt4.6
qint64 msecsTo(const QDateTime& from, const QDateTime& to) {
    return toMSecsSinceEpoch(to) - toMSecsSinceEpoch(from);
}

const int MainWindow::MAX_MOTION_PAUSE_MS = 5000;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), lastMotionStartTime_(QDateTime::currentDateTime()), firstMotionFlag_(true),
    ctr_(0)
{
    qsrand(toMSecsSinceEpoch(QDateTime::currentDateTime())%std::numeric_limits<uint>::max());
    ui->setupUi(this);
    loadSettings();
    pushButton_ = ui->pushButton;
    connect(pushButton_, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
#if QT_VERSION >= 0x050000
    QString docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    qDebug() << "Use doc path for Qt5: " << docPath;
#else
    QString docPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    qDebug() << "Use doc path for Qt5: " << docPath;
#endif
    QString fileName(QString("%1/%2.csv").arg(docPath)
                     .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss")));
    outFile_ = new QFile(fileName);
    if (!outFile_->open(QIODevice::WriteOnly)) {
        qDebug() << "failed to open file " << fileName;
    } else {
        qDebug() << "file " << QFileInfo(*outFile_).absoluteFilePath() << "opened";
        QTextStream ts(outFile_);
        ts << "CUR_MS" << "; "
           << "B_X" << "; " << "B_Y" << "; "
           << "B_W" << "; " << "B_H" << "; "
           << "P_X" << "; " << "P_Y" << "; " << "P_MS" << "; "
           << "S_X" << "; " << "S_Y" << "; " << "S_MS" << "; " << "ERR" << "; "
           << "AGE" << "; " << "C_S" << "\n";
    }
    moveAndResizeButton();
    centralWidget()->setMouseTracking(true);
    pushButton_->setMouseTracking(true);
    setMouseTracking(true);
    QTimer::singleShot(50, this, SLOT(showSettingsDialog()));
    lastPointTime_ = QDateTime::currentDateTime();
}

MainWindow::~MainWindow()
{
    qDebug() << "File " << QFileInfo(*outFile_).absoluteFilePath() << " closed";
    outFile_->close();
    saveSettings();
    delete outFile_;
    delete ui;
}

void MainWindow::buttonClicked()
{
    qDebug() << "MainWindow::buttonClicked() " << mapFromGlobal(QCursor::pos());
    QDateTime now = QDateTime::currentDateTime();
    //qDebug() << toMSecsSinceEpoch(lastPointTime_);
    //qDebug() << msecsTo(lastPointTime_, now);
    if (msecsTo(lastPointTime_, now) > MAX_MOTION_PAUSE_MS) {
        firstMotionFlag_ = true;
    } else {
        firstMotionFlag_ = false;
//#ifdef ANDROID
        lastMotionStart_ = lastPoint_;
        lastMotionStartTime_ = lastPointTime_;
        lastPoint_ = mapFromGlobal(QCursor::pos());
        lastPointTime_ = now;
//#endif
    }
    moveAndResizeButton();
}

void MainWindow::moveAndResizeButton()
{
    qDebug() << "MainWindow::moveAndResizeButton()";
    writeToFile(false);
    int w0 = width();
    int h0 = height();
#ifndef ANDROID
    int min_pixel_size = 10;
#else
    int min_pixel_size = 30;
#endif
    bool again = false;
    if (qrand() % 2 == 1) {
        int w = qrand() % w0;
        int h = qrand() % h0;
        if (w < min_pixel_size)
            again = true;
        if (h < min_pixel_size)
            again = true;
        int x = qrand() % (w0 - w);
        int y = qrand() % (h0 - h);
        if (!again) {
           pushButton_->setGeometry(x, y, w, h);
        }
    } else {
        int x = qrand() % (w0 - min_pixel_size);
        int y = qrand() % (h0 - min_pixel_size);
        int w = qrand() % (w0 - x);
        int h = qrand() % (h0 - y);
        if (w < min_pixel_size)
            again = true;
        if (h < min_pixel_size)
            again = true;
        if (!again) {
            pushButton_->setGeometry(x, y, w, h);
        }
    }
    if (again) {
        moveAndResizeButton();
    }
    ctr_++;
    pushButton_->setText(tr("Button (%1)").arg(ctr_));
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog sd;
    sd.setAge(age_);
    sd.setCompSkill(compSkillYear_);
    sd.exec();
    if (sd.result() == QDialog::Rejected) {
        QApplication::exit(1);
    }
    age_ = sd.getAge();
    compSkillYear_ = sd.getCompSkill();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
/*#ifndef ANDROID
    //qDebug() << "mouseMoveEvent()";
    lastPoint_ = e->pos();
    QDateTime now = QDateTime::currentDateTime();
    if (msecsTo(lastPointTime_, now) > MAX_MOTION_PAUSE_MS) {
        //qDebug() << "new motion started from " << lastPoint_;
        lastMotionStart_ = lastPoint_;
        lastMotionStartTime_ = now;
    }
    lastPointTime_ = now;
#endif*/
    /*QPointF p = e->pos();
    QPointF r = pushButton_->pos();
    QPointF s = QPointF(pushButton_->width(), pushButton_->height());
    double d = fittsDistance(p, r, s);
    double l = fittsSize(p, r, s);
    ui->fittsLabel->setText(QString("d = %1; s = %2").arg(d).arg(l));*/
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "MainWindow::mousePressEvent";
    QDateTime now = QDateTime::currentDateTime();
    if (msecsTo(lastPointTime_, now) > MAX_MOTION_PAUSE_MS) {
        firstMotionFlag_ = true;
    } else {
        firstMotionFlag_ = false;
//#ifdef ANDROID
        lastMotionStart_ = lastPoint_;
        lastMotionStartTime_ = lastPointTime_;
        lastPoint_ = e->pos();
        lastPointTime_ = now;
//#endif
    }
    writeToFile(true);
//#ifdef ANDROID
//#endif
    qDebug() << "MainWindow::mousePressEvent()" << toMSecsSinceEpoch(lastMotionStartTime_) << (lastPointTime_);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        close();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("RSCE", "button_error_model");
    age_ = settings.value("age", "0").toInt();
    compSkillYear_ = settings.value("skill", "0").toInt();
}

void MainWindow::saveSettings()
{
    QSettings settings("RSCE", "button_error_model");
    settings.setValue("age", age_);
    settings.setValue("skill", compSkillYear_);
}

double MainWindow::distance2d(QPointF p1, QPointF p2)
{
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    return qSqrt(dx*dx + dy*dy);
}

QPointF MainWindow::middle2d(QPointF p1, QPointF p2)
{
    return QPointF((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
}

QPointF MainWindow::rect_center(QPointF r, QPointF s)
{
    return QPointF(r.x() + s.x() / 2, r.y() + s.y() / 2);
}

double MainWindow::fittsDistance(QPointF p, QPointF r, QPointF s)
{
    double k = 1;
    if ((p.x() > r.x()) && (p.x() < r.x() + s.x()) &&
        (p.y() > r.y() && p.y() < r.y() + s.y())) {
        // p внутри кнопки
        k = -1;
    }
    QPointF c = rect_center(r, s);
    double x1 = qAbs(p.x() - c.x());
    double x2 = qAbs(s.x()/2);
    double y1 = qAbs(p.y() - c.y());
    double y2 = qAbs(s.y()/2);
    double d = distance2d(p, c);
    if (y1*x2 < y2*x1) {
        return k*qAbs(d*(x1 - x2)/x1);
    } else {
        return k*qAbs(d*(y1 - y2)/y1);
    }
}

double MainWindow::fittsSize(QPointF p, QPointF r, QPointF s)
{
    QPointF c = rect_center(r, s);
    double x1 = qAbs(p.x() - c.x());
    double x2 = qAbs(s.x()/2);
    double y1 = qAbs(p.y() - c.y());
    double y2 = qAbs(s.y()/2);
    double d = distance2d(p, c);
    if (y1*x2 < y2*x1) {
        return 2*d*x2/x1;
    } else {
        return 2*d*y2/y1;
    }
}

void MainWindow::writeToFile(bool error)
{
    if (firstMotionFlag_) {
        firstMotionFlag_ = false;
        return;
    }
    if ((lastMotionStart_.x() == 0) && (lastMotionStart_.y() == 0))
        return;
    if (toMSecsSinceEpoch(lastMotionStartTime_) <= 0)
        return;
    if (toMSecsSinceEpoch(lastPointTime_) <= 0)
        return;
    qDebug() << "MainWindow::writeToFile()";
#if QT_VERSION >= 0x050000
    float dpmx = qApp->primaryScreen()->physicalDotsPerInchX()/25.4;
    float dpmy = qApp->primaryScreen()->physicalDotsPerInchY()/25.4;
#else
    float dpmx = QApplication::desktop()->physicalDpiX()/25.4;
    float dpmy = QApplication::desktop()->physicalDpiY()/25.4;
#endif
    /*QPoint p(lastMotionStart_.x()/dpmx, lastMotionStart_.y()/dpmy);
    QPoint r(pushButton_->x()/dpmx, pushButton_->y()/dpmy);
    QPoint s(pushButton_->width()/dpmx, lastPoint_.y()/dpmy);
    float d = fittsDistance(p, r, s);
    float l = fittsSize(p, r, s);*/

    qDebug() << "dpmx = " << dpmx << "; dpmy = " << dpmy;
    QTextStream ts(outFile_);
    ts << toMSecsSinceEpoch(QDateTime::currentDateTime()) << "; "
       << pushButton_->x()/dpmx << "; " << pushButton_->y()/dpmy << "; "
       << pushButton_->width()/dpmx << "; " << pushButton_->height()/dpmy << "; "
       << lastPoint_.x()/dpmx << "; " << lastPoint_.y()/dpmy << "; " << toMSecsSinceEpoch(lastPointTime_) << "; "
       << lastMotionStart_.x()/dpmx << "; " << lastMotionStart_.y()/dpmy << "; " << toMSecsSinceEpoch(lastMotionStartTime_) << "; " << error << "; "
       << age_ << "; " << compSkillYear_ << "\n";
}
