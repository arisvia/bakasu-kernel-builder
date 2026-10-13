# bakasu-kernel-builder

通用 Android 内核构建项目，基于 **GitHub Actions** 自动化编译集成 [BakaSU](https://github.com/Baka-SU/BakaSU) 的内核刷机包（AnyKernel3 格式）。

无需在本地配置繁杂的 Linux 交叉编译环境，通过 Web 页面点击即可一键构建。支持通过添加设备配置文件（`devices/*.env`）轻松适配手上不同的安卓设备。

---

## 📱 当前已支持设备

| 设备代号 | 机型名称 | 内核版本 | 源码分支 | 适配 ROM | 配置文件 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`polaris`** | 小米 MIX 2S | Linux 4.19.325 | `sdm845` (本仓库独立分支) | AxionOS 2.x (Android 15/16) / LineageOS 21+ | `devices/polaris.env` |


### ✨ `sdm845` (`polaris`) 内核已集成特性
- **Root 框架**：原生集成 [BakaSU](https://github.com/Baka-SU/BakaSU) 手动系统调用钩子（`CONFIG_KSU_MANUAL_HOOK=y`）。
- **基带防护**：集成 **Baseband-Guard**（`CONFIG_BBG=y`），基于 LSM 拦截不可信 Root 派生进程对基带/EFS 等关键分区的写入、篡改与破坏性 `ioctl`，放行 `zram`/`loop`/`dm-*` 虚拟块设备。
- **后台墓碑支持**：集成 **Re-Kernel v8.5**（`CONFIG_REKERNEL=y`、`CONFIG_REKERNEL_NETWORK=y`），通过 Netlink 通知机制配合 Scene / NoActive 等墓碑应用处理冻结进程的 Binder 事务、信号与 TCP 入站数据包解冻。
- **内存与回收子系统**：
  - 完整移植 **Multi-Gen LRU (MGLRU)**（`CONFIG_LRU_GEN=y`、`CONFIG_LRU_GEN_ENABLED=y`），大幅降低 Android 15/16 后台杀进程与内存抖动；
  - **ZRAM & 压缩算法升级**：升级 **LZ4 v1.10.0 + ARM64v8 汇编加速解压**（`lz4armv8.S` + `LZ4_FAST_DEC_LOOP`）与 **64-bit LZO / LZO-RLE**，默认压缩器设为 **`lz4`**（`CONFIG_ZRAM_DEF_COMP_LZ4=y`）并开启闲置页回写（`CONFIG_ZRAM_WRITEBACK=y`）。
- **存储与 I/O 加速**：反向移植高版本 **`blk-mq` & `mq-deadline`** 调度优化（同步请求预留 25% 标签、I/O 优先级、移除冗余关中断）及 **UFS 2.1 PM QoS** 优化（移除 10ms 拖尾延迟投票、仅对 UFS 中断所在 CPU 投票）。
- **Android 15/16 系统调用与安全补丁**：
  - 反向移植 Linux 5.6+ **`openat2(2)`**（syscall `437`）与 Linux 5.14+ **`cgroup.kill`** 原子进程组清理接口，适配 Android 15/16 `lmkd` / Bionic 与容器运行时；
  - 合入 **Linux Foundation CIP SLTS (`v4.19.325-cip116` .. `v4.19.325-cip136`)** 共 **3,982 个** 2025–2026 上游安全漏洞（CVE）与稳定性修复补丁，以及 Binder `kvcalloc` 防碎片化 OOM 修复。
- **网络与协议栈**：默认开启 **TCP BBR** 拥塞控制与 **FQ / FQ_CoDel** 队列调度（`CONFIG_DEFAULT_BBR=y`），原生内置 **WireGuard**（`CONFIG_WIREGUARD=y`）。
- **模拟器与容器扩展**：
  - 完整移植 Linux 6.14 **NTSYNC**（`CONFIG_NTSYNC=y`，`/dev/ntsync`）及 `CONFIG_BINFMT_MISC=y`，提升 Winlator / Box64 / Wine 同步原语性能；
  - Actions 构建支持一键勾选 `enable_docker` 开启全部 27 项 Docker / LXC 容器内核命名空间、Cgroups 与虚拟网络特性。
- **指令集、编译与调试裁剪**：采用 AOSP **Clang 20.0.0 (`clang-r547379`) + LLD 20** 纯 LLVM 编译，针对骁龙 845 Kryo 385 注入 **`-march=armv8.2-a+lse+crc -mtune=cortex-a75`** 硬件原子指令优化，开启 **Clang ThinLTO**、**300Hz** 调度时钟、**RCU Boost**，并彻底剥离 `CORESIGHT` / `IPC_LOGGING` / `MSM_TZ_LOG` / `CPU_BOOST` 等高通遗留调试与过时升频器开销。
---

## 🚀 快速上手：一键构建

1. **进入 GitHub Actions 页面**：
   在你的仓库页面上方点击 **`Actions`** 选项卡。
2. **选择构建工作流**：
   在左侧列表点击 **`Build BakaSU Kernel`**。
3. **手动触发构建**：
   点击右侧的 **`Run workflow`** 下拉按钮：
   - **Target Device Profile**：输入 `polaris`（或选择对应机型配置）。
   - **Enable BakaSU integration**：勾选 `true`。
   - **Enable Docker / LXC Container Kernel Configs**：按需勾选（默认 `false`）。
   - 点击绿色的 **`Run workflow`** 开始编译。
4. **下载产物**：
   - 编译完成后（约 8~15 分钟），进入本次构建详情页面。
   - 页面底部的 **Artifacts** 处可直接下载生成的 `BakaSU-polaris-*.zip`。
   - 同时 Actions 会自动在仓库的 **Releases** 页面发布一份 Release，附带刷机包和内核镜像。

---

## 📲 刷入教程（Xiaomi Mi Mix 2S / polaris）

已编译生成的刷机包为标准的 AnyKernel3 格式，且专门适配 AxionOS 4.19 内核架构：

1. 手机重启进入 **Axion / AOSP 官方 Recovery**（或兼容的第三方 Recovery）。
2. 在 Recovery 菜单中选择 **`Apply update`** -> **`Apply from ADB`**。
3. 电脑连接手机，打开终端执行：
   ```cmd
   adb sideload BakaSU-polaris-XXXXXXXX.zip
   ```
4. 如果 Recovery 提示 `Signature verification failed. Install anyway?`，音量键选择 **`Yes`** 确认刷入。
5. 刷入完成后选择 **`Reboot system now`** 开机。
6. 开机后安装 [BakaSU 管理器 APK](https://github.com/Baka-SU/BakaSU/releases/latest)，即可看到 BakaSU 正常工作并获得完整 Root 权限。

---

## 🛠️ 如何添加更多安卓机型？

本项目采用了**基于配置文件的通用解耦架构**，添加新机型无需修改 Action 工作流代码：

1. 打开 `devices/` 目录。
2. 参考 `devices/template.env`，新建一个以你设备代号命名的文件，如 `devices/dipper.env`（小米 8）或 `devices/raphael.env`（红米 K20 Pro）：
   ```bash
   DEVICE_NAME="dipper"
   DEVICE_MODEL="Xiaomi Mi 8"
   KERNEL_REPO="https://github.com/arisvia/bakasu-kernel-builder"
   KERNEL_BRANCH="sdm845"
   ARCH="arm64"
   KERNEL_DEFCONFIG="vendor/sdm845-perf_defconfig vendor/xiaomi/sdm845-common.config vendor/xiaomi/dipper.config"
   KERNEL_IMAGE_NAME="Image.gz-dtb"
   CLANG_BRANCH="main"
   CLANG_DIR_NAME="clang-r547379"
   KSU_HOOK_METHOD="manual" # 5.10+ GKI 内核填写 tracepoint
   ANYKERNEL3_REPO="https://github.com/osm0sis/AnyKernel3"
   ANYKERNEL3_BRANCH="master"
   ```
3. 提交并推送到 GitHub 仓库。
4. 下次在 GitHub Actions 运行工作流时，直接输入对应的设备代号（如 `dipper`），即可自动拉取该配置进行云端构建！

---

## 📄 鸣谢与参考

- [BakaSU](https://github.com/Baka-SU/BakaSU) (KernelSU Downstream)
- [AnyKernel3](https://github.com/osm0sis/AnyKernel3)
- [AxionOS](https://github.com/AxionOS)
- [xiaomi-smxxx0-devs](https://github.com/xiaomi-smxxx0-devs)
