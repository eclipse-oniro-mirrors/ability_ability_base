# AGENTS.md — ability_base

面向在 `foundation/ability/ability_base` 工作的编码 agent。本仓是元能力子系统（`ability`）的"元能力基础定义"部件（部件名 `ability_base`，见 bundle.json:13-14），提供 Want 启动参数、基础数据类型 wrapper、URI、Configuration、zip 解压工具及 cwant C API；ability_runtime / form_fwk / dmsfwk 等下游仓依赖本仓的序列化与数据模型。

## 1. 范围与优先级

- 本文件适用于仓库根及全部子目录；本仓内无嵌套 AGENTS.md / CLAUDE.md / .cursorrules。若后续子目录出现指引文件，该目录范围内以最近的指引为准，并仍须遵守本文件 §5 公共边界。
- 指令优先级：**用户当前指令 > 本文件 > 仓库其它文档**。
- 证据优先（evidence first / no fabrication）：所有路径、命令、事实以仓库文件为准，禁止凭记忆编造；引用代码统一用 `path:line` 格式。
- 本仓是源码树内的部件仓：**构建入口在源码树根（本仓目录向上三级、含 `build.sh` 的目录），不要在本仓目录内执行构建**。
- 构建框架本身的深度问题（GN 模板、hb、打包）路由到源码树根 `build/AGENTS.md`。
- 维护规则：新增模块、构建 target 或知识文档时，必须同步更新本文件对应章节（代码地图 / 知识路由），防止文档与仓库漂移。

## 2. 快速构建与测试

构建命令从 OpenHarmony 源码树根目录执行（不在本仓目录内）。以下统一以 `rk3568` 产品为例，使用时替换为实际产品名，产物路径中的 `out/rk3568/` 同步替换。

构建与测试定义文件：模块注册与部件测试入口见根 `BUILD.gn` 与 `bundle.json`；单测 target 定义于 `test/unittest/BUILD.gn`（group("unittest") 汇总全部 ohos_unittest）；fuzz target 定义于 `test/fuzztest/BUILD.gn`。

```bash
# 在源码树根执行（不是本仓目录）
cd <源码树根>   # 本仓向上三级

# 构建单个模块。target 名（base/want 等）过于通用，必须用完整 path:target 形式
./build.sh --product-name rk3568 --build-target foundation/ability/ability_base:want

# 构建整个部件
./build.sh --product-name rk3568 --build-target out/rk3568/build_configs/ability/ability_base:ability_base

# 构建单个单测（全部 target 名见 test/unittest/BUILD.gn 的 group("unittest")，如 want_test / extractor_test / cwant_test）
./build.sh --product-name rk3568 --build-target foundation/ability/ability_base/test/unittest:want_test

# 构建 fuzz（目录名与 target 名见 test/fuzztest/BUILD.gn）
./build.sh --product-name rk3568 --build-target foundation/ability/ability_base/test/fuzztest/wantfirst_fuzzer:WantFirstFuzzTest
```

- 产物位置：共享库 `out/rk3568/innerkits/ohos-arm/ability_base/<module>/`（架构目录构成为 `<target_os>-<target_cpu>`）；单测二进制 `out/rk3568/tests/unittest/ability_base/ability_base/<module_out_path>/`（module_out_path 定义于各 ohos_unittest，如 `want_test`、`base_test`）。
- 单测/fuzz 为交叉编译产物，须部署到目标设备运行，构建机无法直接执行；无设备时如实报告"已编译、未运行"。
- 仓内无独立 lint / check 脚本（无 package.json、Makefile、CI workflow）；**编译通过即最小门禁**。

## 3. 代码地图

全部模块在根 `BUILD.gn` 注册，源码都在 `interfaces/` 下。

| GN target | 源码路径 | 职责 / 风险特征 |
|---|---|---|
| `base` | `interfaces/inner_api/base/` | Want 基础类型 wrapper（bool/int/string/byte/long/short/float/double/zchar/remote_object/user_object）；Parcel 序列化密集，高频调用 |
| `want` | `interfaces/kits/native/want/` | Want / WantParams / Operation / PacMap；反序列化历史缺陷热区 |
| `zuri` | `interfaces/kits/native/uri/` | URI 解析（scheme / SSP / fragment） |
| `configuration` | `interfaces/kits/native/configuration/` | 系统环境参数（JSON 转换） |
| `view_data` | `interfaces/kits/native/view_data/` | 页面节点 / Rect 视图数据 |
| `session_info` | `interfaces/kits/native/session_info/` | 扩展会话信息 |
| `string_utils` | `interfaces/kits/native/extractortool/src/file_path_utils.cpp` | 字符串与路径工具（SplitString / JoinString / FixExtName 等）；同时也有 Windows 与 macOS 平台产物，用于 OHOS SDK 的 previewer，修改时须确保 WINDOWS_PLATFORM 与 MAC_PLATFORM 两个条件宏分支均能编译通过 |
| `extractortool` | `interfaces/kits/native/extractortool/` | zip/HAP 解压；Zip Slip / zip bomb / TOCTOU 历史热区 |
| `extractresourcemanager` | `interfaces/kits/native/extractortool/src/extract_resource_manager.cpp` | 资源提取 |
| `ability_base_want` | `interfaces/kits/c/cwant/` | C API（`libability_base_want.so`，@since 15，@kit AbilityKit）；指针/长度边界热区 |

其它关键文件：
- `bundle.json` — inner_kits 头文件合约（:45-157）、外部依赖、测试注册（:158-161）
- `ability_base.gni` — 仓内路径变量（ability_base_path 等，新增 BUILD.gn 引用它而非硬编码路径）
- `interfaces/inner_api/log/include/ability_base_log_wrapper.h` — 日志宏（domain 0xD001305，各模块用 `ABILITYBASE_LOG_TAG` 区分）
- `interfaces/kits/c/common/ability_base_common.h` — C API 错误码（0 = NO_ERROR，401 = PARAM_INVALID）

任务 → 先看哪里（Where to look）：

| 任务 | 先看 |
|---|---|
| Want 携带参数 / 序列化问题 | `interfaces/kits/native/want/src/want_params.cpp`、`interfaces/kits/native/want/src/want.cpp` |
| 新增基础类型 wrapper | `interfaces/inner_api/base/src/*_wrapper.cpp`（同族 11 个，照抄结构）+ bundle.json inner_kits 注册 |
| C API 入参 / 返回值 / 生命周期 | `interfaces/kits/c/cwant/src/want.cpp`、`interfaces/kits/c/cwant/src/want_manager.cpp` |
| URI 解析 / 新 scheme 支持 | `interfaces/kits/native/uri/src/uri.cpp` |
| 解压 / 路径穿越问题 | `interfaces/kits/native/extractortool/src/zip_file.cpp`、`interfaces/kits/native/extractortool/src/zip_file_reader.cpp`、`interfaces/kits/native/extractortool/src/extractor.cpp`、`interfaces/kits/native/extractortool/src/file_path_utils.cpp` |
| 环境参数（语言 / 横竖屏等） | `interfaces/kits/native/configuration/src/configuration.cpp` |
| 加依赖 / 导出头文件 | `bundle.json`（deps.components / inner_kits） |

## 4. 知识路由（每条带触发条件）

**代码检视（意图路由）**：用户说"检视一下代码 / code review / 深度扫描 / 全面排查"→ 读 `skills/codecheck/README.md`（三层架构总入口，由 orchestrator 自动探测路径特征、调度 scanner、合并报告）。用户指定单一维度（纯安全 / 纯逻辑 / 纯外部输入）→ 直接读 `skills/codecheck/scanners/<security|logic|input>-scanner/SKILL.md`。

按任务触发：

| 触发条件 | 先读 |
|---|---|
| 修反序列化 / Parcel 缺陷 | `skills/codecheck/patterns/ipc-serialization.md` + `skills/codecheck/patterns/known-defect-patterns/G03_deserialization_quartet.md` |
| 改解压 / 文件路径逻辑 | `skills/codecheck/patterns/known-defect-patterns/G06_path_traversal_zip_slip.md` |
| 改 cwant C API 边界 | `skills/codecheck/patterns/known-defect-patterns/hotspot-modules.md`（cwant 行） |
| 提交 PR 前自检 | `.gitee/PULL_REQUEST_TEMPLATE.zh-CN.md`（稳定性自检 17 项 + 安全编码自检 22 项） |
| 构建系统报错 / 构建框架问题 | 源码树根 `build/AGENTS.md`、`build/docs/FAQ.md` |
| 新增历史缺陷模式（crash 复盘后） | `skills/codecheck/patterns/INDEX.md` 头部的 SOP |

按路径触发：

| 改动路径 | 先读 |
|---|---|
| `interfaces/kits/native/want/src/want_params*.cpp` | `skills/codecheck/patterns/input-validation.md`（INP-001/008/010） |
| `interfaces/inner_api/base/src/` | `skills/codecheck/patterns/ipc-serialization.md` |
| `interfaces/kits/native/extractortool/` | G06 + `skills/codecheck/patterns/resource-lifecycle.md` |
| `interfaces/kits/c/cwant/` | `skills/codecheck/patterns/known-defect-patterns/hotspot-modules.md` |

按词汇触发：

| 任务描述出现 | 先读 |
|---|---|
| 序列化 / 反序列化 / Marshalling / Parcel | `skills/codecheck/patterns/ipc-serialization.md` |
| 崩溃 / UAF / 越界 / 溢出 | `skills/codecheck/patterns/memory-safety.md` |
| 死锁 / 竞态 / 并发 | `skills/codecheck/patterns/concurrency.md` |
| 泄漏（fd / 内存 / mmap） | `skills/codecheck/patterns/resource-lifecycle.md` |
| 敏感信息 / 日志红线 | `skills/codecheck/patterns/privilege-auth.md`（AUTH-002） |

领域术语表（术语 → 文件锚点）：

| 术语 | 含义 | 文件锚点 |
|---|---|---|
| Want | 组件启动参数对象（显式 / 隐式启动） | `interfaces/kits/native/want/include/want.h` |
| WantParams | Want 自定义参数容器，Parcel 序列化核心，历史缺陷热区 | `interfaces/kits/native/want/include/want_params.h` |
| wrapper（*_wrapper） | 基础类型装箱类（bool/int/string 等 11 个），支持 Marshalling | `interfaces/inner_api/base/include/base_interfaces.h` |
| PacMap | 键值对容器（支持 JSON 转换） | `interfaces/kits/native/want/include/pac_map.h` |
| PatternsMatcher | 匹配器（Skills 已于 2026-11 死代码下线，仅存本类） | `interfaces/kits/native/want/include/patterns_matcher.h` |
| ElementName | 目标组件标识（deviceId / abilityName / bundleName） | `interfaces/kits/native/want/include/element_name.h` |
| SSP | URI 的 scheme-specific part（GetSchemeSpecificPart()） | `interfaces/kits/native/uri/include/uri.h` |
| cwant / ability_base_want | C API（libability_base_want.so，@since 15） | `interfaces/kits/c/cwant/include/want.h` |
| Zip Slip / zip bomb | 解压路径穿越 / 压缩炸弹（extractortool 历史缺陷类） | `interfaces/kits/native/extractortool/src/zip_file.cpp` |
| MAX_RECURSION_DEPTH | WantParams 反序列化递归深度上限（100） | `interfaces/kits/native/want/src/want_params.cpp:210` |
| HAP | Harmony Ability Package，应用安装与运行的基本单元；本仓 extractortool 的解压对象 | `interfaces/kits/native/extractortool/src/extractor.cpp` |

编辑前自检：动代码前先按上述各表确定任务类别并加载对应文档，在最终回复中声明「任务类别 + 已读文档 + 命中的约束」；未命中任何触发条件时声明"无匹配路由"。

## 5. 约束与边界

禁止项（Do not）：
- **不得变更 `interfaces/` 下公共头文件的签名 / 语义 / 错误码取值**——它们是 bundle.json inner_kits 合约（部件间接口）与 C API（@since 15 版本合约），下游多仓直接依赖；`interfaces/`、`test/`、`BUILD.gn`、`bundle.json`、`ability_base.gni`、`OAT.xml` 均在 `.gitee/CODEOWNERS` 管控下。
- `base` / `configuration` / `zuri` / `want` 四个 so 通过 `version_script` 裁剪导出符号（见 BUILD.gn:65/112/149/237）；新增或变更导出符号必须同步更新对应的 `libbase.map` / `libconfiguration.map` / `libzuri.map` / `libwant.map`，漏改会导致符号对外不可见。
- `ReadFromParcel` / `WriteToParcel` 字段顺序两端必须对齐；不得单侧增删字段（跨进程兼容性）。
- 不得删除或放宽 `MAX_RECURSION_DEPTH` 等深度 / 长度 / 范围校验（interfaces/kits/native/want/src/want_params.cpp:210）。
- 日志不得打印文件路径、内存地址、敏感信息；确需输出时用 `%{private}` 修饰参数（.gitee/PULL_REQUEST_TEMPLATE.zh-CN.md:41）；新增日志必须走 `ability_base_log_wrapper.h` 宏并设置正确的 `ABILITYBASE_LOG_TAG`。
- 错误码只用 `ability_base_common.h` 已定义取值（0 / 401），不得自创。
- 不得将同一 cpp 编入多个 so（PR 模板稳定性自检项）。
- 新增源文件必须带 Apache 2.0 许可证头（OAT.xml 对源码做 license header 检查），文件命名跟随现状 snake_case。
- 不做与任务无关的重构、格式化或命名调整；不回滚用户已有改动；工作区有脏文件时只处理与当前任务直接相关的文件。
- 仓内无 generated 标记目录；若后续出现此类目录，禁止手改。
- 检视任务只出报告不改码（`skills/codecheck/conventions.md` §3：修复由用户确认后另起任务）。

需上报项（Ask before）：
- 修改 / 删除任何 `interfaces/` 头文件的公共接口。
- 变更 C API 错误码或枚举（`ability_base_common.h`）。
- 变更 Parcel 序列化格式（影响所有下游仓的跨进程 / 跨版本兼容）。
- 新增三方依赖（bundle.json `third_party` 当前为空）或外部部件依赖。
- 修改 bundle.json inner_kits 列表或 BUILD.gn 中 `innerapi_tags`（platformsdk / sasdk 等标签变更影响 SDK 管控）。

已知陷阱（取自仓内记录 `skills/codecheck/patterns/known-defect-patterns/hotspot-modules.md`）：
- `interfaces/kits/native/want/src/want_params.cpp` 有 53 处 ReadFromParcel，历史反序列化递归 / OOB / DoS 缺陷 20+ 条——改反序列化逻辑必须同步补 fuzz 用例。
- fuzzer 复制粘贴导致用例无效（重复使用 data、目标与真实入口不一致）——新增 fuzzer 对照 `test/fuzztest/wantfirst_fuzzer/` 结构（含 corpus/、project.xml）写有效语料。
- 单测用 `#define private public` 访问私有成员（want_test.cpp:30-35）——该写法仅限 `test/` 下使用，严禁带入生产代码。

## 6. 验证闭环

最小必做（任何源码改动）：
1. 编译改动模块对应 target（§2 命令）通过。
2. 若改动涉及头文件或 BUILD.gn → 编译整个部件 target（`out/rk3568/build_configs/ability/ability_base:ability_base`，产品名替换规则同 §2）确认无破坏。

任务级检查：
- 反序列化 / 解析逻辑改动 → 编译对应 unittest 与 fuzztest target；被改函数无既有用例时新增用例（`HWTEST_F` 风格，参照 test/unittest/want/want_test.cpp:141）。
- 新增测试文件 → 必须两处注册：`test/unittest/BUILD.gn` 中对应 `ohos_unittest` 的 sources，以及 `group("unittest")` 的 deps（bundle.json 只挂 group，漏一处该测试不参与构建）；新增 fuzzer 同理对照 test/fuzztest/BUILD.gn。
- 新增源文件 → 同步注册到根 `BUILD.gn` 的 sources；若导出头文件 → 同步 bundle.json inner_kits。
- 修改序列化 → 自查 Read/Write 字段顺序与校验（对照 §5 禁止项）。

提交规范：commit 主题用简短中文短语，`git commit -s` 附 Signed-off-by，AI 辅助改动追加 Co-Authored-By；PR 描述需填 IssueNo 并完成自检表（.gitee/PULL_REQUEST_TEMPLATE.zh-CN.md）。未经用户明确请求，不执行 `git commit` / `git push`。

Done 定义：上述编译命令全部通过；能在设备上运行的测试已运行，或已如实标注"未运行"；未引入 §5 禁止项变更，或已按上报项取得用户确认；若新增/变更模块或约束，已同步更新本文件；回复中引用的每个路径已逐一核实存在。

兜底：环境无法构建或无设备时，明确报告"未验证 + 原因 + 建议的验证环境"，禁止声称"测试通过"。
