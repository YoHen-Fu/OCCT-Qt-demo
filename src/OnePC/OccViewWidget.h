#ifndef OCCVIEWWIDGET_H
#define OCCVIEWWIDGET_H

#include <QWidget>

#include <V3d_View.hxx>

class OccViewWidget : public QWidget {
    Q_OBJECT
public:
    OccViewWidget(const Handle_V3d_View& view, QWidget* parent = nullptr);

    const Handle_V3d_View& v3dView() const;

    QPaintEngine* paintEngine() const override;

protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    Handle_V3d_View m_view;
};

#endif // OCCVIEWWIDGET_H
