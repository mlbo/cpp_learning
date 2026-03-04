# CPP Learning 35 Days

这是把仓库里的学习文档集中到 `docs/` 下，用 **GitHub Pages** 直接发布成网页的版本（无需构建）。

## 站点特点

- 简洁导航（左侧目录）
- 支持 ` ```mermaid ` 代码块渲染
- 适合直接在 GitHub 网页上编辑 Markdown 并自动发布

## 在 GitHub Pages 启用（推荐）

1. 进入仓库 `Settings` → `Pages`
2. `Build and deployment` 选择 `Deploy from a branch`
3. `Branch` 选择默认分支（例如 `main`），目录选择 `/docs`
4. 保存后等待 GitHub 部署完成（几分钟内）

## 本地预览

在仓库根目录执行：

```bash
cd docs
python3 -m http.server 8000
```

然后打开浏览器访问 `http://localhost:8000`。

## 同步文档

如果你在仓库其它目录更新了 Markdown，希望重新复制到 Pages 目录：

```bash
./docs/sync_from_repo.sh
```

## Mermaid 示例

```mermaid
flowchart LR
  A[Learn C++] --> B[Practice LeetCode]
  B --> C[Build Projects]
  C --> D[Review & Summarize]
```
