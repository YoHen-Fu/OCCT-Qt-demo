#include "OccViewWidget.h"
#include "occtwindow.h"

#include <QResizeEvent>


OccViewWidget::OccViewWidget(const Handle_V3d_View& view, QWidget* parent)
    : QWidget(parent),
    m_view(view)
{
    this->setMouseTracking(false);
    this->setBackgroundRole(QPalette::NoRole);

    // Avoid Qt background clears to improve resizing speed
    this->setAutoFillBackground(false);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->setAttribute(Qt::WA_PaintOnScreen);
}

const Handle_V3d_View &OccViewWidget::v3dView() const
{
    return m_view;
}

QPaintEngine* OccViewWidget::paintEngine() const
{
    return nullptr;
}

void OccViewWidget::showEvent(QShowEvent*)
{
    if (m_view->Window().IsNull()) {
        Handle_Aspect_Window hWnd = new OcctWindow(this);
        m_view->SetWindow(hWnd);
        if (!hWnd->IsMapped())
            hWnd->Map();
        m_view->MustBeResized();
    }
}

void OccViewWidget::paintEvent(QPaintEvent*)
{
    m_view->Redraw();
}

void OccViewWidget::resizeEvent(QResizeEvent* event)
{
    if (!event->spontaneous()) // Workaround for infinite window shrink on Ubuntu
        m_view->MustBeResized();
}
