#!/bin/bash

set -e

# Ensure the muzero package is installed in editable mode
if ! pip show muzero | grep -q 'Location: '; then
  echo "Installing muzero in editable mode..."
  pip install -e .
fi

echo "Running code formatting and quality checks..."

if ! command -v black &> /dev/null; then
    echo "Installing black..."
    pip install black
fi

if ! command -v isort &> /dev/null; then
    echo "Installing isort..."
    pip install isort
fi

if ! command -v autoflake &> /dev/null; then
    echo "Installing autoflake..."
    pip install autoflake
fi

if ! command -v pylint &> /dev/null; then
    echo "Installing pylint..."
    pip install pylint
fi

if ! command -v mypy &> /dev/null; then
    echo "Installing mypy..."
    pip install mypy
fi

if ! command -v pytest &> /dev/null; then
    echo "Installing pytest..."
    pip install pytest pytest-cov
fi

echo "Cleaning up imports..."
autoflake --in-place --recursive --remove-all-unused-imports --remove-unused-variables muzero/ tests/

echo "Sorting imports..."
isort muzero/ tests/ setup.py

echo "Formatting code with black..."
black muzero/ tests/ setup.py

echo "Running type checks..."
mypy muzero/

echo "Running linting..."
pylint --rcfile=setup.cfg muzero/

echo "Formatting complete!"
echo "Running tests..."
pytest