#ifndef MUSEC_UI_FONTUTILITY
#define MUSEC_UI_FONTUTILITY

#include <QString>

namespace Musec
{
namespace UI
{
class FontUtility
{
public:
    static bool suffixIsFont(const QString&);
    static void loadFonts();
};
}
}

#endif // MUSEC_UI_FONTUTILITY
