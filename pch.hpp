#ifndef MUSEC_PCH
#define MUSEC_PCH

// SQLite / sqlite_modern_cpp
#include <sqlite_modern_cpp.h>

// yaml-cpp
#include <yaml-cpp/yaml.h>

// CLAP
#include <clap/events.h>
#include <clap/host.h>
#include <clap/entry.h>
#include <clap/plugin.h>
#include <clap/plugin-factory.h>
#include <clap/plugin-features.h>
#include <clap/ext/audio-ports.h>
#include <clap/ext/params.h>
#include <clap/ext/gui.h>
#include <clap/helpers/param-queue.hh>

// VST2
#include <pluginterfaces/vst2.x/aeffect.h>
#include <pluginterfaces/vst2.x/aeffectx.h>
// VST3
#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/ivstattributes.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/vst/ivsthostapplication.h>
#include <pluginterfaces/vst/ivstmessage.h>
#include <pluginterfaces/vst/ivstnoteexpression.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/ivstprocesscontext.h>
#include <pluginterfaces/vst/ivstrepresentation.h>
#include <pluginterfaces/vst/vstspeaker.h>
#include <public.sdk/source/vst/hosting/hostclasses.h>
#include <public.sdk/source/vst/hosting/connectionproxy.h>
#include <public.sdk/source/vst/hosting/parameterchanges.h>

// Qt
#include <QApplication>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QTextCodec>
#include <QUrl>
#include <QtGlobal>
#include <QString>
#include <QList>
#include <QWindow>
#include <QQuickWindow>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QtCore/qobjectdefs.h>
#include <QObject>
#include <QColor>
#include <qqml.h>
#include <QDebug>
#include <QQuickItem>
#include <QThread>
#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <QVector>
#include <QTranslator>

// C/C++ 不涉及模板的头文件
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <charconv>
#include <mutex>
#include <cwchar>

// C/C++ 涉及模板的头文件
#include <algorithm>
#include <array>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <tuple>
#include <thread>
#include <future>
#include <bitset>
#include <set>
#include <map>
#include <variant>
#include <type_traits>
#include <system_error>
#include <list>
#include <functional>
#include <stack>
#include <queue>
#include <fstream>

#endif //MUSEC_PCH
