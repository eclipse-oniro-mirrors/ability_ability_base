# 历史复发热点模块清单（ability_base 适配版）

> 审查涉及以下模块时提高强度，并对同类历史问题做回归确认。
> ability_base 为元能力基础定义部件：主要热点集中在**反序列化入口**（`want_params`/`*_wrapper`）、**文件解析**（extractortool）与**C API 边界**（cwant）。
> 来源：known-defect-patterns.md 附录 + ability_runtime 仓（跨子系统关联项在"关联子系统"节）。

## 本仓热点模块

| 模块 | 位置 | 历史问题特征 |
| --- | --- | --- |
| `want_params` / `WantParams::ReadFromParcel` | `interfaces/kits/native/want/src/want_params.cpp`（53 处 ReadFromParcel） | **反序列化无限递归、OOB、参数伪造、DoS、反复 crash（20+ 条）**；长度/深度/index 未校验即 `resize`/`new[]` |
| inner_api base wrapper 族 | `interfaces/inner_api/base/src/*_wrapper.cpp` | 单文件多处空指针、递归栈耗尽、死循环、UAF；字符串/数组 wrapper 反序列化边界；开机链路高频调用 |
| Want 解析族 | `interfaces/kits/native/want/src/want.cpp`、`want_params_wrapper*.cpp` | 深浅拷贝、JSON 反序列化炸弹、参数伪造 |
| extractortool / zip 解析族 | `interfaces/kits/native/extractortool/src/{zip_file.cpp,zip_file_reader.cpp,file_path_utils.cpp,extractor.cpp}` | 路径穿越、Zip Slip、解压炸弹（zip bomb）、fd/内存映射泄漏、TOCTOU |
| cwant C API 边界 | `interfaces/kits/c/cwant/src/{want.cpp,want_manager.cpp}` | 指针/长度入参未校验（nullptr、bufferSize、负值）、Get 结果写入越界、销毁后使用（UAF） |
| uri / configuration / view_data / session_info 序列化 | `interfaces/kits/native/{uri,configuration,view_data,session_info}/src/*.cpp` | `Marshalling`/`ReadFromParcel` 字段对齐、枚举范围校验、版本兼容 |
| 各 `*_fuzzer.cpp` 测试代码 | `test/fuzztest/*_fuzzer/` | 复制粘贴"重复使用 data"，用例无效；fuzz 目标与真实入口不一致 |

## 关联子系统（跨仓同类横扫）

> ability_base 是被引用侧；历史缺陷模式多在上游各仓爆发，本仓为其序列化/数据模型提供方。跨仓联合检视时回归以下条目。

| 模块（关联仓） | 历史问题特征 |
| --- | --- |
| AMS / abilitymgr（ability_runtime） | fuzz 异常、接口鉴权遗漏、生态规则拦截器 UAF（12+ 条） |
| appmgrservice / appmgr（ability_runtime） | 进程管理权限校验缺失、后台管控绕过、死亡通知竞争（14+ 条） |
| amsdialog（ability_runtime） | 签名问题、弹窗劫持、调试暴露、JumpInterceptorDialog 参数注入（10+ 条） |
| uri_permission_manager（ability_runtime） | 硬编码白名单、RawDataToStringVec 未限制循环 → DoS（5+ 条） |
| form_fwk / FormMgr（form_fwk） | JSON 解析未确认 array 类型、FormMgrStub 未做权限管控（4+ 条） |
| ability_record / lifecycle_manager（ability_runtime） | 生命周期回调拉起链路未管控、状态机竞争、敏感信息打印 networkid（5+ 条） |

## 附：组件已知漏洞（CVE）排查提示

- 扫描第三方组件版本（zlib、minizip、libunwind 等），比对已知 CVE；同一 CVE 需确认 ROM/SDK/各分支是否同步修复（历史上同 CVE 多分支重复提单）。
