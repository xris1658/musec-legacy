注意事项：  
在编写 Model 之后，要在 QML 端使用 Model，请在 `ModelInitializer` 的构造函数中添加注册 Model 的代码：
```cpp
// 文件：ModelName.hpp
class ModelName;
// 文件：ModelInitializer.cpp
QML_REGISTER_TYPE(ModelName, Musec.Models, 1, 0);
```
此操作会在 `Musec.Models 1.0` 中注册 `ModelName` 类，在 QML 中的类型名与类名相同。
