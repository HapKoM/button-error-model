#include <QPushButton>
#include <QSpinBox>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    acceptButton_ = ui->acceptButton;
    rejectButton_ = ui->rejectButton;
    ageSpinBox_ = ui->ageSpinBox;
    compSkillSpinBox_ = ui->compSkillSpinBox;
    connect(acceptButton_, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(rejectButton_, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setAge(int age)
{
    ageSpinBox_->setValue(age);
}

void SettingsDialog::setCompSkill(int skill)
{
    compSkillSpinBox_->setValue(skill);
}

int SettingsDialog::getAge()
{
    return ageSpinBox_->value();
}

int SettingsDialog::getCompSkill()
{
    return compSkillSpinBox_->value();
}
