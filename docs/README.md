# C++ 35 天教程文档站

文档站使用 **Sphinx + MyST Parser + Sphinx Book Theme** 构建，主题配置参考 MLC AI 的 `modern-gpu-programming-for-mlsys` 教程。源教程仍位于仓库根目录、`tutorials/`、`week_xx/` 和 `model_inference_project/`；`docs/content/` 是构建前自动生成的 Markdown 同步副本，不应手工修改。C++ 源码、头文件、CMake 和测试不会复制进 `docs/`，教程中的源码链接会转到 GitHub 仓库。

## 为什么选择 Sphinx Book Theme

- 与参考教程一致，采用适合书籍和系统课程的三栏布局。
- 左侧章节树、右侧页内目录和移动端导航适合连续阅读 35 天内容。
- 内置全文搜索、深浅色模式、GitHub 入口和代码复制按钮。
- MyST 与 Mermaid 扩展可继续使用仓库现有 Markdown 和流程图。

## 安装依赖

建议使用虚拟环境：

```bash
python3 -m venv .venv-docs
source .venv-docs/bin/activate
python3 -m pip install -r docs/requirements.txt
```

## 严格构建

在仓库根目录执行：

```bash
./docs/build.sh
```

脚本会先同步源教程并生成分层导航，再执行等价的严格构建：

```bash
./docs/sync_from_repo.sh
rm -rf docs/_build
sphinx-build -b html -W -n --keep-going -E -a docs docs/_build/html
```

`docs/content/`、`docs/_generated/` 和 `docs/_build/` 都是可重新生成的目录，已加入 `.gitignore`。`-W -n` 会把构建告警和缺失引用视为失败，`--keep-going` 会继续收集全部问题，`-E -a` 保证不沿用旧环境或旧页面。成功后打开 `docs/_build/html/index.html`。

## 本地预览

在仓库根目录运行启动脚本，它会自动创建并复用 `.venv-docs`、安装缺失的固定版本依赖、严格构建最新网页，再在后台启动本地服务：

```bash
./docs/start_server.sh
```

然后访问 <http://localhost:8000/>。关闭服务时运行：

```bash
./docs/stop_server.sh
```

服务 PID 和日志保存在已被忽略的 `docs/_build/` 中，关闭脚本只会终止这一个预览进程。需要更换监听地址或端口时，可使用 `DOCS_HOST` 和 `DOCS_PORT`；桌面环境下设置 `DOCS_OPEN_BROWSER=1` 可在启动后尝试自动打开浏览器：

```bash
DOCS_PORT=8080 DOCS_OPEN_BROWSER=1 ./docs/start_server.sh
```

## 同步文档

只重新生成 `docs/content/` 和 Sphinx 导航：

```bash
./docs/sync_from_repo.sh
```

同步脚本只复制 Markdown，并排除构建目录和审查报告。源代码链接会转换到 `https://github.com/mlbo/cpp_learning/blob/master/`，因此文档目录不再保存重复代码。

## GitHub Pages

仓库中的 `.github/workflows/docs.yml` 会在 `main` 或 `master` 分支推送时安装固定版本的文档依赖、执行严格 Sphinx 构建并部署 `docs/_build/html`。GitHub 仓库的 `Settings → Pages → Build and deployment` 应选择 **GitHub Actions**，不再选择“从 `/docs` 目录直接发布”。正式入口是 <https://mlbo.github.io/cpp_learning/>。
