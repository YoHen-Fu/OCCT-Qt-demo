#include "OcctWindow.h"
#include "Settings.h"


OcctWindow::OcctWindow(QWidget* theWidget, const Quantity_NameOfColor theBackColor)
    : Aspect_Window(),
    myWidget(theWidget),
    dpiScale_(Settings::instance()->getdpiScale())
{
    SetBackground (theBackColor);
    myXLeft   = qRound(dpiScale_*myWidget->rect().left());
    myYTop    = qRound(dpiScale_*myWidget->rect().top());
    myXRight  = qRound(dpiScale_*myWidget->rect().right());
    myYBottom = qRound(dpiScale_*myWidget->rect().bottom());
}


//! Returns True if the window <me> is opened
//! and False if the window is closed.
Standard_Boolean OcctWindow::IsMapped() const{
    return !(myWidget->isMinimized() || myWidget->isHidden());
}
//! Opens the window <me>.
void OcctWindow::Map() const{
    myWidget->show();
    myWidget->update();
}
//! Closes the window <me>.
void OcctWindow:: Unmap() const{
    myWidget->hide();
    myWidget->update();
}
//! Apply the resizing to the window <me>.
Aspect_TypeOfResize OcctWindow::DoResize(){
    int                 aMask = 0;
    Aspect_TypeOfResize aMode = Aspect_TOR_UNKNOWN;

    if ( !myWidget->isMinimized() )
    {
        if ( Abs ( dpiScale_*myWidget->rect().left()   - myXLeft   ) > 2 ) aMask |= 1;
        if ( Abs ( dpiScale_*myWidget->rect().right()  - myXRight  ) > 2 ) aMask |= 2;
        if ( Abs ( dpiScale_*myWidget->rect().top()    - myYTop    ) > 2 ) aMask |= 4;
        if ( Abs ( dpiScale_*myWidget->rect().bottom() - myYBottom ) > 2 ) aMask |= 8;

        switch ( aMask )
        {
        case 0:
            aMode = Aspect_TOR_NO_BORDER;
            break;
        case 1:
            aMode = Aspect_TOR_LEFT_BORDER;
            break;
        case 2:
            aMode = Aspect_TOR_RIGHT_BORDER;
            break;
        case 4:
            aMode = Aspect_TOR_TOP_BORDER;
            break;
        case 5:
            aMode = Aspect_TOR_LEFT_AND_TOP_BORDER;
            break;
        case 6:
            aMode = Aspect_TOR_TOP_AND_RIGHT_BORDER;
            break;
        case 8:
            aMode = Aspect_TOR_BOTTOM_BORDER;
            break;
        case 9:
            aMode = Aspect_TOR_BOTTOM_AND_LEFT_BORDER;
            break;
        case 10:
            aMode = Aspect_TOR_RIGHT_AND_BOTTOM_BORDER;
            break;
        default:
            break;
        }  // end switch

        *( ( Standard_Integer* )&myXLeft  ) = qRound(dpiScale_*myWidget->rect().left());
        *( ( Standard_Integer* )&myXRight ) = qRound(dpiScale_*myWidget->rect().right());
        *( ( Standard_Integer* )&myYTop   ) = qRound(dpiScale_*myWidget->rect().top());
        *( ( Standard_Integer* )&myYBottom) = qRound(dpiScale_*myWidget->rect().bottom());
    }

    return aMode;
}
//! Apply the mapping change to the window <me>.
//! and returns TRUE if the window is mapped at screen.
Standard_Boolean OcctWindow::DoMapping() const{
    return Standard_True;
};
void OcctWindow::Position (Standard_Integer& X1, Standard_Integer& Y1, Standard_Integer& X2, Standard_Integer& Y2) const {
    X1 = qRound(dpiScale_*myWidget->rect().left());
    X2 = qRound(dpiScale_*myWidget->rect().right());
    Y1 = qRound(dpiScale_*myWidget->rect().top());
    Y2 = qRound(dpiScale_*myWidget->rect().bottom());
}
//! Returns The Window SIZE in PIXEL
void OcctWindow::Size (Standard_Integer& Width, Standard_Integer& Height) const{
    QRect aRect = myWidget->rect();
    Width  = dpiScale_*aRect.width();
    Height = dpiScale_*aRect.height();
}
//! Returns The Window RATIO equal to the physical
//! WIDTH/HEIGHT dimensions
Standard_Real OcctWindow::Ratio() const{
    QRect aRect = myWidget->rect();
    return Standard_Real( aRect.right() - aRect.left() ) / Standard_Real( aRect.bottom() - aRect.top() );
}

//! Returns native Window handle (HWND on Windows, Window with Xlib, and so on)
Aspect_Drawable OcctWindow::NativeHandle() const{
    return (Aspect_Drawable)myWidget->winId();
}
//! Returns parent of native Window handle (HWND on Windows, Window with Xlib, and so on)
Aspect_Drawable OcctWindow::NativeParentHandle() const{
    QWidget* parentWidget = myWidget->parentWidget();
    if (parentWidget) {
        return (Aspect_Drawable)parentWidget->winId();
    }else{
        return 0;
    }
}
//! Returns native Window FB config (GLXFBConfig on Xlib)
Aspect_FBConfig OcctWindow::NativeFBConfig() const{
    return nullptr;
}
