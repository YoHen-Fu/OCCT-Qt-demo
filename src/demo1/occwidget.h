#ifndef OCCWIDGET_H
#define OCCWIDGET_H

#include <QWidget>

#include "V3d_Viewer.hxx"
#include "V3d_view.hxx"
#include "AIS_InteractiveContext.hxx"


class OCCWidget : public QWidget
{
    Q_OBJECT
public:
    OCCWidget(QWidget *parent = nullptr);

protected:
    QPaintEngine* paintEngine() const;
    void paintEvent( QPaintEvent* /*theEvent*/ );
    void resizeEvent( QResizeEvent* /*theEvent*/ );

private:
    Handle(V3d_Viewer) m_viewer = nullptr;
    Handle(V3d_View) m_view = nullptr;
    Handle(AIS_InteractiveContext) m_context = nullptr;
};

#endif // OCCWIDGET_H
