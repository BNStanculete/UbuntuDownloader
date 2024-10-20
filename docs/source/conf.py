# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os, subprocess

# -- Project information -----------------------------------------------------

project = 'UbuntuDownloader'
copyright = '2024, HV0rtex'
author = 'HV0rtex'
release = '0.0.1'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinxcontrib.plantuml',
    'sphinx_needs',
    'breathe'
]

templates_path = ['_templates']
exclude_patterns = []

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Options for PlantUML support ---------------------------------------------

on_rtd = os.environ.get('READTHEDOCS') == 'True'

if on_rtd:
    plantuml = 'java -Djava.awt.headless=true -jar /usr/share/plantuml/plantuml.jar'
else:
    plantuml = 'java -jar %s' % os.path.join(os.path.dirname(__file__), "utils", "plantuml.jar")

plantuml_output_format = 'png'

# -- Breathe Configuration ---------------------------------------------------

breathe_default_project = "UbuntuDownloader"

# -- RTD Configuration -------------------------------------------------------

def configureDoxyfile(input_dir, output_dir):
    with open('../Doxyfile.in', 'r') as file :
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', input_dir)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}

if read_the_docs_build:
    input_dir = 'build/include'
    output_dir = 'build/docs/doxygen'
    configureDoxyfile(input_dir, output_dir)
    subprocess.call('doxygen', shell=True)
    breathe_projects['UbuntuDownloader'] = output_dir + '/xml'
