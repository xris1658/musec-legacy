#include "Render.hpp"

#include "native/Native.hpp"

#include <QQuickWindow>

namespace Musec::UI::Render
{
void setSystemRender()
{
    // 检测应用自身是否处于调试状态。若有则不使用系统渲染。
    // Windows 平台: 服务模式的 MacType 在调试模式下无法正常工作，系统渲染会导致文字难以阅读。
    // 可以参照 https://github.com/snowie2000/mactype/wiki/Enable-registry-mode-manually
    // 将 MacType 改为注册表模式。
    if(!Musec::Native::isDebuggerPresent())
    {
        QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
    }
}
}