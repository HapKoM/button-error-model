#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QPushButton;
class QSpinBox;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void setAge(int age);
    void setCompSkill(int skill);
    int getAge();
    int getCompSkill();
private:
    Ui::SettingsDialog *ui;
    QPushButton* acceptButton_;
    QPushButton* rejectButton_;
    QSpinBox* ageSpinBox_;
    QSpinBox* compSkillSpinBox_;
};

#endif // SETTINGSDIALOG_H
