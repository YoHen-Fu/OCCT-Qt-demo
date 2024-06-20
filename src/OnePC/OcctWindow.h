#ifndef OCCTWINDOW_H
#define OCCTWINDOW_H

// Qt库
#include <QWidget>
// 三方库
#include <Aspect_Window.hxx>
// 自定义

class OcctWindow : public Aspect_Window
{
public:
    OcctWindow(QWidget *theWidget, const Quantity_NameOfColor theBackColor = Quantity_NOC_MATRAGRAY );

    // 虚函数实现
public:
    //! Returns True if the window <me> is opened
    //! and False if the window is closed.
    virtual Standard_Boolean IsMapped() const override;
    //! Opens the window <me>.
    virtual void Map() const override;
    //! Closes the window <me>.
    virtual void Unmap() const override;
    //! Apply the resizing to the window <me>.
    virtual Aspect_TypeOfResize DoResize() override;
    //! Apply the mapping change to the window <me>.
    //! and returns TRUE if the window is mapped at screen.
    virtual Standard_Boolean DoMapping() const override;
    //! Returns The Window RATIO equal to the physical
    //! WIDTH/HEIGHT dimensions
    //!   //! Returns The Window POSITION in PIXEL
    virtual void Position (Standard_Integer& X1, Standard_Integer& Y1, Standard_Integer& X2, Standard_Integer& Y2) const override;
    //! Returns The Window SIZE in PIXEL
    virtual void Size (Standard_Integer& Width, Standard_Integer& Height) const override;
    virtual Standard_Real Ratio() const override;
    //! Returns native Window handle (HWND on Windows, Window with Xlib, and so on)
    virtual Aspect_Drawable NativeHandle() const override;
    //! Returns parent of native Window handle (HWND on Windows, Window with Xlib, and so on)
    virtual Aspect_Drawable NativeParentHandle() const override;
    //! Returns native Window FB config (GLXFBConfig on Xlib)
    virtual Aspect_FBConfig NativeFBConfig() const override;
private:
    Standard_Integer myXLeft;
    Standard_Integer myYTop;
    Standard_Integer myXRight;
    Standard_Integer myYBottom;
    QWidget* myWidget;
    double dpiScale_;
};

#endif // OCCTWINDOW_H
