from __future__ import annotations

from pathlib import Path


DOCS_DIR = Path(__file__).resolve().parent

project = "C++ 35 天系统学习教程"
author = "CPP Learning"
copyright = "2026, CPP Learning"
version = "1.0"
release = "1.0"
language = "zh_CN"

extensions = [
    "myst_parser",
    "sphinx_copybutton",
    "sphinxcontrib.mermaid",
]

source_suffix = {
    ".md": "markdown",
}
root_doc = "index"
exclude_patterns = [
    "_build",
    "README.md",
]

myst_enable_extensions = [
    "attrs_inline",
    "colon_fence",
    "deflist",
    "fieldlist",
    "tasklist",
]
myst_fence_as_directive = ["mermaid"]
myst_heading_anchors = 6

html_theme = "sphinx_book_theme"
html_title = project
html_baseurl = "https://mlbo.github.io/cpp_learning/"
html_logo = "_static/logo.svg"
html_favicon = "favicon.svg"
html_static_path = ["_static"]
html_css_files = ["custom.css"]
html_extra_path = [".nojekyll"]
html_copy_source = False
html_show_sourcelink = False
html_context = {"default_mode": "auto"}
html_theme_options = {
    "show_navbar_depth": 1,
    "show_toc_level": 2,
    "home_page_in_toc": False,
    "use_download_button": False,
    "use_fullscreen_button": False,
    "repository_url": "https://github.com/mlbo/cpp_learning",
    "use_repository_button": True,
}

pygments_style = "sphinx"
pygments_dark_style = "monokai"

# Render Mermaid in the browser so building the site does not require Node.js.
mermaid_output_format = "raw"
mermaid_version = "11.12.1"
# sphinxcontrib-mermaid's optional fullscreen wrapper forces every inline SVG
# into the same fixed-size box. Native Mermaid sizing respects each diagram's
# own viewBox and is much better suited to a mix of wide and tall teaching diagrams.
mermaid_fullscreen = False

DAY_DOCS = [
    f"content/week_{week:02d}/day_{day:02d}/README"
    for week, days in enumerate(
        (range(1, 8), range(8, 15), range(15, 22), range(22, 29), range(29, 36)),
        start=1,
    )
    for day in days
]


def _day_page_context(app, pagename, templatename, context, doctree):
    del templatename, doctree
    if pagename not in DAY_DOCS:
        return

    index = DAY_DOCS.index(pagename)
    context["prev"] = None
    context["next"] = None

    def relation(docname: str) -> dict[str, str]:
        title_node = app.env.titles.get(docname)
        title = title_node.astext() if title_node is not None else docname
        return {
            "link": app.builder.get_relative_uri(pagename, docname),
            "title": title,
        }

    if index > 0:
        context["prev"] = relation(DAY_DOCS[index - 1])
    if index + 1 < len(DAY_DOCS):
        context["next"] = relation(DAY_DOCS[index + 1])


def setup(app):
    app.connect("html-page-context", _day_page_context)
