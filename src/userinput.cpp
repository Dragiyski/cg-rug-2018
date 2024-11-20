#include <QDebug>

#include "mainview.h"

/**
 * @brief MainView::keyPressEvent Triggered by a key press.
 * @param ev Key event.
 */
void MainView::keyPressEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
    case 'A':
        qDebug() << "A pressed";
        break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it
        // equivalent with the char value ('A' == 65, '1' == 49) Alternatively,
        // you could use Qt Key enums, see http://doc.qt.io/qt-6/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }
    // Used to update the screen after changes
    update();
}

// Triggered by releasing a key
/**
 * @brief MainView::keyReleaseEvent Triggered by a key released.
 * @param ev Key event.
 */
void MainView::keyReleaseEvent(QKeyEvent *ev)
{
    switch (ev->key()) {
    case 'A':
        qDebug() << "A released";
        break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    update();
}

/**
 * @brief MainView::mouseDoubleClickEvent Triggered by clicking two subsequent
 * times on any mouse button. It also fires two mousePress and mouseRelease
 * events.
 * @param ev Mouse events.
 */
void MainView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse double clicked:" << ev->button();

    update();
}

/**
 * @brief MainView::mouseMoveEvent Triggered when moving the mouse inside the
 window (only when the mouse is clicked).
 * @param ev Mouse event.
 */
void MainView::mouseMoveEvent(QMouseEvent *ev)
{
    if (m_drag.dragType == 1) {
        int deltaX = ev->position().x() - m_drag.initialX;
        int deltaY = ev->position().y() - m_drag.initialY;
        QVector3D deltaRotation(fmod(fmod(deltaY / (double) height(), 1) + 1, 1.0) * 360,
                                fmod(fmod(deltaX / (double) width(), 1) + 1, 1.0) * 360,
                                0);

        m_state.rotation = m_drag.initialRotation + deltaRotation;
        m_state.rotation = QVector3D(fmod(360.0 + m_state.rotation.x(), 360.0),
                                     fmod(360.0 + m_state.rotation.y(), 360.0),
                                     fmod(360.0 + m_state.rotation.z(), 360.0));
        updateViewTransform();
        qDebug() << "View Rotation: [" << m_state.rotation.x() << ", " << m_state.rotation.y()
                 << ", " << m_state.rotation.z() << "]";
        emit rotationChanged(m_state.rotation);
    }

    update();
}

/**
 * @brief MainView::mousePressEvent Triggered when pressing any mouse button.
 * @param ev Mouse event.
 */
void MainView::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == 1) {
        m_drag.dragType = 1;
        m_drag.initialX = ev->position().x();
        m_drag.initialY = ev->position().y();
        m_drag.initialRotation = m_state.rotation;
    }

    update();
    // Do not remove the line below, clicking must focus on this widget!
    this->setFocus();
}

/**
 * @brief MainView::mouseReleaseEvent Triggered when releasing any mouse button.
 * @param ev Mouse event.
 */
void MainView::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == 1) {
        m_drag.dragType = 0;
    }

    update();
}

/**
 * @brief MainView::wheelEvent Triggered when clicking scrolling with the scroll
 * wheel on the mouse.
 * @param ev Mouse event.
 */
void MainView::wheelEvent(QWheelEvent *ev)
{
    auto delta = ev->angleDelta().x() + ev->angleDelta().y();
    if(delta < 0) {
        m_state.scale = std::max(m_state.scale - 1.0f, 0.0f);
    } else if (delta > 0) {
        m_state.scale = std::min(m_state.scale + 1.0f, 200.0f);
    } else {
        return;
    }

    qDebug() << "Zoom: " << m_state.scale;

    updateViewTransform();

    emit scaleChanged(m_state.scale);
    // Implement something
    qDebug() << "Mouse wheel:" << ev->angleDelta();

    update();
}
