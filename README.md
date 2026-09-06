# 2025RobMaster-Flying-Dart-Embedded
RoboMaster 2025 飞镖系统嵌入式控制代码 | STM32F427 + 双路CAN + FreeRTOS | 9路电机控制、状态机、视觉伺服 | 全国一等奖
# 🎯 RoboMaster 2025 飞镖系统嵌入式控制

[![License](https://img.shields.io/badge/License-No--License-red.svg)](https://choosealicense.com/no-permission/)
[![Platform](https://img.shields.io/badge/Platform-STM32F427VET6-blue.svg)](https://www.st.com/)
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-lightgrey.svg)]()

> **太原理工大学 TRoMaC 战队 · 2025 赛季**  
> 全国一等奖获奖作品（飞镖系统嵌入式软件负责人）

本项目为 RoboMaster 赛场飞镖发射装置的控制层源码。基于 STM32F427，通过 **双路 CAN 总线** 驱动 9 路电机与舵机，在 15 秒发射窗口内实现 **全自动装填、视觉自瞄与发射**，赛季 11 场实战 **零误发射**。

---

## 📌 核心特性

- **分布式电机控制**：双路 CAN（CAN1/CAN2）按功能分组挂载 9 路电机（装填组 5 / 蓄力组 3 / 瞄准组 1），降低总线负载。
- **全自动状态机**：实现 `待机 → 瞄准 → 发射 → 回收` 四状态闭环，支持单发/连发与 4 发弹匣循环。
- **上电自标定**：5 路电机自动找零（堵转计数确认），上电到可发射仅需 **~1.5 分钟**。
- **视觉伺服闭环**：对接视觉上位机，像素偏差 → 速度环，丢靶自动扫靶找回。
- **四重安全联锁**：机构到位 && 视觉对准 && 识别绿灯 && 15s 窗口硬超时 → 才释放舵机，**赛季 110 发零误发射**。

---

## 🛠 技术栈

| 类别 | 详情 |
| :--- | :--- |
| **主控** | STM32F427VET6（HAL 库） |
| **OS** | FreeRTOS（2 ms 周期调度） |
| **通信** | 双路 CAN（RoboMaster 电机协议）、UART（视觉/裁判系统）、PWM（舵机） |
| **算法** | 串级 PID、有限状态机、软件限位、在线热更新 |
| **驱动** | M3508 ×6、M2006 ×2、GM6020 ×1 |
| **工具** | Keil MDK、CubeMX、Git |

---

## 🏗 系统架构

采用「感知 → 决策 → 执行 → 仲裁」四层架构，2 ms 定时中断单入口调度。

（💡 **这里放一张你作品集里的架构图截图**，比如 `docs/arch.png`）

---

## 🚀 快速开始

### 环境依赖
- Keil MDK 5
- STM32CubeMX（生成初始化代码）
- Git

### 编译烧录
1. 克隆仓库：`git clone https://github.com/你的用户名/仓库名.git`
2. 用 Keil 打开 `Project.uvprojx`
3. 编译并烧录到 STM32F427 主控板

---

## 📊 赛季战绩（2025）

| 指标 | 数值 |
| :--- | :--- |
| 正式比赛场次 | 11 场（南部赛区 + 全国赛） |
| 赛季发射总数 | ~110 发 |
| **误发射 / 卡镖** | **0 次** |
| 上电自标定时间 | ~1.5 分钟 |
| 单次循环耗时 | ~4 s |

---

## 📂 仓库结构
├── Core/           # 主控制逻辑（Dart.c, Auto_Load.c）

├── Drivers/        # HAL 库与 CMSIS

├── Middlewares/    # FreeRTOS

├── docs/           # 详细设计文档与截图

└── README.md
---

## ⚠️ 许可证

**版权所有 © 2025 [杨佳睿]。保留所有权利。**  
本仓库代码仅供查看与学习，未经授权禁止修改、分发或使用于商业/竞赛用途。

（如需查看完整技术细节与状态机设计，请参考随附的 [作品集 PDF](链接)）
