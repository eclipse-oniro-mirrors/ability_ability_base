# G14 返回值与异常分支处理

> 来源：known-defect-patterns.md G14。

## 特征信号 / grep 线索

- `ReadParcelable` / `ReadFromParcel` 解析链中获取容器（Map/Vector/Array）或子对象后未判空/未校验即解引用。
- `Uri::GetScheme` / `GetPath` / `Want::GetStringParam` / `GetElementName` 等取值接口返回空值时上层未判空即使用。
- `get_instance` / `GetInstance` / 单例或工厂方法失败分支打印日志后返回 `true` / `ERR_OK`，而非错误码。
- `std::stoi` / `std::stol` / JSON 类型转换无 `try-catch` 兜底。
- C API（`OH_AbilityBase_Want_*`）内部把 Native 层异常/空值包成 `ERR_OK` 而非返回明确错误码。

## 历史案例

> 下列为能力子系统跨仓历史案例，ability_base 中同族反模式应映射到**序列化解析链**（`interfaces/inner_api/base/`、`interfaces/kits/native/want/`）与 **C API 外壳**（`interfaces/kits/c/cwant/`）等价位置。

- `ability_manager_stub.cpp` 中 `ReadParcelable` 返回空指针未校验即解引用。
- `bundleMgrHelper->QueryAbilityInfo` 失败分支打印日志但返回 `true` → 异常分支影响正常流程。
- `ability_manager_client_c.cpp` 空指针解引用。
- `request_id_util.cpp` 整数上溢。

## 本仓（ability_base）检查点

- `want_params.cpp` 等 `ReadFromParcel` 族：`ReadParcelableVec` / `ReadParcelableMap` / `ReadRemoteObject` 返回值未判空即写入容器或继续解引用？
- `uri.cpp` / `configuration.cpp` / `view_data` / `session_info`：解析段失败时是否返回明确错误码而非吞掉？
- `want_params_wrapper` / `*_wrapper.cpp`：序列化异常（`ERR_FLATTENED_OBJECT`、`ReadFromParcel` 非 `ERR_OK`）时是否向上透传，而非置空返回成功？
- `extractortool`：zip 读取失败分支是否只打日志不返回错误，导致上层把损坏数据当成功结果使用？
- 所有 `ReadParcelable` / `iface_cast` / 取值接口调用点是否都有判空保护？
- 命中一处即 grep 全库同族 API，按文件分组列出。
