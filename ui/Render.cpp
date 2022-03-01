#include "Render.hpp"

#include <QQuickWindow>

namespace Musec::UI::Render
{
void setSystemRender()
{
    QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
}
}