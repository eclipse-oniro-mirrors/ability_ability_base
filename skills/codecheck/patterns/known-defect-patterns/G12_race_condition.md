# G12 条件竞争（Race Condition）

> 来源：known-defect-patterns.md G12。

## 特征信号 / grep 线索

- 回调容器（`mCancelCallbacks_`、`callProxyRecords_`、`observers_` 等）以 `std::vector` / `std::map` / `std::list` 存储，读写无 `mutex` / `rwlock` 保护。
- `Handler` / `Proxy` / `sptr` 引用跨线程传递，注册/注销与触发/销毁不在同一线程。
- `death recipient` 回调与主业务逻辑并发修改同一对象状态。
- "先检查后使用"（Check-Then-Act）模式：判空后立即使用，期间对象可能被其他线程释放。

## 历史案例

- `pending_want_record.cpp` 中 `mCancelCallbacks_` 存在条件竞争风险。
- `LocalCallContainer` 中 `callProxyRecords_` 未加锁 → 多线程并发访问导致 UAF 或记录丢失。
- `AbilityManagerService` 使用 `wmsHandler_` 未上锁 → 窗口生命周期消息与 Ability 状态变更并发触发竞争。
- AMS 接口存在数据竞争引发的内存破坏问题（多次复发）。
- `appmgr` 完全信任 binder death notification，close fd 即可实现进程保活（death 通知与状态清理竞争）。

## 检查点

- 所有回调注册/注销/遍历是否收敛到统一线程或有显式锁保护？
- `Handler` / `Proxy` 引用是否仅在创建线程使用，或已做线程安全设计？
- 对象销毁前是否确保所有异步回调已注销/等待完成？
- 命中一处竞争即 grep 全库同类容器（`vector`/`map` + 回调），按文件分组列出。

## 本仓（ability_base）检查点

- `interfaces/kits/native/want/src/want_params.cpp` 与 `interfaces/inner_api/base/src/*_wrapper.cpp`：`WantParams`/wrapper 是否在跨线程共享（如单例/全局缓存、回调线程复用）而无 `mutex` 保护。
- `interfaces/kits/native/extractortool/src/file_mapper.cpp`/`zip_file_reader.cpp`：内存映射文件句柄在并发打开/关闭间的 Check-Then-Act。
- `interfaces/kits/c/cwant/src/want_manager.cpp`：内部全局管理器/缓存容器的并发读写保护。
