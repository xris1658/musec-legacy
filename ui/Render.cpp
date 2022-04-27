#include "Render.hpp"

#include "native/Native.hpp"

#include <QQuickWindow>

namespace Musec::UI::Render
{
void setSystemRender()
{
    // 检测应用自身是否处于调试状态。若有则不使用系统渲染。
    // Windows平台: 在调试模式下 MacType 不起作用，系统渲染会导致文字难以阅读。
    if(!Musec::Native::isDebuggerPresent())
    {
        QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
    }
}
}