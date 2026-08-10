# G3 反序列化缺"四件套"

> 来源：known-defect-patterns.md G3。

## 特征信号 / grep 线索

- `ReadFromParcel` / `Unmarshalling` / JSON 解析 / 自定义二进制解析中：
  - 从 parcel 读出的 int32/size/count 直接用于 `resize`/`new`/循环次数（无上限校验）。
  - 递归解析嵌套结构无深度计数。
  - 按外部 length 循环读取无总次数/超时限制。
  - `std::stoi`/`stol`、类型转换无 try-catch。
- 解析逻辑位于权限校验代码**之前**（未授权即可触发）。

## 历史案例

- `SkillExecuteResult::ReadFromParcel` 未校验 uriCount → IPC 致 foundation CPU/内存耗尽（鉴权前触发，任意三方 App 可打）。
- `InsightIntentExecuteResult::ReadFromParcel` int32 直传 `resize` → OOM。
- `ParseWantParams` 递归无深度限制 → 开机加载通知 want 时栈耗尽 crash（不开机风险）。
- 44/79 字节 JSON → `ParseWantParamsWithBrackets` 死循环 600s+。
- `ReadFromParcel` 缺容器大小上限 → 内存放大；`EncodeBase64(srcLen=0)` 堆越界。

## 检查点

- 每个解析入口逐一核对四件套：长度/数量上限、递归深度、循环上限、异常兜底。
- 解析调用点是否可能先于鉴权执行？

## 本仓（ability_base）检查点

- `interfaces/kits/native/want/src/want_params.cpp`（53 处 `ReadFromParcel`）：int32 长度/count 是否校验非负与上限后再 `resize`/`new[]`/循环。
- `interfaces/kits/native/want/src/want_params_wrapper.cpp` / `want_params_wrapper_json.cpp`：JSON 解析嵌套深度、`std::stoi` 类转换 try-catch。
- `interfaces/inner_api/base/src/*_wrapper.cpp`：string/array wrapper 读取长度后分配是否有限幅；递归容器（如 value 嵌套 Array）是否有深度计数。
- `interfaces/kits/native/extractortool/src/zip_file.cpp`：zip 条目数/解压大小是否有限制（zip bomb）。
- fuzz 覆盖对照：`test/fuzztest/{wantparams*,want*,arraywrapper*,*_wrapper*}_fuzzer` 是否真实覆盖上述入口。
