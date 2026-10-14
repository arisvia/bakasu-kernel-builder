# Kernel Builder - 后续架构与特性演进规划 (Roadmap)

本项目致力于打造现代化、通用型 Android 内核自动化编译与优化系统。本文档记录后续迭代的架构设计与技术演进方向。

---

## 🗺️ 架构与工程规范

### 1. 分支命名体系规范化
- **现状**：当前 SDM845 统一内核源码维护在 `sdm845` 分支；
- **演进**：后续引入跨大版本（如 Linux 4.9、5.x 或主线 6.x）或多芯片族时，演进为 **`<soc>-<kernel_version>`** 规范（如 `sdm845-4.19`、`sm8150-4.14`、`gki-5.10`），彻底杜绝跨内核版本混淆。

### 2. 刷机包官方签名与 ZipAlign 标准化
- **目标**：在 CI 构建的打包末尾引入官方 AOSP `testkey`（`testkey.pk8` + `testkey.x509.pem`）自动化签名与 4 字节边界对齐（ZipAlign 4）；
- **价值**：
  - 彻底兼容 OrangeFox (橙狐) / TWRP 开启“Zip Signature Verification”的校验环境；
  - 优化 Recovery `mmap()` 内存映射加载性能。

### 3. 多机型扩展规划
- 保持 `devices/*.env` 纯配置解耦架构；
- 规划引入对同 SoC 兄弟机型的支持（如 `dipper` 小米8、`beryllium` POCO F1、`perseus` 小米 MIX 3）。

---

## 🛡️ 安全与防检测体系 (Anti-Detection)

### 1. SUSFS (Superuser Safe Filesystem) 内核级防检测
- **定位**：由 `simonpunk` 开发的 VFS 级隐身挂载技术；
- **目标**：为 Linux 4.19 及后续 GKI 引入 SUSFS 补丁，彻底对用户态沙盒应用隐藏 `/data/adb`、KSU 挂载点与 Root 迹象，完美通过银行 App、原生检测工具（Native Detector）和大型网游反作弊扫描。

### 2. Unicode 路径混淆逃逸防护
- **目标**：合入 `fs/unicode/` 路径规范化补丁，防御利用非标准 UTF-8 或零宽字符绕过 Mount Namespace 与 Root 路径检查的逃逸漏洞。

---

## 🧩 动态模块与通用 GKI 架构

### 1. KPM (Kernel Patch Module) 评估与分级支持
- **非 GKI / 拥有专属源码树的设备（如 SDM845）**：
  - 坚持 **“纯血原生静态编译”** 路线。所有功能（MGLRU、Re-Kernel、BBR、EROFS 等）在源码层与编译器内联优化融合，零运行时跳板开销，稳定性最高；
- **通用 GKI 2.0 现代设备（如 GKI 5.10 / 5.15 / 6.1）**：
  - 在无厂商专有源码树的纯通用 GKI 镜像构建中，引入 `CONFIG_KPM=y` 作为进阶扩展开关，允许极客在运行态热加载 `.kpm` 动态补丁。

---

## ⚡ 性能与调度进一步微调

1. **`schedutil` 升频延迟微调**：
   - 优化 `up_rate_limit_us`（由默认过缓的 20ms 调优至 500us~1000us），消除现代 Android 15/16 快速滑动的首帧微卡顿。
2. **TCP Fast Open (TFO)**：
   - 默认开启客户端与服务端 TFO 快速连接捎带，缩短网络首屏加载耗时。
