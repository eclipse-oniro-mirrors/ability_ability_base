# 元能力基础部件

## 简介

**ability_base**部件作为元能力的基础定义部件，提供组件启动参数（Want），系统环境参数（Configuration），URI参数（Uniform Resource Identifier）的定义，用于启动应用，获取环境参数等功能。

**部件内子模块职责**

| 子模块名称       | 职责                                                         |
| ---------------- | ------------------------------------------------------------|
| Want模块          | 组件启动参数模块，开发者可以使用Want携带自定义参数，显示/隐示启动应用，同时支持Pending机制，可本地及跨设备延迟启动目标组件。	|
| Configuration模块 | 系统环境参数模块，支持开发者查询当前环境配置信息，感知系统环境变化。                                            		   |
| URI模块           | URI参数定义模块，提供本地及跨设备资源访问能力，开发者可以使用URI访问文件等资源。											 |
| Base模块          | 基础数据类型模块，提供Boolean，Integer，String等支持Want携带的基础数据类型定义，方便开发者启动过程中传递自定义参数。		   |

## 目录

```
foundation/ability/ability_base
├── frameworks
│   └── js
│       └── napi					# ability_base的napi代码实现
│   └── native 					    # ability_base的核心代码实现
├── interfaces
│   └── inner_api 				    # ability_base的系统内部件间接口 
└── test							# 测试目录
```

## 使用说明
功能模块开发指导可参考[**开发指南**](https://gitee.com/openharmony/docs/blob/master/zh-cn/application-dev/application-models/Readme-CN.md)


## 相关仓
元能力子系统

[**ability_base**](https://gitee.com/openharmony/ability_ability_base)

[ability_runtime](https://gitee.com/openharmony/ability_ability_runtime)

[dmsfwk](https://gitee.com/openharmony/ability_dmsfwk)

[form_fwk](https://gitee.com/openharmony/ability_form_fwk)

[idl_tool](https://gitee.com/openharmony/ability_idl_tool)
