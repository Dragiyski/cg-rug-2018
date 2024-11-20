#include "mainwindow.h"

#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow Constructs a new main window.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , inRotationUpdate(false)
    , inScaleUpdate(false)
    , inLockUpdate(false)
{
    ui->setupUi(this);
}

/**
 * @brief MainWindow::~MainWindow Destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_ResetRotationButton_clicked Resets the rotation.
 * @param checked Unused.
 */
void MainWindow::on_ResetRotationButton_clicked(bool checked)
{
    Q_UNUSED(checked)
    ui->RotationDialX->setValue(0);
    ui->RotationDialY->setValue(0);
    ui->RotationDialZ->setValue(0);
    ui->mainView->setRotation(0, 0, 0);
}

/**
 * @brief MainWindow::on_RotationDialX_sliderMoved Updates the number of degrees
 * of rotation in the x direction.
 * @param value Unused.
 */
void MainWindow::on_RotationDialX_sliderMoved(int value)
{
    if (inRotationUpdate) {
        return;
    }

    inRotationUpdate = true;
    ui->mainView->setRotation(value, ui->RotationDialY->value(), ui->RotationDialZ->value());
    inRotationUpdate = false;
    ui->mainView->update();
}

/**
 * @brief MainWindow::on_RotationDialY_sliderMoved Updates the number of degrees
 * of rotation in the y direction.
 * @param value Unused.
 */
void MainWindow::on_RotationDialY_sliderMoved(int value)
{
    if (inRotationUpdate) {
        return;
    }

    inRotationUpdate = true;
    ui->mainView->setRotation(ui->RotationDialX->value(), value, ui->RotationDialZ->value());
    inRotationUpdate = false;
    ui->mainView->update();
}

/**
 * @brief MainWindow::on_RotationDialZ_sliderMoved Updates the number of degrees
 * of rotation in the z direction.
 * @param value Unused.
 */
void MainWindow::on_RotationDialZ_sliderMoved(int value)
{
    if (inRotationUpdate) {
        return;
    }

    inRotationUpdate = true;
    ui->mainView->setRotation(ui->RotationDialX->value(), ui->RotationDialY->value(), value);
    inRotationUpdate = false;
    ui->mainView->update();
}

/**
 * @brief MainWindow::on_ResetScaleButton_clicked Resets the scale factor.
 * @param checked Unused.
 */
void MainWindow::on_ResetScaleButton_clicked(bool checked)
{
    Q_UNUSED(checked)
    ui->ScaleSlider->setValue(100);
    ui->mainView->setScale(100);
    ui->mainView->update();
}

/**
 * @brief MainWindow::on_ScaleSlider_sliderMoved Updates the scale value.
 * @param value The new scale value.
 */
void MainWindow::on_ScaleSlider_sliderMoved(int value)
{
    ui->mainView->setScale(value);
    ui->mainView->update();
}

void MainWindow::on_PhongButton_toggled(bool checked)
{
    if (checked) {
        ui->mainView->setShadingMode(MainView::PHONG);
        ui->mainView->update();
    }
}

void MainWindow::on_NormalButton_toggled(bool checked)
{
    if (checked) {
        ui->mainView->setShadingMode(MainView::NORMAL);
        ui->mainView->update();
    }
}

void MainWindow::on_GouraudButton_toggled(bool checked)
{
    if (checked) {
        ui->mainView->setShadingMode(MainView::GOURAUD);
        ui->mainView->update();
    }
}

void MainWindow::on_CameraLock_currentTextChanged(const QString &name)
{
    if (inLockUpdate) {
        return;
    }
    ui->mainView->setLockObject(name);
    ui->mainView->update();
}

void MainWindow::on_mainView_rotationChanged(QVector3D rotation)
{
    // Rounding in interval [0, 360) + epsilon error
    // Could result in a value less than 0, so cast to integer alone is unsafe.
    // Error is small (definitely smaller than 360), so we can add 360 making it
    // positive Then use mod to cut it off, also positive epsilon error can result
    // in value >= 360, which is also taken care of with mod.
    int x, y, z;
    x = (((int) (round(rotation.x()))) + 360) % 360;
    y = (((int) (round(rotation.y()))) + 360) % 360;
    z = (((int) (round(rotation.z()))) + 360) % 360;

    inRotationUpdate = true;
    ui->RotationDialX->setValue(x);
    ui->RotationDialY->setValue(y);
    ui->RotationDialZ->setValue(z);
    inRotationUpdate = false;
}

void MainWindow::on_mainView_scaleChanged(float factor)
{
    int value = std::min(std::max((int) (round(factor)), 0), 200);

    inScaleUpdate = true;
    ui->ScaleSlider->setValue(value);
    inScaleUpdate = false;
}

void MainWindow::on_mainView_namedMeshAdded(const QString &name)
{
    ui->CameraLock->addItem(name);
}

/**
 * @brief MainWindow::renderToFile Used to render the frame buffer to the file.
 * DO NOT REMOVE OR MODIFY!
 */
void MainWindow::renderToFile()
{
    QImage image = ui->mainView->grabFramebuffer();
    image.save("framebuffer_contents.png", "PNG",
               100); // Save using the best quality
}
