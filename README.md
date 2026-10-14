# kernel-builder

现代化通用 Android 内核构建平台，基于 **GitHub Actions** 提供全自动化编译、优化打包与分发。

无需在本地配置繁杂的 Linux 交叉编译工具链，通过配置文件即可轻松扩展任意机型，一键输出开箱即刷的现代化内核刷机包。

---

## 🌟 核心特性

- 📱 **通用多机型架构**：基于 `devices/<device>.env` 解耦机型配置，添加新设备仅需一个环境配置文件。
- ⚡ **自研 Baka-Flash 刷机引擎**：
  - 集成官方最新 **Magisk v31.0 ARM64 原生 `magiskboot`**；
  - 彻底淘汰 10 年前的 AnyKernel3 遗留架构，零 32 位 BusyBox 依赖；
  - 自动识别 A/B 分区，自动剔除旧 DTB 防止 Double-DTB 破坏，1.5 秒极速刷入。
- 🛡️ **自由切换 Root 方案**：
  - 支持 **BakaSU** (默认推荐)、**SukiSU-Ultra**、**KernelSU-Next**、**KernelSU** 或 **纯净原版内核**。
- 🐳 **灵活功能扩展**：
  - 可选一键集成全部 27 项 Docker / LXC 容器环境内核选项；
  - 可选集成开机黑匣子崩溃探针 (`enable_debug`)，故障排查零门槛。

---

## 📱 支持机型列表

| 设备代号 | 手机型号 | 芯片与内核基线 | 详细技术文档 | 设备配置 |
| :--- | :--- | :--- | :--- | :--- |
| **`polaris`** | 小米 MIX 2S | 骁龙 845 (Linux 4.19) | [📖 polaris 调优报告与特性](docs/devices/polaris.md) | `devices/polaris.env` |

> 欢迎提交 PR 适配更多高通、联发科或 Google Tensor 机型！

---

## 🚀 快速上手：一键构建

1. **进入 GitHub Actions 页面**：点击仓库顶部的 **`Actions`** 选项卡；
2. **选择构建工作流**：在左侧列表中点击 **`Build Kernel`**；
3. **点击 Run workflow 触发构建**：
   - **Target Device Profile**：选择目标机型（例如 `polaris`）；
   - **Superuser / Root Solution**：单选 Root 方案（`bakasu` / `sukisu` / `ksu_next` / `ksu` / `none`）；
   - **Publish Mode**：
     - `release`：自动发布带有 Release 标签的正式版本；
     - `nightly`：覆盖更新每夜预发布版；
     - `artifact_only`：仅在 Actions 生成测试产物（适合日常调试验证，不发 Release）；
   - **Enable Docker / LXC Container Kernel Configs**：按需开启容器支持；
   - **Include boot crash logger & diagnostic probe**：按需开启黑匣子开机排错探针；
4. **下载与刷入**：
   - 编译完成后（约 8~12 分钟），在 Release 页面或 Actions Artifacts 列表中下载生成的 `.zip` 刷机包；
   - 手机进入 Recovery 模式，点击 **Apply update** -> **Apply from ADB**；
   - 电脑终端运行：
     ```bash
     adb sideload <刷机包文件名>.zip
     ```
   - 刷入完成后点击 **Reboot system now** 即可开机。

---

## 🛠️ 添加新设备 (适配指南)

要在本系统内支持一款新设备，只需在 `devices/` 目录下新增一个 `<device>.env` 文件：

```bash
# devices/your_device.env
DEVICE_NAME="your_device"
DEVICE_MODEL="Brand Device Name"

KERNEL_REPO="https://github.com/..."
KERNEL_BRANCH="android-4.19"
ARCH="arm64"
KERNEL_DEFCONFIG="vendor/device_defconfig"
KERNEL_IMAGE_NAME="Image.gz-dtb"
KERNEL_VERSION_TYPE="4.19"

CLANG_BRANCH="main"
CLANG_DIR_NAME="clang-r547379"
KSU_HOOK_METHOD="manual"
```

提交后即可直接在 Actions 下拉列表中选择该机型一键编译！

---

## 📄 授权许可

本项目遵循 [MIT License](LICENSE) 开源。内核源码与各驱动模块遵循其各自的 GPL-2.0 / 上游开源协议。
