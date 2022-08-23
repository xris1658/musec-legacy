#include "Render.hpp"

#include "native/Native.hpp"

#include <QQuickWindow>

namespace Musec::UI::Render
{
void setSystemRender()
{
    // Check if the application is being debugged. If yes, then system rendering is disabled.
    // On Windows, MacType running in service mode doesn't work in debug mode, and system rendering
    // makes text not clear.
    // See https://github.com/snowie2000/mactype/wiki/Enable-registry-mode-manually
    // on how to make MacType run in registry mode.
    if(!Musec::Native::isDebuggerPresent())
    {
        QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
    }
}
}