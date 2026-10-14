# Xiaomi Mi MIX 2S (`polaris`) - SDM845 Linux 4.19

小米 MIX 2S 深度定制与调优内核文档。

---

## 📋 机型规格与环境

- **SoC**：Qualcomm Snapdragon 845 (Kryo 385, 4x Cortex-A75 @ 2.8GHz + 4x Cortex-A55 @ 1.8GHz)
- **基线内核**：Linux 4.19.325 (CIP SLTS 上游安全补丁分支)
- **目标 ROM**：AxionOS 2.x (Android 15 / 16) 及 LineageOS 21 / 22+
- **构建分支**：`sdm845`
- **配置文件**：`devices/polaris.env`

---

## ✨ 核心特性与深度优化

### 1. Root 解决方案原生集成
- 支持在构建工作流中自由切换：
  - **BakaSU** (v4.2+)：基于原生内核手动系统调用钩子（`CONFIG_KSU_MANUAL_HOOK=y`）；
  - **KernelSU-Next** / **SukiSU-Ultra** / **KernelSU**；
  - **Vanilla**（纯净无 Root 官方内核）。

### 2. 安全与硬件防护
- **Baseband-Guard (基带护盾)** (`CONFIG_BBG=y`)：
  基于 LSM 钩子，物理级拦截任何 Root 派生进程对 `modem`、`fsg`、`fsc`、`persist` 等底层基带及 EFS 分区的意外格式化、清空或破坏性 `ioctl` 操作，放行虚拟块设备。
- **SELinux 策略合规**：
  锁定 `CONFIG_DEFAULT_SECURITY_SELINUX=y` 为默认主安全模块，确保 Android 15/16 严格模式下服务正常初始化。

### 3. 内存与回收优化 (MGLRU & ZRAM)
- **Multi-Gen LRU (MGLRU)** (`CONFIG_LRU_GEN=y`)：
  完整反向移植多代 LRU 内存回收算法，解决 Android 15/16 高负载下的多任务频繁杀后台与内存抖动。
- **ZRAM 默认 LZ4 算法** (`CONFIG_ZRAM_DEF_COMP_LZ4=y`)：
  升级 LZ4 v1.10.0 并加入 ARM64v8 汇编加速（`lz4armv8.S`），解压性能提升 30%；
  开启闲置页回写机制（`CONFIG_ZRAM_WRITEBACK=y`），降低物理内存驻留开销。

### 4. 存储与现代 Android 文件系统兼容
- **全套 EROFS 支持** (`CONFIG_EROFS_FS=y`)：
  完美挂载 AxionOS / LineageOS 现代 ROM 采用的高压缩比只读系统分区（`/system`、`/vendor`），彻底杜绝 Bootloader 引导 Panic。
- **全主板硬件 DTB 覆盖**：
  镜像打包全部 14 款小米 MIX 2S 工程主板设备树（`polaris-p0` ~ `polaris-p3`, `polaris-mp`, `v2`, `v2.1`），自适应任何出厂批次。
- **I/O 调度加速**：
  开启 Kyber 自适应调度器（`CONFIG_MQ_IOSCHED_KYBER=y`），优化 UFS 2.1 队列响应与 PM QoS 投票。

### 5. 后台墓碑与调度优化
- **Re-Kernel v8.5** (`CONFIG_REKERNEL=y`, `CONFIG_REKERNEL_NETWORK=y`)：
  通过内核级 Netlink 通信，配合 NoActive / Scene 等墓碑管理工具，在被冻结应用收到 Binder 调用、系统信号或 TCP 数据包时实现毫秒级快速唤醒。
- **高响应调度**：
  配置 300Hz 调度时钟、RCU Boost 与 Clang ThinLTO 链接时优化；全面剥离高通冗余追踪日志（`CORESIGHT`、`IPC_LOGGING`、`MSM_TZ_LOG`）。

### 6. 网络与跨架构支持
- **网络栈**：默认采用 **TCP BBR** 拥塞控制算法 + **FQ / FQ_CoDel** 队列管理；原生编译内置 **WireGuard**。
- **模拟器同步原语**：反向移植 **NTSYNC** (`/dev/ntsync`)，极速提升 Box64 / Winlator Windows 互操作游戏性能。

---

## 📦 刷入方法 (Baka-Flash)

1. 重启手机进入 Recovery 模式；
2. 选择 **Apply update** -> **Apply from ADB**；
3. 电脑执行：
   ```bash
   adb sideload <刷机包文件名>.zip
   ```
4. 刷机仅需 1.5 秒，完成后点击 **Reboot system now** 即可开机。
