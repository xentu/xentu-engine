# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
import furo

# -- Project information -----------------------------------------------------

project = 'Xentu Game Engine'
copyright = '2021, Kodaloid'
author = 'Kodaloid'

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autosectionlabel',
	'sphinx_copybutton',
	'sphinxext.opengraph',
	'sphinx.ext.todo'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_static_path = ['_static']
html_theme = 'furo' #stanford_theme
html_theme_path = []
html_logo = "site-logo-dark.png"
html_css_files = [
    'extra.css',
    '.htaccess',
    'favicon.ico'
]


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".


html_theme_options = {
	"announcement": "<em>Important:</em> These docs are a work in progress, please check back when the main website goes live!",
    "light_logo": "site-logo-dark.png",
    "light_css_variables": {
        "color-brand-primary": "#cc1247",
        "color-brand-content": "#cc1247",
    },
    "dark_logo": "site-logo-light.png",
	"dark_css_variables": {
        "color-brand-primary": "#f8b020",
        "color-brand-content": "#f8b020",
    },
}


ogp_site_url="https://docs.xentu.net"
ogp_description_length=200