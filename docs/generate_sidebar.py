#!/usr/bin/env python3
from __future__ import annotations

from pathlib import Path


DOCS_DIR = Path(__file__).resolve().parent
CONTENT_DIR = DOCS_DIR / "content"


def _read_title(md_path: Path) -> str | None:
    try:
        for line in md_path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line.startswith("# "):
                return line[2:].strip()
    except Exception:
        return None
    return None


def _md_label(md_path: Path) -> str:
    return _read_title(md_path) or md_path.stem


def _rel_to_docs(path: Path) -> str:
    rel = path.relative_to(DOCS_DIR).as_posix()
    # Keep links robust when paths contain spaces (e.g. "Leecode 专项").
    return rel.replace(" ", "%20")


def _sorted_week_dirs(weeks_dir: Path) -> list[Path]:
    def key(p: Path) -> tuple[int, str]:
        name = p.name
        if name.startswith("week_"):
            try:
                return (int(name.split("_", 1)[1]), name)
            except Exception:
                pass
        return (10_000, name)

    return sorted([p for p in weeks_dir.iterdir() if p.is_dir()], key=key)


def _sorted_day_dirs(week_dir: Path) -> list[Path]:
    def key(p: Path) -> tuple[int, str]:
        name = p.name
        if name.startswith("day_"):
            try:
                return (int(name.split("_", 1)[1]), name)
            except Exception:
                pass
        return (10_000, name)

    return sorted([p for p in week_dir.iterdir() if p.is_dir()], key=key)


def _day_label(day_dir: Path) -> str:
    name = day_dir.name
    if name.startswith("day_"):
        suffix = name.split("_", 1)[1]
        if suffix.isdigit():
            return f"Day {int(suffix):02d}"
    return name


def _week_label(week_dir: Path) -> str:
    name = week_dir.name
    if name.startswith("week_"):
        suffix = name.split("_", 1)[1]
        if suffix.isdigit():
            return f"Week {int(suffix):02d}"
    return name


def _collect_markdown_files(dir_path: Path) -> list[Path]:
    return sorted([p for p in dir_path.iterdir() if p.is_file() and p.suffix.lower() == ".md"])


def _is_report_file(md_path: Path) -> bool:
    return "REPORT" in md_path.name.upper()


def _write_section(lines: list[str], title: str, items: list[tuple[str, str]], indent: int = 0) -> None:
    prefix = " " * indent
    lines.append(f"{prefix}* {title}")
    for label, link in items:
        lines.append(f"{prefix}  * [{label}]({link})")


def main() -> int:
    lines: list[str] = []
    lines.append("* [首页](README.md)")
    lines.append("")

    overview_dir = CONTENT_DIR / "00-overview"
    if overview_dir.exists():
        items: list[tuple[str, str]] = []
        for md in _collect_markdown_files(overview_dir):
            if md.name == "README.md" or _is_report_file(md):
                continue
            items.append((_md_label(md), _rel_to_docs(md)))
        if items:
            _write_section(lines, "概览", items)
            lines.append("")

    tutorials_dir = CONTENT_DIR / "01-tutorials"
    if tutorials_dir.exists():
        items = []
        for md in _collect_markdown_files(tutorials_dir):
            if md.name == "README.md" or _is_report_file(md):
                continue
            items.append((_md_label(md), _rel_to_docs(md)))
        if items:
            _write_section(lines, "教程", items)
            # One-level nested tutorial bundles (e.g. Leecode 专项).
            subdirs = sorted([p for p in tutorials_dir.iterdir() if p.is_dir()], key=lambda p: p.name)
            for sub in subdirs:
                sub_items = []
                for md in _collect_markdown_files(sub):
                    if md.name == "README.md" or _is_report_file(md):
                        continue
                    sub_items.append((_md_label(md), _rel_to_docs(md)))
                if not sub_items:
                    continue
                lines.append(f"  * {sub.name}")
                for label, link in sub_items:
                    lines.append(f"    * [{label}]({link})")
            lines.append("")

    weeks_dir = CONTENT_DIR / "02-weeks"
    if weeks_dir.exists():
        lines.append("* Weeks")
        for week in _sorted_week_dirs(weeks_dir):
            week_readme = week / "README.md"
            if week_readme.exists():
                week_label = _md_label(week_readme)
                week_link = _rel_to_docs(week_readme)
            else:
                week_label = _week_label(week)
                week_link = ""
                for day in _sorted_day_dirs(week):
                    day_readme = day / "README.md"
                    if day_readme.exists():
                        week_link = _rel_to_docs(day_readme)
                        break
                if not week_link:
                    week_link = _rel_to_docs(week)
            lines.append(f"  * [{week_label}]({week_link})")

            for day in _sorted_day_dirs(week):
                day_readme = day / "README.md"
                if not day_readme.exists():
                    continue
                day_label = _day_label(day)
                lines.append(f"    * [{day_label}]({_rel_to_docs(day_readme)})")

            # Week-level topic markdown (e.g. 专题指南)
            week_topics = [p for p in _collect_markdown_files(week) if p.name != "README.md" and not _is_report_file(p)]
            for md in week_topics:
                lines.append(f"    * [{_md_label(md)}]({_rel_to_docs(md)})")
        lines.append("")

    projects_dir = CONTENT_DIR / "03-projects"
    if projects_dir.exists():
        items = []
        for project in sorted([p for p in projects_dir.iterdir() if p.is_dir()], key=lambda p: p.name):
            readme = project / "README.md"
            if not readme.exists():
                continue
            items.append((_md_label(readme), _rel_to_docs(readme)))
        if items:
            _write_section(lines, "Projects", items)
            lines.append("")

    sidebar_path = DOCS_DIR / "_sidebar.md"
    sidebar_path.write_text("\n".join(lines).rstrip() + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
