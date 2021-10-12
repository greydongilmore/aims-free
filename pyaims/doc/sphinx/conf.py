# -*- coding: utf-8 -*-
#
# totor documentation build configuration file, created by
# sphinx-quickstart on Mon Jan 24 17:33:44 2011.
#
# This file is execfile()d with the current directory set to its containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

from __future__ import print_function

from __future__ import absolute_import
import sys
import os

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
# sys.path.append(os.path.abspath('.'))
try:
    import matplotlib
    sys.path.append(
        os.path.abspath(os.path.join(os.path.dirname(os.path.dirname(matplotlib.__file__)), 'sphinx', 'ext')))
except Exception as e:
    print('warning:', e)
from soma import aims

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
sys.path.insert(0, os.path.abspath('sphinxext'))

# -- General configuration -----------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be extensions
# coming with Sphinx (named 'sphinx.ext.*') or your custom ones.
try:
    # try napoleon which replaces numpydoc (and googledoc),
    # comes with sphinx 1.2
    import sphinx.ext.napoleon
    napoleon = 'sphinx.ext.napoleon'
except ImportError:
    # not available, fallback to numpydoc
    napoleon = 'numpy_ext.numpydoc'
extensions = ['sphinx.ext.autodoc',
              'sphinx.ext.intersphinx',
              'sphinx.ext.todo',
              'sphinx.ext.coverage',
              'sphinx.ext.ifconfig',
              'sphinx.ext.viewcode',
              'sphinx.ext.extlinks',
              'sphinx.ext.inheritance_diagram',
              'sphinx.ext.autosummary',
              #'sphinx.ext.imgmath',
              napoleon, ]

try:
    # nbsphinx converts ipython/jupyter notebooks to sphinx docs
    import nbsphinx
    nbsphinx_allow_errors = True
    extensions += ['nbsphinx',
                   'sphinx.ext.mathjax']
    # set this env variable to tell notebooks that we should not use
    # any GUI during notebooks execution
    os.environ['ALLOW_GUI'] = '0'
except ImportError:
    nbsphinx = None

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix of source filenames.
source_suffix = '.rst'

# The encoding of source files.
# source_encoding = 'utf-8'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'PyAIMS'
copyright = u'2015, CEA'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = '.'.join([str(x) for x in aims.version()])
# The full version, including alpha/beta/rc tags.
release = aims.versionString()

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
# language = None

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
# today = ''
# Else, today_fmt is used as the format for a strftime call.
# today_fmt = '%B %d, %Y'

# List of documents that shouldn't be included in the build.
# unused_docs = []

# List of directories, relative to source directory, that shouldn't be searched
# for source files.
exclude_trees = []

# List of directories, relative to source directory, that shouldn't be searched
# for source files.
exclude_patterns = ['examples',
                    "_themes/scikit-learn/static/ML_MAPS_README.rst",
                    '_build',
                    '**.ipynb_checkpoints'] \
                   + templates_path

# The reST default role (used for this markup: `text`) to use for all documents.
# default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
# add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
# add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
# show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# A list of ignored prefixes for module index sorting.
# modindex_common_prefix = []


# -- Options for HTML output ---------------------------------------------

# The theme to use for HTML and HTML Help pages.  Major themes that come with
# Sphinx are currently 'default' and 'sphinxdoc'.
html_theme = 'default'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
# html_theme_path = []

# The name for this set of Sphinx documents.  If None, it defaults to
# "<project> v<release> documentation".
# html_title = None

# A shorter title for the navigation bar.  Default is the same as html_title.
# html_short_title = None

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
# html_logo = '../../../../axon/trunk/share/icons/brainvisa.png'

html_style = 'custom.css'

# The name of an image file (within the static path) to use as favicon of the
# docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
# html_favicon = '../../../../communication/web/trunk/gas/favicon.ico'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# If not '', a 'Last updated on:' timestamp is inserted at every page bottom,
# using the given strftime format.
# html_last_updated_fmt = '%b %d, %Y'

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
# html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
# html_sidebars = { '**' : [ 'relations.html' ], }

# Additional templates that should be rendered to pages, maps page names to
# template names.
html_additional_pages = {}

# If false, no module index is generated.
html_use_modindex = True

# If false, no index is generated.
# html_use_index = True

# If true, the index is split into individual pages for each letter.
# html_split_index = False

# If true, links to the reST sources are added to the pages.
# html_show_sourcelink = True

# If true, an OpenSearch description file will be output, and all pages will
# contain a <link> tag referring to it.  The value of this option must be the
# base URL from which the finished HTML is served.
# html_use_opensearch = ''

# If nonempty, this is the file name suffix for HTML files (e.g. ".xhtml").
# html_file_suffix = ''

# Output file base name for HTML help builder.
htmlhelp_basename = 'pyaimsdoc'


# -- Options for LaTeX output --------------------------------------------

# The paper size ('letter' or 'a4').
# latex_paper_size = 'letter'

# The font size ('10pt', '11pt' or '12pt').
# latex_font_size = '10pt'

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title, author, documentclass [howto/manual]).
latex_documents = [
    ('index', 'pyaims.tex', u'PyAIMS Documentation',
     u'someone', 'manual'),
]

# The name of an image file (relative to this directory) to place at the top of
# the title page.
# latex_logo = None

# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
# latex_use_parts = False

# Additional stuff for the LaTeX preamble.
# latex_preamble = ''

# Documents to append as an appendix to all manuals.
# latex_appendices = []

# If false, no module index is generated.
# latex_use_modindex = True

autoclass_content = "both"

extlinks = {
    'aimsalgodox': ('../../aimsalgo-' + version + '/doxygen/%s',
                    'aimsalgodox '),
    'aimsalgoepy': ('../../pyaimsalgo-' + version + '/epydoc/%s',
                    'aimsalgoepy '),
    'aimsalgoex': ('../../pyaimsalgo-' + version + '/examples/%s',
                   'aimsalgoex '),
    'aims': ('%s',
             'aims '),
    'aimsdox': ('../../aimsdata-' + version + '/doxygen/%s',
                'aimsdox '),
    'aimsdata': ('../../aimsdata-' + version + '/%s', 'aimsdata '),
    'aimsepy': ('../../pyaims-' + version + '/epydoc/%s',
                'aimsepy '),
    'cartobdox': ('../../cartobase-' + version + '/doxygen/%s',
                  'cartobdox '),
    'cartoddox': ('../../cartodata-' + version + '/doxygen/%s',
                  'cartoddox '),
    'graphdox': ('../../graph-' + version + '/doxygen/%s',
                 'graphdox '),
    'pyanatomist': ('../../pyanatomist-' + version + '/sphinx/%s',
                    'pyanatomist '),
    'numpy': ('https://numpy.org/%s', 'numpy '),
}

import soma
docpath = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(
                                                       soma.__file__))), 'share', 'doc')
try:
    from soma import aims, aimsalgo
except:
    pass

intersphinx_mapping = {
    'somabase': (os.path.join(docpath, 'soma-base-' + version + '/sphinx'), None),
    'pyaims': (os.path.join(docpath, 'pyaims-' + version + '/sphinx'), None),
    'pyana': (os.path.join(docpath, 'pyanatomist-' + version + '/sphinx'), None),
    'aims': (os.path.join(docpath, 'aimsdata-' + version + '/dev_doc'), None),
    'python': ('http://docs.python.org/'
               + '.'.join([str(x) for x in sys.version_info[:2]]),
               None),
    'numpy': ('https://numpy.org/doc/stable/', None),
}
