Note:  
If a model is added, and you'd like to make use of it in QML codes, please add model registration codes in constructor of `ModelInitializer`:
```cpp
// File: ModelName.hpp
class ModelName;
// File: ModelInitializer.cpp
QML_REGISTERTYPE(ModelName, Musec.Models, 1, 0);
```
This will register `ModelName` class in `Musec.Models 1.0`, and the name in QML is same as the class name.