# bakasu-kernel-builder

通用 Android 内核构建项目，基于 **GitHub Actions** 自动化编译集成 [BakaSU](https://github.com/Baka-SU/BakaSU) 的内核刷机包（AnyKernel3 格式）。

无需在本地配置繁杂的 Linux 交叉编译环境，通过 Web 页面点击即可一键构建。支持通过添加设备配置文件（`devices/*.env`）轻松适配手上不同的安卓设备。

---

## 📱 当前已支持设备

| 设备代号 | 机型名称 | 内核版本 | 适配 ROM | 配置文件 |
| :--- | :--- | :--- | :--- | :--- |
| **`polaris`** | 小米 MIX 2S | Linux 4.19.y | AxionOS 2.x (Android 15/16) / LineageOS 21+ | `devices/polaris.env` |

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
   KERNEL_REPO="https://github.com/xxx/kernel_xiaomi_sdm845-4.19"
   KERNEL_BRANCH="main"
   ARCH="arm64"
   KERNEL_DEFCONFIG="vendor/sdm845-perf_defconfig xiaomi/sdm845-common.config xiaomi/dipper.config"
   KERNEL_IMAGE_NAME="Image.gz-dtb"
   CLANG_VERSION="r510016"
   CLANG_URL="https://android.googlesource.com/platform/prebuilts/clang/host/linux-x86/+archive/refs/heads/main/clang-r510016.tar.gz"
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
