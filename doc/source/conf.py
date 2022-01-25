# ==========================================================================
#  Copyright 2020-2021 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly
#  stated in the License.
#
# ==========================================================================

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.

import os
import sys
import docutils
# sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------

project = 'Intel® DML'
copyright = '2022, Intel'
author = 'Intel'
release = 'v0.1.7-beta'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx_rtd_theme',
    'breathe'
]
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- Breathe configuration -------------------------------------------------

DOXYGEN_OUTPUT = os.path.join(os.path.abspath(os.path.join(os.path.curdir, os.pardir)), 'build', 'doxygen_xml')

breathe_projects = {
    "Intel DML": DOXYGEN_OUTPUT,
    }
breathe_default_project = "Intel DML"

# Tell sphinx what the primary language being documented is.
# primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
# highlight_language = 'cpp'

# -- Options for HTML output -------------------------------------------------

import sphinx_rtd_theme

html_theme = 'sphinx_rtd_theme'
# html_theme = 'sphinx_book_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
# html_static_path = ['_static']
